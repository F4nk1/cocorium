#include "GameLibraryManager.h"
#include <BeyotaEngine.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;

void GameLibraryManager::LoadLibrary(const std::string& json_path) {
    games.clear();
    std::ifstream f(json_path);
    if (!f.is_open()) {
        beyota_engine->log->Message("Warning: Could not open library file: " + json_path);
        return;
    }
    
    try {
        json data = json::parse(f);
        for (const auto& item : data["games"]) {
            GameInfo info;
            info.id = item.value("id", "unknown");
            info.title = item.value("title", "Untitled");
            info.developer = item.value("developer", "Unknown");
            info.description = item.value("description", "");
            info.working_dir = item.value("working_dir", ".");
            info.execute_path = item.value("execute_path", "./game");
            games.push_back(info);
        }
        beyota_engine->log->Message("Loaded " + std::to_string(games.size()) + " games from library.");
    } catch (std::exception& e) {
        beyota_engine->log->Message("Error parsing " + json_path + ": " + std::string(e.what()));
    }
}

void GameLibraryManager::LaunchGame(size_t index) {
    if (index >= games.size()) return;
    
    const GameInfo& g = games[index];
    beyota_engine->log->Message("Launching game: " + g.title);
    
    std::string launcher_path;
    try {
        launcher_path = std::filesystem::canonical("/proc/self/exe").string();
    } catch (const std::exception& e) {
        beyota_engine->log->Message(std::string("Error getting launcher path: ") + e.what());
        launcher_path = "./CocoriumLauncher"; // fallback
    }
    
    // Launch game in background
    std::string command = "( cd \"" + g.working_dir + "\" && \"" + g.execute_path + "\" > /tmp/game_log.txt 2>&1 ) &";
    
    std::system(command.c_str());
}
