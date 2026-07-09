#pragma once
#include "../ISection.h"
#include <string>

class LibrarySection : public ISection {
public:
    void OnInit() override;
    void OnUpdate() override;
    void OnRender() override;
    const char* GetName() const override { return "Librería"; }

private:
    int selected_game_index = 0;
};
