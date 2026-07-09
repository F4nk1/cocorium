#include "ConfigManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void ConfigManager::Load(const std::string& path) {
    config_path = path;
    std::ifstream f(path);
    if (!f.is_open()) return; // Defaults will be used
    
    try {
        json j = json::parse(f);
        auto_login = j.value("auto_login", auto_login);
        saved_username = j.value("saved_username", saved_username);
        saved_password = j.value("saved_password", saved_password);
        default_section = j.value("default_section", default_section);
        
        if (j.contains("theme")) {
            auto theme = j["theme"];
            window_rounding = theme.value("window_rounding", window_rounding);
            dark_mode = theme.value("dark_mode", dark_mode);
            
            if (theme.contains("accent_color") && theme["accent_color"].is_array()) {
                auto acc = theme["accent_color"];
                if (acc.size() >= 3) {
                    accent_color[0] = acc[0];
                    accent_color[1] = acc[1];
                    accent_color[2] = acc[2];
                }
            }
        }
    } catch (...) {
        // Ignore parse errors, use defaults
    }
}

void ConfigManager::Save() {
    if (config_path.empty()) return;
    
    json j;
    j["auto_login"] = auto_login;
    j["saved_username"] = saved_username;
    j["saved_password"] = saved_password;
    j["default_section"] = default_section;
    
    j["theme"]["window_rounding"] = window_rounding;
    j["theme"]["dark_mode"] = dark_mode;
    j["theme"]["accent_color"] = {accent_color[0], accent_color[1], accent_color[2]};
    
    std::ofstream f(config_path);
    if (f.is_open()) {
        f << j.dump(4);
    }
}
