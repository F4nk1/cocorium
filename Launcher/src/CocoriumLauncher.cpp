#include "CocoriumLauncher.h"
#include "GameLibraryManager.h"
#include "UIManager.h"
#include "ConfigManager.h"
#include "ThemeManager.h"
#include "SectionManager.h"
#include "sections/HomeSection.h"
#include "sections/LibrarySection.h"
#include "sections/SettingsSection.h"
#include <CocoriumSDK/Cocorium.h>
#include <imgui.h>
#include <filesystem>
#include <SDL.h>
#include "fonts/IconsFontAwesome6.h"

Cocorium::Client netClient;

CocoriumLauncher::CocoriumLauncher() {
    beyota_engine->Init();
    beyota_engine->log->OpenLogFile("cocorium_platform.log");
    beyota_engine->log->Message("--- COCORIUM PLATFORM LAUNCHED ---");
    
    // Iniciar SDK (conexión de red)
    if (!netClient.Initialize("127.0.0.1", 7777)) {
        beyota_engine->log->Message("[Warning] No se pudo contactar al servidor en 127.0.0.1:7777");
    }
    
    netClient.SetAuthCallback([](bool success, Cocorium::ErrorCode code, const std::string& message, const std::string& username) {
        UIManager::GetInstance().HandleAuthResponse(success, code, message, username);
    });
    
    beyota_engine->graphics->Init("Cocorium Platform", 1280, 720, BEYOTA_SCR_ADAPTIVE, false, true);
    beyota_engine->graphics->SetBackdropColor(230, 240, 255, 255);
    
    // ConfigManager y ThemeManager
    auto& config = ConfigManager::GetInstance();
    config.Load("config.json");
    
    // Cargar fuente FontAwesome (ImGui context already initialized by beyota_engine->graphics->Init)
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    
    ImFontConfig font_config;
    font_config.MergeMode = true;
    font_config.PixelSnapH = true;
    font_config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    io.Fonts->AddFontFromFileTTF("Launcher/src/fonts/fa-solid-900.ttf", 16.0f, &font_config, icon_ranges);
    // Necesitamos reconstruir la textura de la fuente, pero el motor (BeyotaEngine) maneja esto internamente 
    // en su primer frame o requiere un re-upload? Si ImGui-Impl lo hace OnNewFrame, estará bien.

    UIManager::GetInstance().InitTheme();
    
    // Inicializar Secciones
    auto& sectionManager = SectionManager::GetInstance();
    sectionManager.RegisterSection(std::make_unique<HomeSection>());
    sectionManager.RegisterSection(std::make_unique<LibrarySection>());
    sectionManager.RegisterSection(std::make_unique<SettingsSection>());
    
    sectionManager.SetActiveSection(config.default_section);
    
    std::string exe_dir = ".";
    try {
        exe_dir = std::filesystem::path(std::filesystem::canonical("/proc/self/exe")).parent_path().string();
    } catch (...) {}
    
    GameLibraryManager::GetInstance().LoadLibrary(exe_dir + "/games.json");
    
    // Iniciar captura de texto para ImGui
    SDL_StartTextInput();
}

CocoriumLauncher::~CocoriumLauncher() {
    SDL_StopTextInput();
    ConfigManager::GetInstance().Save();
    netClient.Shutdown();
}

void CocoriumLauncher::Run() {
    while (!beyota_engine->system->quit) {
        beyota_engine->system->EventUpdate();
        
        // Procesar mensajes de red del Servidor
        netClient.PollEvents();
        
        UIManager::GetInstance().Render();
        
        beyota_engine->graphics->Update();
    }
}
