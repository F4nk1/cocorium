#pragma once
#include "../ISection.h"

class HomeSection : public ISection {
public:
    void OnInit() override;
    void OnUpdate() override;
    void OnRender() override;
    const char* GetName() const override { return "Inicio"; }
};
