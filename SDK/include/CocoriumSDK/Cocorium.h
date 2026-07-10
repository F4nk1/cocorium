#pragma once
#include <string>
#include <functional>
#include <enet/enet.h>
#include <nlohmann/json.hpp>
#include "ErrorCodes.h"

namespace Cocorium {

    class Client {
    public:
        // Definimos el tipo de callback para las respuestas del servidor
        using AuthCallback = std::function<void(bool success, Cocorium::ErrorCode code, const std::string& message, const std::string& username)>;

        Client();
        ~Client();

        // Gestión de conexión
        bool Initialize(const std::string& host, int port);
        void PollEvents(); // Esto se llamará en el loop principal del Launcher
        void Shutdown();

        // Acciones
        void RequestLogin(const std::string& username, const std::string& password);
        void RequestRegister(const std::string& username, const std::string& password);
        void Logout();

        // Asignación de Callbacks (El Launcher inyectará sus funciones aquí)
        void SetAuthCallback(AuthCallback callback);

        // Getters de estado
        bool IsLoggedIn() const;
        std::string GetUsername() const;

    private:
        ENetHost* host;
        ENetPeer* peer;
        
        bool loggedIn;
        std::string currentUser;
        
        AuthCallback onAuthResponse;

        // Métodos internos
        void HandleReceive(ENetPacket* packet);
        void HandleDisconnect();
        void SendJSON(const nlohmann::json& j);
    };

}