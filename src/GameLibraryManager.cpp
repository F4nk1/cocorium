#include "GameLibraryManager.h"
#include <BeyotaEngine.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstdlib>
#include <iostream>

using json = nlohmann::json;

void GameLibraryManager::LoadLibrary(const std::string& json_path) {
    games.clear();
    std::ifstream f(json_path);
    if (!f.is_open()) {
        beyota_engine->log->Message("Warning: Could not open library file: " + json_path);
        
        // Fallback: Default game if json is missing
        GameInfo def;
        def.id = "101_maze_puzzle_game";
        def.title = "Maze Puzzle Game";
        def.developer = "Acuarius Studio";
        def.description = "Un juego de físicas y puzzles diseñado para desafiar tu mente. Atraviesa el laberinto evitando las trampas.";
        def.working_dir = "/home/f4nk1/Projects/acuarius/101_maze_puzzle_game";
        def.execute_path = "../build/101_maze_puzzle_game/Maze_Puzzle_Game";
        games.push_back(def);
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
        beyota_engine->log->Message("Error parsing games.json: " + std::string(e.what()));
    }
}

void GameLibraryManager::LaunchGame(size_t index) {
    if (index >= games.size()) return;
    
    const GameInfo& g = games[index];
    beyota_engine->log->Message("Launching game: " + g.title);
    
    std::string command = "( cd " + g.working_dir + " && " + g.execute_path + " > /tmp/game_log.txt 2>&1 ; cd /home/f4nk1/Projects/cocorium/build && ./CocoriumLauncher ) &";
    
    std::system(command.c_str());
    
    // Exit launcher to free resources
    beyota_engine->system->quit = true;
}
