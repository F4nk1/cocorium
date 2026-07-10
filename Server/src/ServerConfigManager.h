#pragma once
#include <string>

namespace CocoriumServer {

class ServerConfigManager {
public:
    std::string db_host;
    std::string db_port;
    std::string db_user;
    std::string db_password;
    std::string db_name;
    int server_port;

    ServerConfigManager();

    // Lee la configuración (sobreescribe json con variables de entorno)
    void LoadConfig(const std::string& json_path = "server_config.json");

    // Construye el connection string para PostgreSQL
    std::string GetConnectionString() const;
};

} // namespace CocoriumServer
