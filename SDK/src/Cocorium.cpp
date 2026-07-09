#include <CocoriumSDK/Cocorium.h>
#include <iostream>
#include <enet/enet.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Cocorium {

    static ENetHost* client = nullptr;
    static ENetPeer* peer = nullptr;
    
    static std::string currentUser = "";
    static bool loggedIn = false;
    
    static bool hasLoginResponse = false;
    static LoginResult lastLoginResult;

    bool Initialize(const std::string& host, int port) {
        if (enet_initialize() != 0) {
            std::cerr << "[Cocorium SDK] Error inicializando ENet.\n";
            return false;
        }

        client = enet_host_create(NULL, 1, 2, 0, 0);
        if (client == NULL) {
            std::cerr << "[Cocorium SDK] Error creando el cliente ENet.\n";
            return false;
        }

        ENetAddress address;
        enet_address_set_host(&address, host.c_str());
        address.port = port;

        peer = enet_host_connect(client, &address, 2, 0);
        if (peer == NULL) {
            std::cerr << "[Cocorium SDK] No hay peers disponibles para conectar.\n";
            return false;
        }

        ENetEvent event;
        if (enet_host_service(client, &event, 500) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << "[Cocorium SDK] Conectado al servidor Cocorium.\n";
            return true;
        } else {
            enet_peer_reset(peer);
            std::cerr << "[Cocorium SDK] Conexión al servidor fallida.\n";
            return false;
        }
    }

    void PollEvents() {
        if (!client) return;

        ENetEvent event;
        while (enet_host_service(client, &event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                std::string data((char*)event.packet->data, event.packet->dataLength);
                
                try {
                    json j = json::parse(data);
                    if (j["type"] == "LOGIN_RESPONSE") {
                        lastLoginResult.success = j["success"];
                        lastLoginResult.message = j["message"];
                        hasLoginResponse = true;
                        
                        if (lastLoginResult.success) {
                            currentUser = j["username"];
                            loggedIn = true;
                            std::cout << "[Cocorium SDK] Login exitoso como: " << currentUser << "\n";
                        } else {
                            std::cout << "[Cocorium SDK] Error de login: " << lastLoginResult.message << "\n";
                        }
                    }
                } catch(std::exception& e) {
                    std::cerr << "[Cocorium SDK] Error parseando respuesta: " << e.what() << "\n";
                }
                
                enet_packet_destroy(event.packet);
            } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                std::cout << "[Cocorium SDK] Desconectado del servidor.\n";
                loggedIn = false;
                currentUser = "";
            }
        }
    }

    void Shutdown() {
        if (peer) {
            enet_peer_disconnect(peer, 0);
            ENetEvent event;
            while (enet_host_service(client, &event, 1000) > 0) {
                if (event.type == ENET_EVENT_TYPE_RECEIVE) enet_packet_destroy(event.packet);
                else if (event.type == ENET_EVENT_TYPE_DISCONNECT) break;
            }
        }
        if (client) enet_host_destroy(client);
        enet_deinitialize();
    }

    void RequestLogin(const std::string& username, const std::string& password) {
        if (!peer) return;
        
        hasLoginResponse = false;
        
        json j;
        j["type"] = "LOGIN_REQUEST";
        j["username"] = username;
        j["password"] = password;
        
        std::string payload = j.dump();
        ENetPacket* packet = enet_packet_create(payload.c_str(), payload.length() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }
    
    void Logout() {
        loggedIn = false;
        currentUser = "";
        Shutdown();
        Initialize("127.0.0.1", 7777);
    }
    
    void RequestRegister(const std::string& username, const std::string& password) {
        if (!peer) return;
        
        hasLoginResponse = false;
        
        json j;
        j["type"] = "REGISTER_REQUEST";
        j["username"] = username;
        j["password"] = password;
        
        std::string payload = j.dump();
        ENetPacket* packet = enet_packet_create(payload.c_str(), payload.length() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }

    bool HasLoginResponse() { return hasLoginResponse; }
    LoginResult GetLastLoginResult() { hasLoginResponse = false; return lastLoginResult; }

    std::string GetUsername() { return currentUser; }
    bool IsLoggedIn() { return loggedIn; }
}
