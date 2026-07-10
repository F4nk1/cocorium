#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <string>

enum class AppState {
    Login,
    Register,
    MainUI
};

class UIManager {
public:
    static UIManager& GetInstance() {
        static UIManager instance;
        return instance;
    }

    void InitTheme();
    void Render();
    
    void HandleAuthResponse(bool success, const std::string& message, const std::string& username);

private:
    UIManager() = default;
    
    void RenderLoginScreen();
    void RenderRegisterScreen();
    void RenderMainUI();

    AppState current_state = AppState::Login;
    
    bool attempt_auto_login = true;
    
    char input_username[64] = "";
    char input_password[64] = "";
    bool login_pending = false;
    std::string login_error_msg = "";
};

#endif
