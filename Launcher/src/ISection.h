#pragma once

class ISection {
public:
    virtual ~ISection() = default;
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual const char* GetName() const = 0;
};
