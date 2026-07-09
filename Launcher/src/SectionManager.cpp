#include "SectionManager.h"

void SectionManager::RegisterSection(std::unique_ptr<ISection> section) {
    section->OnInit();
    sections.push_back(std::move(section));
    if (current_section_index == -1) {
        current_section_index = 0;
    }
}

void SectionManager::SetActiveSection(const std::string& name) {
    for (size_t i = 0; i < sections.size(); ++i) {
        if (name == sections[i]->GetName()) {
            current_section_index = static_cast<int>(i);
            break;
        }
    }
}

void SectionManager::Update() {
    if (current_section_index >= 0 && current_section_index < static_cast<int>(sections.size())) {
        sections[current_section_index]->OnUpdate();
    }
}

void SectionManager::Render() {
    if (current_section_index >= 0 && current_section_index < static_cast<int>(sections.size())) {
        sections[current_section_index]->OnRender();
    }
}

ISection* SectionManager::GetActiveSection() const {
    if (current_section_index >= 0 && current_section_index < static_cast<int>(sections.size())) {
        return sections[current_section_index].get();
    }
    return nullptr;
}
