#pragma once
#include <RE/Skyrim.h>

class NakedFixManager : public RE::BSTEventSink<RE::TESObjectLoadedEvent>, public RE::BSTEventSink<RE::TESCellFullyLoadedEvent> {
public:
    static NakedFixManager* GetSingleton() {
        static NakedFixManager singleton;
        return &singleton;
    }

    void Init();
    void CheckLoadedActors();

    // Event Sink implementations
    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_source) override;
    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESCellFullyLoadedEvent* a_event, RE::BSTEventSource<RE::TESCellFullyLoadedEvent>* a_source) override;

private:
    NakedFixManager() = default;
    NakedFixManager(const NakedFixManager&) = delete;
    NakedFixManager& operator=(const NakedFixManager&) = delete;

    bool IsUniqueCustomNPC(RE::Actor* actor);
    bool IsNaked(RE::Actor* actor);
    void EquipRandomOutfit(RE::Actor* actor);
};
