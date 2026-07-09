#pragma once

#include <string>

namespace Cocorium {

    struct LoginResult {
        bool success;
        std::string message;
    };

    // Inicializa la conexión local con el servidor/launcher
    bool Initialize(const std::string& host = "127.0.0.1", int port = 7777);
    
    // Procesa eventos de red (debe llamarse en el game loop)
    void PollEvents();

    // Cierra la conexión
    void Shutdown();

    // Intenta iniciar sesión
    void RequestLogin(const std::string& username, const std::string& password);
    void Logout();

    // Intenta registrar una nueva cuenta
    void RequestRegister(const std::string& username, const std::string& password);

    // Obtiene el estado del último intento de login
    bool HasLoginResponse();
    LoginResult GetLastLoginResult();

    std::string GetUsername();
    bool IsLoggedIn();
}
