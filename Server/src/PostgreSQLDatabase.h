#pragma once
#include "DatabaseManager.h"
#include <pqxx/pqxx>
#include <memory>

namespace CocoriumServer {

class PostgreSQLDatabase : public DatabaseManager {
private:
    std::unique_ptr<pqxx::connection> conn;

public:
    ~PostgreSQLDatabase() override;
    
    bool Connect(const std::string& connectionString) override;
    void Disconnect() override;
    bool ValidateUser(const std::string& username, const std::string& password) override;
    bool RegisterUser(const std::string& username, const std::string& password) override;
};

} // namespace CocoriumServer
