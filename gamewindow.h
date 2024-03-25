#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "renderer.h"
#include "mainmenu.h"
#include "ingamemenu.h"
#include "optionsmenu.h"
#include <QMainWindow>
#include <unordered_map>
#include <iostream>

#define GAME_KEY 0
#define MAIN_MENU_KEY 1
#define INGAME_MENU_KEY 2
#define OPTIONS_MENU_KEY 3
#define MULTIPLAYER_MENU_KEY 4 // unimplemented
#define HOST_MENU_KEY 5 // unimplemented
#define JOINIP_MENU_KEY 6 // unimplemented

class GameWindow : public QMainWindow {
    Q_OBJECT

private:
    // Private Variables
    Renderer *rend;
    MainMenu *mainMenu;
    InGameMenu *inGameMenu;
    OptionsMenu *optionsMenu;

    std::unordered_map<int, QWidget *> widgets;
    int activeKey;

public:
    // Public Functions
    GameWindow();
    QWidget* changeWidget(int key);
    void displayWidget();
    void hideWidget();
};



#endif // GAMEWINDOW_H
