#include "PostgreSQLDatabase.h"
#include "ServerConfigManager.h"
#include "ErrorCodes.h"
#include "Logger.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <enet/enet.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// --- Simulación de Redis para Sesiones Simultáneas ---
struct Session {
    ENetPeer* peer;
    std::string token;
    std::string current_match;
};
std::unordered_map<std::string, Session> active_sessions; // user -> session

void SendLoginResponse(ENetPeer* peer, bool success, Cocorium::ErrorCode code, const std::string& message, const std::string& username) {
    json res;
    res["type"] = "LOGIN_RESPONSE";
    res["success"] = success;
    res["code"] = static_cast<uint32_t>(code);
    res["message"] = message;
    res["username"] = username;
    
    std::string payload = res.dump();
    ENetPacket* packet = enet_packet_create(payload.c_str(), payload.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

int main() {
    Cocorium::Logger::Init("server.log");
    Cocorium::Logger::Info("System", "--- Cocorium Server Starting ---");
    
    // 1. Cargar Configuración
    CocoriumServer::ServerConfigManager config;
    config.LoadConfig();
    int port = config.server_port;

    // 2. Conectar a Base de Datos
    CocoriumServer::PostgreSQLDatabase db;
    if (!db.Connect(config.GetConnectionString())) {
        Cocorium::Logger::Critical("Database", "Failed to connect to database. Exiting.");
        return EXIT_FAILURE;
    }
    Cocorium::Logger::Info("Database", "Connected to PostgreSQL at " + config.db_host);
    
    // 3. Iniciar Red (ENet)
    if (enet_initialize() != 0) {
        Cocorium::Logger::Critical("Network", "Error al inicializar ENet.");
        return EXIT_FAILURE;
    }
    
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    
    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
    if (!server) {
        Cocorium::Logger::Critical("Network", "Error al crear el host del servidor ENet.");
        return EXIT_FAILURE;
    }
    
    Cocorium::Logger::Info("Network", "Cocorium Server escuchando en el puerto " + std::to_string(port) + "...");
    
    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 10) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    Cocorium::Logger::Info("Network", "Conexion entrante desde " + std::to_string(event.peer->address.host) + ":" + std::to_string(event.peer->address.port));
                    event.peer->data = nullptr;
                    break;
                    
                case ENET_EVENT_TYPE_RECEIVE: {
                    std::string data((char*)event.packet->data, event.packet->dataLength);
                    try {
                        json req = json::parse(data);
                        if (req["type"] == "LOGIN_REQUEST") {
                            std::string user = req["username"];
                            std::string pass = req["password"];
                            
                            // Verificar sesión activa
                            if (active_sessions.find(user) != active_sessions.end()) {
                                Cocorium::Logger::Warn("Auth", "Intento de login para usuario ya autenticado: " + user);
                                SendLoginResponse(event.peer, false, Cocorium::ErrorCode::AUTH_USER_ALREADY_LOGGED_IN, "Usuario ya autenticado en otra sesión.", user);
                            } else {
                                // Verificar BD
                                if (db.ValidateUser(user, pass)) {
                                    active_sessions[user] = {event.peer, "token_123", ""};
                                    event.peer->data = new std::string(user); // Asociar peer al username
                                    SendLoginResponse(event.peer, true, Cocorium::ErrorCode::SUCCESS, "Login Exitoso", user);
                                    Cocorium::Logger::Info("Auth", "Usuario " + user + " inició sesión.");
                                } else {
                                    Cocorium::Logger::Warn("Auth", "Credenciales inválidas para usuario: " + user);
                                    SendLoginResponse(event.peer, false, Cocorium::ErrorCode::AUTH_INVALID_CREDENTIALS, "Contraseña incorrecta o usuario no existe.", user);
                                }
                            }
                        }
                        else if (req["type"] == "REGISTER_REQUEST") {
                            std::string user = req["username"];
                            std::string pass = req["password"];
                            
                            if (db.RegisterUser(user, pass)) {
                                // Registro exitoso, responder como LOGIN_RESPONSE para que la UI use el mismo callback
                                SendLoginResponse(event.peer, true, Cocorium::ErrorCode::SUCCESS, "Registro Exitoso! Por favor inicia sesión.", user);
                                Cocorium::Logger::Info("Auth", "Usuario registrado exitosamente: " + user);
                            } else {
                                Cocorium::Logger::Warn("Auth", "Fallo al registrar, usuario ya existe: " + user);
                                SendLoginResponse(event.peer, false, Cocorium::ErrorCode::AUTH_USER_ALREADY_EXISTS, "El nombre de usuario ya está en uso.", user);
                            }
                        }
                    } catch (std::exception& e) {
                        Cocorium::Logger::Error("Network", std::string("Error procesando paquete: ") + e.what());
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }
                
                case ENET_EVENT_TYPE_DISCONNECT: {
                    if (event.peer->data) {
                        std::string* user = (std::string*)event.peer->data;
                        active_sessions.erase(*user);
                        Cocorium::Logger::Info("Auth", "Usuario " + *user + " desconectado.");
                        delete user;
                        event.peer->data = nullptr;
                    }
                    break;
                }
                case ENET_EVENT_TYPE_NONE: break;
            }
        }
    }
    
    db.Disconnect();
    enet_host_destroy(server);
    enet_deinitialize();
    return EXIT_SUCCESS;
}
