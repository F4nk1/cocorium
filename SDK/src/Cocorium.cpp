#include "CocoriumSDK/Cocorium.h"
#include <iostream>

using json = nlohmann::json;

namespace Cocorium {

    Client::Client() 
        : host(nullptr), peer(nullptr), loggedIn(false), currentUser(""), onAuthResponse(nullptr) {}

    Client::~Client() {
        Shutdown();
    }

    bool Client::Initialize(const std::string& serverHost, int port) {
        if (enet_initialize() != 0) {
            std::cerr << "[Cocorium SDK] Error inicializando ENet.\n";
            return false;
        }

        host = enet_host_create(NULL, 1, 2, 0, 0);
        if (host == NULL) {
            std::cerr << "[Cocorium SDK] Error creando el host ENet.\n";
            enet_deinitialize();
            return false;
        }

        ENetAddress address;
        enet_address_set_host(&address, serverHost.c_str());
        address.port = port;

        peer = enet_host_connect(host, &address, 2, 0);
        if (peer == NULL) {
            std::cerr << "[Cocorium SDK] No hay peers disponibles para conectar.\n";
            enet_host_destroy(host);
            host = nullptr;
            enet_deinitialize();
            return false;
        }

        ENetEvent event;
        // Esperar hasta 500ms para confirmar la conexión inicial
        if (enet_host_service(host, &event, 500) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << "[Cocorium SDK] Conectado exitosamente al servidor.\n";
            return true;
        } else {
            std::cerr << "[Cocorium SDK] Conexión al servidor fallida (Timeout).\n";
            enet_peer_reset(peer);
            enet_host_destroy(host);
            host = nullptr;
            peer = nullptr;
            enet_deinitialize();
            return false;
        }
    }

    void Client::PollEvents() {
        if (!host) return;

        ENetEvent event;
        // Loop no bloqueante (timeout = 0) para procesar ráfagas de paquetes
        while (enet_host_service(host, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    HandleReceive(event.packet);
                    enet_packet_destroy(event.packet);
                    break;
                    
                case ENET_EVENT_TYPE_DISCONNECT:
                    HandleDisconnect();
                    break;
                    
                default:
                    break;
            }
        }
    }

    void Client::HandleReceive(ENetPacket* packet) {
        std::string data(reinterpret_cast<char*>(packet->data), packet->dataLength);
        
        try {
            json j = json::parse(data);
            std::string type = j.value("type", "");

            if (type == "LOGIN_RESPONSE" || type == "REGISTER_RESPONSE") {
                bool success = j.value("success", false);
                std::string message = j.value("message", "");

                if (success && type == "LOGIN_RESPONSE") {
                    currentUser = j.value("username", "");
                    loggedIn = true;
                }

                // Si el Launcher registró un callback, lo invocamos de inmediato
                if (onAuthResponse) {
                    onAuthResponse(success, message, j.value("username", ""));
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[Cocorium SDK] Error parseando JSON de respuesta: " << e.what() << "\n";
        }
    }

    void Client::HandleDisconnect() {
        std::cout << "[Cocorium SDK] El servidor cerró la conexión.\n";
        loggedIn = false;
        currentUser = "";
        if (peer) {
            enet_peer_reset(peer);
            peer = nullptr;
        }
    }

    void Client::RequestLogin(const std::string& username, const std::string& password) {
        if (!peer) return;

        json j = {
            {"type", "LOGIN_REQUEST"},
            {"username", username},
            {"password", password} // Nota de seguridad: El Launcher debería enviar esto hasheado
        };
        SendJSON(j);
    }

    void Client::RequestRegister(const std::string& username, const std::string& password) {
        if (!peer) return;

        json j = {
            {"type", "REGISTER_REQUEST"},
            {"username", username},
            {"password", password}
        };
        SendJSON(j);
    }

    void Client::SendJSON(const json& j) {
        std::string payload = j.dump();
        // payload.length() + 1 incluye el terminador nulo para cadenas en C en el otro extremo si es necesario
        ENetPacket* packet = enet_packet_create(payload.c_str(), payload.length() + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }

    void Client::Logout() {
        loggedIn = false;
        currentUser = "";
        Shutdown();
    }

    void Client::Shutdown() {
        if (peer) {
            enet_peer_disconnect(peer, 0);
            
            // Vaciar pacientemente los eventos restantes para notificar la desconexión limpia
            ENetEvent event;
            while (enet_host_service(host, &event, 500) > 0) {
                if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                    enet_packet_destroy(event.packet);
                } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                    break;
                }
            }
            peer = nullptr;
        }

        if (host) {
            enet_host_destroy(host);
            host = nullptr;
        }
        
        enet_deinitialize();
        std::cout << "[Cocorium SDK] Recursos de red liberados.\n";
    }

    void Client::SetAuthCallback(AuthCallback callback) {
        onAuthResponse = callback;
    }

    bool Client::IsLoggedIn() const { return loggedIn; }
    std::string Client::GetUsername() const { return currentUser; }
}