#pragma once
#include "../ISection.h"
#include <string>

class SettingsSection : public ISection {
public:
    void OnInit() override;
    void OnUpdate() override;
    void OnRender() override;
    const char* GetName() const override { return "Ajustes"; }
};
