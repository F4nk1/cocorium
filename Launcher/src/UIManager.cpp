#include "UIManager.h"
#include "GameLibraryManager.h"
#include "ConfigManager.h"
#include "ThemeManager.h"
#include "SectionManager.h"
#include <CocoriumSDK/Cocorium.h>
#include <imgui.h>
#include <imgui_internal.h>
#include "fonts/IconsFontAwesome6.h"
#include <BeyotaEngine.h>

extern Cocorium::Client netClient;

void UIManager::HandleAuthResponse(bool success, Cocorium::ErrorCode code, const std::string& message, const std::string& username) {
    login_pending = false;
    if (success) {
        current_state = AppState::MainUI;
        login_error_msg = "";
        auto& config = ConfigManager::GetInstance();
        if (config.auto_login) {
            config.saved_username = input_username;
            config.saved_password = input_password;
            config.Save();
        }
    } else {
        login_error_msg = message;
    }
}

void UIManager::InitTheme() {
    ThemeManager::ApplyTheme();
}

void UIManager::Render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    
    ImGui::Begin("Cocorium OS", nullptr, window_flags);
    
    if (current_state == AppState::Login) {
        RenderLoginScreen();
    } else if (current_state == AppState::Register) {
        RenderRegisterScreen();
    } else if (current_state == AppState::MainUI) {
        RenderMainUI();
    }
    
    ImGui::End();
}

void UIManager::RenderLoginScreen() {
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 panel_size = ImVec2(400, 420);
    
    if (attempt_auto_login) {
        attempt_auto_login = false;
        auto& config = ConfigManager::GetInstance();
        if (config.auto_login && !config.saved_username.empty() && !config.saved_password.empty()) {
            strncpy(input_username, config.saved_username.c_str(), sizeof(input_username) - 1);
            strncpy(input_password, config.saved_password.c_str(), sizeof(input_password) - 1);
            netClient.RequestLogin(config.saved_username, config.saved_password);
            login_pending = true;
            login_error_msg = "";
        }
    }
    
    ImGui::SetCursorPos(ImVec2((window_size.x - panel_size.x) * 0.5f, (window_size.y - panel_size.y) * 0.5f));
    ImGui::BeginChild("LoginPanel", panel_size, true, ImGuiWindowFlags_NoScrollbar);
    
    ImGui::Spacing(); ImGui::Spacing();
    ImVec2 text_size = ImGui::CalcTextSize("C O C O R I U M");
    ImGui::SetCursorPosX((panel_size.x - text_size.x) * 0.5f);
    ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "C O C O R I U M");
    
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing();
    
    ImGui::Text("Nombre de Usuario:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##username", input_username, IM_ARRAYSIZE(input_username));
    
    ImGui::Spacing();
    
    ImGui::Text("Contraseña:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##password", input_password, IM_ARRAYSIZE(input_password), ImGuiInputTextFlags_Password);
    
    ImGui::Spacing(); ImGui::Spacing();
    
    if (!login_error_msg.empty()) {
        ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "%s", login_error_msg.c_str());
    } else {
        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
    }
    
    ImGui::Spacing(); ImGui::Spacing();
    
    if (login_pending) {
        ImGui::SetCursorPosX((panel_size.x - ImGui::CalcTextSize("Conectando...").x) * 0.5f);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Conectando...");
    } else {
        ImVec2 btn_size = ImVec2(200, 40);
        ImGui::SetCursorPosX((panel_size.x - btn_size.x) * 0.5f);
        if (ImGui::Button("INICIAR SESION", btn_size)) {
            std::string user(input_username);
            std::string pass(input_password);
            if (!user.empty() && !pass.empty()) {
                netClient.RequestLogin(user, pass);
                login_pending = true;
                login_error_msg = "";
            } else {
                login_error_msg = "Por favor, llena ambos campos.";
            }
        }
        
        ImGui::Spacing(); ImGui::Spacing();
        ImGui::SetCursorPosX((panel_size.x - ImGui::CalcTextSize("¿No tienes cuenta? Regístrate").x) * 0.5f);
        if (ImGui::Selectable("¿No tienes cuenta? Regístrate", false, 0, ImGui::CalcTextSize("¿No tienes cuenta? Regístrate"))) {
            current_state = AppState::Register;
            login_error_msg = "";
        }
    }
    
    ImGui::EndChild();
}

void UIManager::RenderRegisterScreen() {
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 panel_size = ImVec2(400, 420);
    
    ImGui::SetCursorPos(ImVec2((window_size.x - panel_size.x) * 0.5f, (window_size.y - panel_size.y) * 0.5f));
    ImGui::BeginChild("RegisterPanel", panel_size, true, ImGuiWindowFlags_NoScrollbar);
    
    ImGui::Spacing(); ImGui::Spacing();
    ImVec2 text_size = ImGui::CalcTextSize("REGISTRO DE CUENTA");
    ImGui::SetCursorPosX((panel_size.x - text_size.x) * 0.5f);
    ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "REGISTRO DE CUENTA");
    
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing();
    
    ImGui::Text("Elige un Nombre de Usuario:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##reg_username", input_username, IM_ARRAYSIZE(input_username));
    
    ImGui::Spacing();
    
    ImGui::Text("Crea una Contraseña:");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::InputText("##reg_password", input_password, IM_ARRAYSIZE(input_password), ImGuiInputTextFlags_Password);
    
    ImGui::Spacing(); ImGui::Spacing();
    
    if (!login_error_msg.empty()) {
        ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 1.0f), "%s", login_error_msg.c_str());
    } else {
        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
    }
    
    ImGui::Spacing(); ImGui::Spacing();
    
    if (login_pending) {
        ImGui::SetCursorPosX((panel_size.x - ImGui::CalcTextSize("Conectando...").x) * 0.5f);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Conectando...");
    } else {
        ImVec2 btn_size = ImVec2(200, 40);
        ImGui::SetCursorPosX((panel_size.x - btn_size.x) * 0.5f);
        if (ImGui::Button("CREAR CUENTA", btn_size)) {
            std::string user(input_username);
            std::string pass(input_password);
            if (!user.empty() && !pass.empty()) {
                netClient.RequestRegister(user, pass);
                login_pending = true;
                login_error_msg = "";
            } else {
                login_error_msg = "Por favor, llena ambos campos.";
            }
        }
        
        ImGui::Spacing(); ImGui::Spacing();
        ImGui::SetCursorPosX((panel_size.x - ImGui::CalcTextSize("Volver al Login").x) * 0.5f);
        if (ImGui::Selectable("Volver al Login", false, 0, ImGui::CalcTextSize("Volver al Login"))) {
            current_state = AppState::Login;
            login_error_msg = "";
        }
    }
    
    ImGui::EndChild();
}

void UIManager::RenderMainUI() {
    if (!netClient.IsLoggedIn()) {
        current_state = AppState::Login;
        return;
    }
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    
    // Sidebar
    ImGui::BeginChild("GlobalSidebar", ImVec2(80, 0), true);
    
    ImGui::Spacing();
    
    auto& sm = SectionManager::GetInstance();
    ISection* active = sm.GetActiveSection();
    
    auto drawSidebarIcon = [&](const char* icon, const char* sectionName) {
        bool is_active = active && std::string(active->GetName()) == sectionName;
        if (is_active) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
        }
        
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.Size > 1 ? ImGui::GetIO().Fonts->Fonts[1] : ImGui::GetIO().Fonts->Fonts[0]);
        if (ImGui::Button(icon, ImVec2(60, 60))) {
            sm.SetActiveSection(sectionName);
        }
        ImGui::PopFont();
        
        ImGui::PopStyleColor();
        ImGui::Spacing();
    };

    drawSidebarIcon(ICON_FA_HOUSE, "Inicio");
    drawSidebarIcon(ICON_FA_GAMEPAD, "Librería");
    drawSidebarIcon(ICON_FA_GEAR, "Ajustes");
    
    // Bottom aligned buttons (Logout / Quit)
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 140.0f);
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.Size > 1 ? ImGui::GetIO().Fonts->Fonts[1] : ImGui::GetIO().Fonts->Fonts[0]);
    
    if (ImGui::Button(ICON_FA_RIGHT_FROM_BRACKET, ImVec2(60, 60))) {
        // Logout
        auto& config = ConfigManager::GetInstance();
        config.auto_login = false;
        config.saved_username = "";
        config.saved_password = "";
        config.Save();
        netClient.Logout();
        attempt_auto_login = false; // Prevent immediate re-login
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Cerrar Sesión");
    
    if (ImGui::Button(ICON_FA_POWER_OFF, ImVec2(60, 60))) {
        // Quit
        beyota_engine->system->quit = true;
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Salir");
    
    ImGui::PopFont();
    ImGui::PopStyleColor();
    
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // Main Content
    ImGui::BeginChild("MainContent", ImVec2(0, 0), false);
    sm.Update();
    sm.Render();
    ImGui::EndChild();
}
