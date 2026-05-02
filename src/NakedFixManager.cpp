#include <SKSE/SKSE.h>
#include "NakedFixManager.h"
#include <random>
#include <fstream>
#include <vector>
#include <string_view>

void NakedFixManager::Init() {
    auto scriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();
    if (scriptEventSource) {
        scriptEventSource->AddEventSink<RE::TESObjectLoadedEvent>(this);
        scriptEventSource->AddEventSink<RE::TESCellFullyLoadedEvent>(this);
        SKSE::log::info("Registered for TESObjectLoadedEvent and TESCellFullyLoadedEvent");
    }
}

RE::BSEventNotifyControl NakedFixManager::ProcessEvent(const RE::TESCellFullyLoadedEvent*, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>*) {
    CheckLoadedActors();
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl NakedFixManager::ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*) {
    if (a_event && a_event->loaded) {
        auto tObj = RE::TESForm::LookupByID(a_event->formID);
        if (tObj && tObj->Is(RE::FormType::ActorCharacter)) {
            auto actor = tObj->As<RE::Actor>();
            if (actor && actor->Is3DLoaded()) {
                if (IsUniqueCustomNPC(actor)) {
                    if (IsInAnimationScene(actor)) {
                        return RE::BSEventNotifyControl::kContinue;
                    }
                    if (IsNaked(actor)) {
                        EquipRandomOutfit(actor);
                    }
                }
            }
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}

void NakedFixManager::CheckLoadedActors() {
    auto processList = RE::ProcessLists::GetSingleton();
    if (!processList) return;

    for (auto& handle : processList->highActorHandles) {
        if (auto actor = handle.get()) {
            if (IsUniqueCustomNPC(actor.get())) {
                if (IsInAnimationScene(actor.get())) continue;
                if (IsNaked(actor.get())) {
                    EquipRandomOutfit(actor.get());
                }
            }
        }
    }
}

bool NakedFixManager::IsUniqueCustomNPC(RE::Actor* actor) {
    if (!actor || actor->IsPlayerRef() || actor->IsDead()) return false;
    
    auto base = actor->GetActorBase();
    if (!base) return false;

    auto race = actor->GetRace();
    if (!race) return false;

    // 1. ADIM: Plugin indekslerini kontrol et.
    uint32_t basePluginIndex = (base->GetFormID() >> 24);
    uint32_t racePluginIndex = (race->GetFormID() >> 24);

    // Eğer ActorBase VEYA Race bir moddan geliyorsa devam et.
    // 0x05 ve sonrası modları temsil eder (00:Skyrim, 01:Update, 02:DG, 03:HF, 04:DB).
    bool isModAddedActor = (basePluginIndex >= 0x05);
    bool isModAddedRace = (racePluginIndex >= 0x05);

    if (!isModAddedActor && !isModAddedRace) {
        return false; 
    }

    // 2. ADIM: Sadece insansi karakterlere odaklan.
    // Iskelet, Draugr, Hayalet, Daedra ve diger canavarlari kesinlikle disla.
    static const std::vector<std::string_view> blacklist = {
        "ActorTypeAnimal", "ActorTypeCreature", "ActorTypeUndead",
        "ActorTypeGhost", "ActorTypeDaedra", "ActorTypeDwarven",
        "ActorTypeDragon", "ActorTypeGiant"
    };

    for (const auto& keyword : blacklist) {
        if (base->HasKeywordString(keyword) || race->HasKeywordString(keyword)) {
            return false;
        }
    }
    
    return true;
}

bool NakedFixManager::IsInAnimationScene(RE::Actor* actor) {
    if (!actor) return false;

    // OStim Check
    int oActive = 0;
    if (actor->GetGraphVariableInt("OActive", oActive) && oActive > 0) {
        return true;
    }

    // SexLab Check
    if (actor->HasKeywordString("SexLabActive")) {
        return true;
    }

    // General "Animating" flag check (sometimes used by other frameworks)
    if (actor->HasKeywordString("AnimatingMain") || actor->HasKeywordString("IsAnimating")) {
        return true;
    }

    return false;
}

bool NakedFixManager::IsNaked(RE::Actor* actor) {
    if (!actor) return false;
    
    // Slot 32 (Gövde) kontrol ediliyor
    auto bodyArmor = actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody);
    
    if (!bodyArmor) {
        return true; 
    }
    
    auto base = actor->GetActorBase();
    if (base && base->skin == bodyArmor) return true;
    
    auto race = actor->GetRace();
    if (race && race->skin == bodyArmor) return true;
    
    return false;
}

void NakedFixManager::EquipRandomOutfit(RE::Actor* actor) {
    if (!actor) return;
    
    auto handle = actor->GetHandle();

    SKSE::GetTaskInterface()->AddTask([handle, this]() {
        if (auto actorRef = handle.get()) {
            auto actor = actorRef.get();
            if (!actor || !actor->Is3DLoaded() || actor->IsDead()) return;

            // KRITIK: Islem baslamadan hemen once bir kez daha kontrol et.
            if (IsInAnimationScene(actor) || !IsNaked(actor)) {
                return;
            }

            // Eger karakterin zaten gecerli bir Outfit seti tanimliysa dokunma.
            auto base = actor->GetActorBase();
            if (base && base->defaultOutfit) {
                return;
            }

            std::ofstream statusFile("NakedFollowerFix_STATUS.txt", std::ios::app);
            statusFile << "FILTREYI GECTI VE DEMIR SET VERILDI: " << (actor->GetName() ? actor->GetName() : "Bilinmeyen") << std::endl;

            // Demir Seti FormID'leri
            std::vector<RE::FormID> ironSet = { 0x00012E49, 0x00012E4B, 0x00012E46 };

            actor->SetDefaultOutfit(nullptr, false);
            actor->SetSleepOutfit(nullptr, false);

            auto aem = RE::ActorEquipManager::GetSingleton();
            if (aem) {
                for (auto id : ironSet) {
                    auto item = RE::TESForm::LookupByID<RE::TESObjectARMO>(id);
                    if (item) {
                        actor->AddObjectToContainer(item, nullptr, 1, nullptr);
                        aem->EquipObject(actor, item, nullptr, 1, nullptr, true, true, false, true);
                    }
                }
            }
            
            actor->Update3DModel();
            statusFile.close();
        }
    });
}
