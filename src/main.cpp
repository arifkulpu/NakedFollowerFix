#include <SKSE/SKSE.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "NakedFixManager.h"
#include <string_view>
#include <fstream>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace Console {
    bool FixNaked(const RE::SCRIPT_PARAMETER*, RE::SCRIPT_FUNCTION::ScriptData*, RE::TESObjectREFR*, RE::TESObjectREFR*, RE::Script*, RE::ScriptLocals*, double&, std::uint32_t&) {
        NakedFixManager::GetSingleton()->CheckLoadedActors();
        if (auto console = RE::ConsoleLog::GetSingleton()) {
            console->Print("NakedFollowerFix: Scanning and fixing nearby NPCs...");
        }
        return true;
    }

    void Register() {
        auto commands = RE::SCRIPT_FUNCTION::GetFirstConsoleCommand();
        if (!commands) return;

        for (std::uint32_t i = 0; i < RE::SCRIPT_FUNCTION::Commands::kConsoleCommandsEnd; ++i) {
            auto& command = commands[i];
            if (command.functionName && std::string_view(command.functionName) == "ToggleNavMesh") {
                const char* name = "fixnaked";
                const char* sn = "fnk";
                const char* help = "Fixes nearby naked NPCs.";
                
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.functionName), reinterpret_cast<std::uintptr_t>(name));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.shortName), reinterpret_cast<std::uintptr_t>(sn));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.helpString), reinterpret_cast<std::uintptr_t>(help));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.executeFunction), reinterpret_cast<std::uintptr_t>(FixNaked));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.numParams), static_cast<std::uint16_t>(0));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.params), reinterpret_cast<std::uintptr_t>(nullptr));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.compileFunction), reinterpret_cast<std::uintptr_t>(nullptr));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.conditionFunction), reinterpret_cast<std::uintptr_t>(nullptr));
                REL::safe_write(reinterpret_cast<std::uintptr_t>(&command.referenceFunction), false);
                break;
            }
        }
    }
}

namespace
{
    void InitializeLog()
    {
        auto path = SKSE::log::log_directory();
        if (!path) return;

        *path /= "NakedFollowerFix.log"sv;
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
        auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v"s);
    }

    void OnMessage(SKSE::MessagingInterface::Message* a_msg)
    {
        switch (a_msg->type) {
        case SKSE::MessagingInterface::kDataLoaded:
            SKSE::log::info("DataLoaded event alindi.");
            NakedFixManager::GetSingleton()->Init();
            Console::Register();
            NakedFixManager::GetSingleton()->CheckLoadedActors();
            break;
        case SKSE::MessagingInterface::kPostLoadGame:
            SKSE::log::info("Oyun yuklendi, NPCler taraniyor.");
            NakedFixManager::GetSingleton()->CheckLoadedActors();
            break;
        }
    }
}

extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion({ 1, 4, 1 });
    v.PluginName("NakedFollowerFix");
    v.AuthorName("Developer");
    v.UsesAddressLibrary(true);
    v.UsesStructsPost629(true);
    v.CompatibleVersions({ 
        SKSE::RUNTIME_SSE_1_5_97, 
        SKSE::RUNTIME_SSE_1_6_640, 
        REL::Version{ 1, 6, 1130, 0 }, 
        REL::Version{ 1, 6, 1170, 0 } 
    });
    return v;
}();

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse) {
    InitializeLog();
    
    auto runtimeVersion = a_skse->RuntimeVersion();
    SKSE::log::info("NakedFollowerFix 1.4.1 (SE/AE) yukleniyor...");
    SKSE::log::info("Tespit edilen oyun surumu: {}", runtimeVersion.string());

    std::ofstream testFile("NakedFollowerFix_STATUS.txt");
    testFile << "Mod is fully active. Game Version: " << runtimeVersion.string() << std::endl;
    testFile.close();

    SKSE::Init(a_skse);

    auto messaging = SKSE::GetMessagingInterface();
    if (!messaging || !messaging->RegisterListener(OnMessage)) {
        SKSE::log::error("Messaging interface kaydi basarisiz!");
        return false;
    }

    return true;
}
