#include "ThemeManager.h"
#include "ConfigManager.h"
#include <imgui.h>

void ThemeManager::ApplyTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    auto& config = ConfigManager::GetInstance();
    
    ImVec4 accent(config.accent_color[0], config.accent_color[1], config.accent_color[2], 1.0f);
    ImVec4 accent_hovered(config.accent_color[0] + 0.1f, config.accent_color[1] + 0.1f, config.accent_color[2] + 0.1f, 1.0f);
    ImVec4 accent_active(config.accent_color[0] - 0.1f, config.accent_color[1] - 0.1f, config.accent_color[2] - 0.1f, 1.0f);

    if (config.dark_mode) {
        style.Colors[ImGuiCol_Text]                  = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    } else {
        style.Colors[ImGuiCol_Text]                  = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.80f, 0.80f, 0.85f, 1.00f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.95f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.80f, 0.90f, 1.00f, 1.00f);
    }
    
    style.Colors[ImGuiCol_Button]                = accent;
    style.Colors[ImGuiCol_ButtonHovered]         = accent_hovered;
    style.Colors[ImGuiCol_ButtonActive]          = accent_active;
    style.Colors[ImGuiCol_CheckMark]             = accent;
    style.Colors[ImGuiCol_Header]                = ImVec4(accent.x, accent.y, accent.z, 0.3f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(accent.x, accent.y, accent.z, 0.5f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(accent.x, accent.y, accent.z, 0.8f);
    
    style.WindowRounding    = config.window_rounding;
    style.ChildRounding     = config.window_rounding - 4.0f;
    style.FrameRounding     = config.window_rounding - 6.0f;
    if (style.ChildRounding < 0.0f) style.ChildRounding = 0.0f;
    if (style.FrameRounding < 0.0f) style.FrameRounding = 0.0f;
}
