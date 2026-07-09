#pragma once
#include <string>

class ConfigManager {
public:
    static ConfigManager& GetInstance() {
        static ConfigManager instance;
        return instance;
    }

    void Load(const std::string& path);
    void Save();

    // Preferences
    bool auto_login = true;
    std::string saved_username = "";
    std::string saved_password = "";
    std::string default_section = "Library";
    
    // Theme configurations
    float window_rounding = 12.0f;
    float accent_color[3] = {0.98f, 0.50f, 0.45f}; 
    bool dark_mode = false;

private:
    ConfigManager() = default;
    std::string config_path;
};
