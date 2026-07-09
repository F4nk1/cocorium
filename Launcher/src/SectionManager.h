#pragma once
#include "ISection.h"
#include <vector>
#include <memory>
#include <string>

class SectionManager {
public:
    static SectionManager& GetInstance() {
        static SectionManager instance;
        return instance;
    }

    void RegisterSection(std::unique_ptr<ISection> section);
    void SetActiveSection(const std::string& name);
    void Update();
    void Render();
    
    const std::vector<std::unique_ptr<ISection>>& GetSections() const { return sections; }
    ISection* GetActiveSection() const;

private:
    SectionManager() = default;
    std::vector<std::unique_ptr<ISection>> sections;
    int current_section_index = -1;
};
