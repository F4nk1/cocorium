#ifndef UI_MANAGER_H
#define UI_MANAGER_H

class UIManager {
public:
    static UIManager& GetInstance() {
        static UIManager instance;
        return instance;
    }

    void InitTheme();
    void Render();

private:
    UIManager() = default;
    int selected_game_index = 0;
};

#endif
