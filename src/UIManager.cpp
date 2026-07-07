#include "UIManager.h"
#include "GameLibraryManager.h"
#include <imgui.h>

void UIManager::InitTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.Colors[ImGuiCol_Text]                  = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.80f, 0.80f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.95f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.80f, 0.90f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.85f, 0.85f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.98f, 0.50f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.99f, 0.60f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.90f, 0.40f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.80f, 0.90f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.70f, 0.85f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.60f, 0.80f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.98f, 0.50f, 0.45f, 1.00f);
    
    style.WindowRounding    = 12.0f;
    style.ChildRounding     = 8.0f;
    style.FrameRounding     = 6.0f;
    style.PopupRounding     = 8.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding      = 6.0f;
    style.WindowBorderSize  = 1.0f;
    style.FrameBorderSize   = 1.0f;
    style.ItemSpacing       = ImVec2(10, 10);
    style.WindowPadding     = ImVec2(16, 16);
}

void UIManager::Render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    
    ImGui::Begin("Biblioteca Cocorium", nullptr, window_flags);
    
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "C O C O R I U M   O S");
    ImGui::Separator();
    ImGui::Spacing(); ImGui::Spacing();
    
    const auto& games = GameLibraryManager::GetInstance().GetGames();
    
    ImGui::BeginChild("Sidebar", ImVec2(viewport->WorkSize.x * 0.25f, 0), true);
    ImGui::Text("JUEGOS INSTALADOS");
    ImGui::Separator();
    
    for (size_t i = 0; i < games.size(); ++i) {
        if (ImGui::Selectable(games[i].title.c_str(), selected_game_index == (int)i)) {
            selected_game_index = i;
        }
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginChild("Detalles", ImVec2(0, 0), true);
    if (!games.empty() && selected_game_index >= 0 && selected_game_index < (int)games.size()) {
        const auto& selected = games[selected_game_index];
        
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        float banner_h = 200.0f;
        draw_list->AddRectFilled(p, ImVec2(p.x + ImGui::GetContentRegionAvail().x, p.y + banner_h), IM_COL32(250, 128, 114, 255), 12.0f);
        ImGui::Dummy(ImVec2(0, banner_h + 10.0f));
        
        ImGui::Text("%s", selected.title.c_str());
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", selected.developer.c_str());
        ImGui::Spacing();
        ImGui::TextWrapped("%s", selected.description.c_str());
        ImGui::Spacing(); ImGui::Spacing();
        
        ImVec2 btn_pos = ImGui::GetCursorScreenPos();
        ImVec2 btn_size = ImVec2(200, 50);
        
        bool is_hovered_rect = ImGui::IsMouseHoveringRect(btn_pos, ImVec2(btn_pos.x + btn_size.x, btn_pos.y + btn_size.y));
        bool is_clicked = is_hovered_rect && ImGui::IsMouseClicked(0);
        
        ImU32 btn_color = is_hovered_rect ? IM_COL32(252, 153, 140, 255) : IM_COL32(250, 128, 114, 255);
        if (is_hovered_rect && ImGui::IsMouseDown(0)) btn_color = IM_COL32(230, 102, 89, 255);
        
        ImGui::GetWindowDrawList()->AddRectFilled(btn_pos, ImVec2(btn_pos.x + btn_size.x, btn_pos.y + btn_size.y), btn_color, 8.0f);
        ImVec2 text_size = ImGui::CalcTextSize("JUGAR AHORA");
        ImGui::GetWindowDrawList()->AddText(ImVec2(btn_pos.x + (btn_size.x - text_size.x)*0.5f, btn_pos.y + (btn_size.y - text_size.y)*0.5f), IM_COL32(255,255,255,255), "JUGAR AHORA");
        
        ImGui::InvisibleButton("JUGAR_AHORA_BTN", btn_size);
        
        if (is_clicked) {
            GameLibraryManager::GetInstance().LaunchGame(selected_game_index);
        }
    } else {
        ImGui::Text("No hay juegos instalados o seleccionados.");
    }
    ImGui::EndChild();
    
    ImGui::End();
}
