#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "renderer.h"
#include <QObject>
#include <QWidget>
#include <vector>

#include "mainmenu.h"
#include "ingamemenu.h"
#include "optionsmenu.h"
// #include "multiplayermenu.h"   unimplemented
// #include "hostmenu.h"   unimplemented
// #include "joinipmenu.h"   unimplemented

#define GAME_KEY 0
#define MAIN_MENU_KEY 1
#define INGAME_MENU_KEY 2
#define OPTIONS_MENU_KEY 3
// #define MULTIPLAYER_MENU_KEY 4   unimplemented
// #define HOST_MENU_KEY 5   unimplemented
// #define JOINIP_MENU_KEY 6   unimplemented

class MenuManager : public QObject {
Q_OBJECT
public:
    // Public functions
    // Constructor/destructor
    MenuManager();
    ~MenuManager() override;

protected:
    // Protected variables
    QWidget* active_widget;
    std::vector<QWidget*> QW_Vec;
    mainmenu *mainMenu;
    Renderer *renderer;
    ingamemenu *inGameMenu;
    optionsmenu *optionsMenu;
    // QWidget multiplayerMenu;  unimplemented
    // QWidget hostMenu;   unimplemented
    // QWidget joinIPMenu;   unimplemented

    // Protected functions
    void setActiveMenu(QWidget* menu);
    bool checkActiveMenu(QWidget* menu);

public slots:
    QWidget* fetch_menu(int key);
};


#endif // MENUMANAGER_H
