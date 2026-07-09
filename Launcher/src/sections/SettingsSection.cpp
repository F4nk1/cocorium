#include "SettingsSection.h"
#include "../ConfigManager.h"
#include "../ThemeManager.h"
#include <imgui.h>

void SettingsSection::OnInit() {}
void SettingsSection::OnUpdate() {}

void SettingsSection::OnRender() {
    auto& config = ConfigManager::GetInstance();
    
    ImGui::Text("Ajustes del Sistema");
    ImGui::Separator();
    ImGui::Spacing();
    
    bool changed = false;
    
    if (ImGui::Checkbox("Inicio de sesion automatico", &config.auto_login)) {
        changed = true;
    }
    
    if (ImGui::Checkbox("Modo Oscuro", &config.dark_mode)) {
        changed = true;
    }
    
    if (ImGui::ColorEdit3("Color de Acento", config.accent_color)) {
        changed = true;
    }
    
    if (ImGui::SliderFloat("Redondeo de Ventanas", &config.window_rounding, 0.0f, 24.0f)) {
        changed = true;
    }
    
    if (changed) {
        ThemeManager::ApplyTheme();
        config.Save();
    }
}
