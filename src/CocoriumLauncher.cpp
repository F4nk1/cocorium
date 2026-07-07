#include "CocoriumLauncher.h"
#include "GameLibraryManager.h"
#include "UIManager.h"
#include <imgui.h>

CocoriumLauncher::CocoriumLauncher() {
    beyota_engine->Init();
    beyota_engine->log->OpenLogFile("cocorium_platform.log");
    beyota_engine->log->Message("--- COCORIUM PLATFORM LAUNCHED ---");
    
    beyota_engine->graphics->Init("Cocorium Platform", 1280, 720, BEYOTA_SCR_ADAPTIVE, false, true);
    beyota_engine->graphics->SetBackdropColor(230, 240, 255, 255);
    
    UIManager::GetInstance().InitTheme();
    GameLibraryManager::GetInstance().LoadLibrary("games.json");
}

CocoriumLauncher::~CocoriumLauncher() {
}

void CocoriumLauncher::Run() {
    while (!beyota_engine->system->quit) {
        beyota_engine->system->EventUpdate();
        beyota_engine->graphics->PrepareFrame();
        
        UIManager::GetInstance().Render();
        
        beyota_engine->graphics->Update();
    }
}
