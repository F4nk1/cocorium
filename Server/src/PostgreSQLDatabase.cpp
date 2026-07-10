#include "PostgreSQLDatabase.h"
#include <iostream>
#include <bcrypt/BCrypt.hpp> 

namespace CocoriumServer {

PostgreSQLDatabase::~PostgreSQLDatabase() {
    Disconnect();
}

bool PostgreSQLDatabase::Connect(const std::string& connectionString) {
    try {
        // El constructor lanza una excepción si falla, no es necesario verificar is_open() inmediatamente después
        conn = std::make_unique<pqxx::connection>(connectionString);
        
        // Inicializar tabla si no existe
        pqxx::work w(*conn);
        w.exec("CREATE TABLE IF NOT EXISTS Users ("
               "id SERIAL PRIMARY KEY, "
               "username VARCHAR(50) UNIQUE NOT NULL, "
               "password_hash TEXT NOT NULL);");
        w.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[DB] Error conectando a PostgreSQL: " << e.what() << "\n";
    }
    return false;
}

void PostgreSQLDatabase::Disconnect() {
    if (conn) {
        // Resetear el unique_ptr cierra la conexión de forma segura invocando el destructor de pqxx::connection
        conn.reset();
    }
}

bool PostgreSQLDatabase::ValidateUser(const std::string& username, const std::string& password) {
    if (!conn || !conn->is_open()) return false;
    
    try {
        pqxx::nontransaction ntx(*conn);
        
        // DOCUMENTACIÓN OFICIAL: Usar exec_params pasa los datos de forma separada al motor.
        // Es 100% inmune a SQL Injection y más rápido.
        pqxx::result r = ntx.exec(
            "SELECT password_hash FROM Users WHERE username = $1;", 
            pqxx::params{username}
        );
        
        if (!r.empty()) {
            std::string db_pass_hash = r[0][0].as<std::string>();
            return BCrypt::validatePassword(password, db_pass_hash);
        }
    } catch (const std::exception& e) {
        std::cerr << "[DB] Error en ValidateUser: " << e.what() << "\n";
    }
    
    return false;
}

bool PostgreSQLDatabase::RegisterUser(const std::string& username, const std::string& password) {
    if (!conn || !conn->is_open()) return false;
    
    try {
        pqxx::work w(*conn);
        
        // Verificar si ya existe usando parámetros ($1)
        pqxx::result r = w.exec(
            "SELECT id FROM Users WHERE username = $1;", 
            pqxx::params{username}
        );
        
        if (!r.empty()) {
            return false; // El usuario ya existe
        }
        
        std::string hashedPass = BCrypt::generateHash(password, 12);
        
        // Inserción limpia y segura
        w.exec(
            "INSERT INTO Users (username, password_hash) VALUES ($1, $2);", 
            pqxx::params{username, hashedPass}
        );
        
        w.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[DB] Error en RegisterUser: " << e.what() << "\n";
    }
    
    return false;
}

} // namespace CocoriumServer