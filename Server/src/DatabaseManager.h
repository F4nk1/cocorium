#pragma once
#include <string>

struct sqlite3;

namespace CocoriumServer {

class DatabaseManager {
public:
    virtual ~DatabaseManager() = default;
    
    // Conecta a la base de datos usando un string de conexión (ej. path al archivo sqlite)
    virtual bool Connect(const std::string& connectionString) = 0;
    
    // Desconecta la base de datos
    virtual void Disconnect() = 0;

    // Verifica credenciales.
    virtual bool ValidateUser(const std::string& username, const std::string& password) = 0;

    // Registra un nuevo usuario. Retorna true si tuvo exito, false si ya existe.
    virtual bool RegisterUser(const std::string& username, const std::string& password) = 0;
};

class SQLiteDatabase : public DatabaseManager {
private:
    ::sqlite3* db = nullptr;
public:
    ~SQLiteDatabase() override;
    
    bool Connect(const std::string& connectionString) override;
    void Disconnect() override;
    bool ValidateUser(const std::string& username, const std::string& password) override;
    bool RegisterUser(const std::string& username, const std::string& password) override;
};

} // namespace CocoriumServer
