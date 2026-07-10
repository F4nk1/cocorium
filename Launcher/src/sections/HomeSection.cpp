#include "HomeSection.h"
#include <imgui.h>
#include <CocoriumSDK/Cocorium.h>

extern Cocorium::Client netClient;

void HomeSection::OnInit() {}
void HomeSection::OnUpdate() {}

void HomeSection::OnRender() {
    ImGui::Text("Bienvenido de nuevo%s", netClient.IsLoggedIn() ? (", " + netClient.GetUsername()).c_str() : "");
    ImGui::Separator();
    
    ImGui::Spacing(); ImGui::Spacing();
    
    ImGui::TextWrapped("Explora tu libreria, o configura la plataforma desde la barra lateral. Cocorium OS te permite personalizar los colores, usar el modo oscuro, y modificar el redondeo de la interfaz a tu gusto.");
    
    ImGui::Spacing(); ImGui::Spacing();
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    
    float banner_w = ImGui::GetContentRegionAvail().x;
    float banner_h = 300.0f;
    draw_list->AddRectFilled(p, ImVec2(p.x + banner_w, p.y + banner_h), IM_COL32(50, 150, 250, 150), 12.0f);
    
    ImGui::SetCursorScreenPos(ImVec2(p.x + 20, p.y + 20));
    ImGui::TextColored(ImVec4(1,1,1,1), "Noticias destacadas");
    ImGui::SetCursorScreenPos(ImVec2(p.x + 20, p.y + 50));
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1), "¡Cocorium 2.0 ya está aquí con nuevas características!");
}
