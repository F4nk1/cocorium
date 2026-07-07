#ifndef GAME_LIBRARY_MANAGER_H
#define GAME_LIBRARY_MANAGER_H

#include <string>
#include <vector>

struct GameInfo {
    std::string id;
    std::string title;
    std::string developer;
    std::string description;
    std::string execute_path;
    std::string working_dir;
};

class GameLibraryManager {
public:
    static GameLibraryManager& GetInstance() {
        static GameLibraryManager instance;
        return instance;
    }

    void LoadLibrary(const std::string& json_path);
    const std::vector<GameInfo>& GetGames() const { return games; }
    
    void LaunchGame(size_t index);

private:
    GameLibraryManager() = default;
    std::vector<GameInfo> games;
};

#endif
