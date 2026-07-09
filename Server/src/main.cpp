#include "DatabaseManager.h"
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

void SendLoginResponse(ENetPeer* peer, bool success, const std::string& message, const std::string& username) {
    json res;
    res["type"] = "LOGIN_RESPONSE";
    res["success"] = success;
    res["message"] = message;
    res["username"] = username;
    
    std::string payload = res.dump();
    ENetPacket* packet = enet_packet_create(payload.c_str(), payload.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

int main() {
    std::cout << "--- Cocorium Server Starting ---\n";
    
    // 1. Cargar Configuración
    int port = 7777;
    std::string db_path = "cocorium_prod.db";
    
    std::ifstream config_file("server_config.json");
    if (config_file.is_open()) {
        try {
            json config;
            config_file >> config;
            if (config.contains("port")) port = config["port"];
            if (config.contains("db_path")) db_path = config["db_path"];
            std::cout << "[Config] Loaded from server_config.json\n";
        } catch (...) {
            std::cerr << "[Config] Error parsing server_config.json, using defaults.\n";
        }
    } else {
        std::cout << "[Config] No server_config.json found, using defaults.\n";
    }

    // 2. Conectar a Base de Datos
    CocoriumServer::SQLiteDatabase db;
    if (!db.Connect(db_path)) {
        std::cerr << "Failed to connect to database. Exiting.\n";
        return EXIT_FAILURE;
    }
    std::cout << "[DB] Connected to " << db_path << "\n";
    
    // 3. Iniciar Red (ENet)
    if (enet_initialize() != 0) {
        std::cerr << "[Net] Error al inicializar ENet.\n";
        return EXIT_FAILURE;
    }
    
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    
    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
    if (!server) {
        std::cerr << "[Net] Error al crear el host del servidor ENet.\n";
        return EXIT_FAILURE;
    }
    
    std::cout << "[Net] Cocorium Server escuchando en el puerto " << port << "...\n";
    
    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 10) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "[Net] Conexion entrante desde " << event.peer->address.host << ":" << event.peer->address.port << "\n";
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
                                SendLoginResponse(event.peer, false, "Usuario ya autenticado en otra sesión.", user);
                            } else {
                                // Verificar BD
                                if (db.ValidateUser(user, pass)) {
                                    active_sessions[user] = {event.peer, "token_123", ""};
                                    event.peer->data = new std::string(user); // Asociar peer al username
                                    SendLoginResponse(event.peer, true, "Login Exitoso", user);
                                    std::cout << "[Auth] Usuario " << user << " inició sesión.\n";
                                } else {
                                    SendLoginResponse(event.peer, false, "Contraseña incorrecta o usuario no existe.", user);
                                }
                            }
                        }
                        else if (req["type"] == "REGISTER_REQUEST") {
                            std::string user = req["username"];
                            std::string pass = req["password"];
                            
                            if (db.RegisterUser(user, pass)) {
                                // Registro exitoso, responder como LOGIN_RESPONSE para que la UI use el mismo callback
                                SendLoginResponse(event.peer, false, "Registro Exitoso! Por favor inicia sesión.", user);
                                std::cout << "[Auth] Usuario registrado: " << user << "\n";
                            } else {
                                SendLoginResponse(event.peer, false, "El nombre de usuario ya está en uso.", user);
                            }
                        }
                    } catch (std::exception& e) {
                        std::cerr << "[Net] Error procesando paquete: " << e.what() << "\n";
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }
                
                case ENET_EVENT_TYPE_DISCONNECT: {
                    if (event.peer->data) {
                        std::string* user = (std::string*)event.peer->data;
                        active_sessions.erase(*user);
                        std::cout << "[Auth] Usuario " << *user << " desconectado.\n";
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
