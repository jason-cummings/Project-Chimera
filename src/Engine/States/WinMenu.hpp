#ifndef WINMENU_HPP
#define WINMENU_HPP

#include "Menu.hpp"

class InGameState;

class WinMenu : public Menu {
  private:
    void handleButtonEvent(MenuButton *clicked) override;
    GameState *last_state;

  public:
    WinMenu(InGameState *input);
    ~WinMenu();

    inline bool init() override {
        is_initialized = true;
        return true;
    }
};

#endif