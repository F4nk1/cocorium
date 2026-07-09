#include "DatabaseManager.h"
#include <sqlite3.h>
#include <iostream>

namespace CocoriumServer {

SQLiteDatabase::~SQLiteDatabase() {
    Disconnect();
}

bool SQLiteDatabase::Connect(const std::string& connectionString) {
    if (sqlite3_open(connectionString.c_str(), &db)) {
        std::cerr << "[DB] Error abriendo base de datos: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    
    const char* sql = "CREATE TABLE IF NOT EXISTS Users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "username TEXT UNIQUE NOT NULL, "
                      "password TEXT NOT NULL);";
    char* err = nullptr;
    if (sqlite3_exec(db, sql, 0, 0, &err) != SQLITE_OK) {
        std::cerr << "[DB] Error creando tabla: " << err << "\n";
        sqlite3_free(err);
        return false;
    }
    return true;
}

void SQLiteDatabase::Disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool SQLiteDatabase::ValidateUser(const std::string& username, const std::string& password) {
    if (!db) return false;
    
    std::string query = "SELECT password FROM Users WHERE username = '" + username + "';"; // Note: Needs proper prepared statements for production to avoid SQL injection
    sqlite3_stmt* stmt = nullptr;
    
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string db_pass = (const char*)sqlite3_column_text(stmt, 0);
            sqlite3_finalize(stmt);
            return db_pass == password;
        }
        sqlite3_finalize(stmt);
    }
    
    return false;
}

bool SQLiteDatabase::RegisterUser(const std::string& username, const std::string& password) {
    if (!db) return false;
    
    // Verificar si ya existe
    std::string query = "SELECT id FROM Users WHERE username = '" + username + "';";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return false; // Ya existe
        }
        sqlite3_finalize(stmt);
    }

    std::cout << "[DB] Usuario nuevo, registrando: " << username << "\n";
    std::string insert = "INSERT INTO Users (username, password) VALUES ('" + username + "', '" + password + "');";
    if (sqlite3_exec(db, insert.c_str(), 0, 0, 0) == SQLITE_OK) {
        return true;
    }
    return false;
}

} // namespace CocoriumServer
