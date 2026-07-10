#include "ServerConfigManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>

using json = nlohmann::json;

namespace CocoriumServer {

ServerConfigManager::ServerConfigManager() {
    // Valores por defecto
    db_host = "127.0.0.1";
    db_port = "5432";
    db_user = "admin";
    db_password = "root";
    db_name = "cocorium";
    server_port = 7777;
}

void ServerConfigManager::LoadConfig(const std::string& json_path) {
    std::ifstream config_file(json_path);
    if (config_file.is_open()) {
        try {
            json config;
            config_file >> config;
            if (config.contains("port")) server_port = config["port"];
            if (config.contains("db_host")) db_host = config["db_host"];
            if (config.contains("db_port")) db_port = config["db_port"];
            if (config.contains("db_user")) db_user = config["db_user"];
            if (config.contains("db_password")) db_password = config["db_password"];
            if (config.contains("db_name")) db_name = config["db_name"];
            std::cout << "[Config] Cargado desde " << json_path << "\n";
        } catch (...) {
            std::cerr << "[Config] Error leyendo " << json_path << ", usando valores por defecto.\n";
        }
    } else {
        std::cout << "[Config] No se encontró " << json_path << ", usando valores por defecto.\n";
    }

    // Sobrescribir con variables de entorno si existen
    if (const char* env_p = std::getenv("PORT")) server_port = std::stoi(env_p);
    if (const char* env_h = std::getenv("DB_HOST")) db_host = env_h;
    if (const char* env_po = std::getenv("DB_PORT")) db_port = env_po;
    if (const char* env_u = std::getenv("DB_USER")) db_user = env_u;
    if (const char* env_pw = std::getenv("DB_PASSWORD")) db_password = env_pw;
    if (const char* env_n = std::getenv("DB_NAME")) db_name = env_n;
}

std::string ServerConfigManager::GetConnectionString() const {
    return "dbname=" + db_name + " user=" + db_user + " password=" + db_password + " host=" + db_host + " port=" + db_port;
}

} // namespace CocoriumServer
