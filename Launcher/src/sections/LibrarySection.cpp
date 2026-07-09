#include "LibrarySection.h"
#include "../GameLibraryManager.h"
#include <CocoriumSDK/Cocorium.h>
#include <imgui.h>

void LibrarySection::OnInit() {
    selected_game_index = 0;
}

void LibrarySection::OnUpdate() {
}

void LibrarySection::OnRender() {
    const auto& games = GameLibraryManager::GetInstance().GetGames();
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    
    ImGui::BeginChild("SidebarGames", ImVec2(viewport->WorkSize.x * 0.25f, 0), true);
    ImGui::Text("JUEGOS INSTALADOS");
    ImGui::Separator();
    
    for (size_t i = 0; i < games.size(); ++i) {
        if (ImGui::Selectable(games[i].title.c_str(), selected_game_index == (int)i)) {
            selected_game_index = static_cast<int>(i);
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
        draw_list->AddRectFilled(p, ImVec2(p.x + ImGui::GetContentRegionAvail().x, p.y + banner_h), IM_COL32(100, 100, 100, 255), 12.0f);
        ImGui::Dummy(ImVec2(0, banner_h + 10.0f));
        
        ImGui::Text("%s", selected.title.c_str());
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", selected.developer.c_str());
        ImGui::Spacing();
        ImGui::TextWrapped("%s", selected.description.c_str());
        ImGui::Spacing(); ImGui::Spacing();
        
        ImVec2 btn_size = ImVec2(200, 50);
        
        if (ImGui::Button("JUGAR AHORA", btn_size)) {
            GameLibraryManager::GetInstance().LaunchGame(selected_game_index);
        }
    } else {
        ImGui::Text("No hay juegos instalados o seleccionados.");
    }
    ImGui::EndChild();
}
