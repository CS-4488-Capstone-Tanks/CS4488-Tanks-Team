#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Renderer.h"
#include "qwindow.h"
#include "menumanager.h"
#include <QObject>
#include <unordered_map>
#include <QWidget>
#include "game.h"


class GameWindow : public QWindow {
    Q_OBJECT
// Default key values for mapping to QWidget Pointers
#define MAIN_MENU_KEY 0
#define RENDERER_KEY 1
#define INGAME_MENU_KEY 2
#define OPTIONS_MENU_KEY 3
#define MULTIPLAYER_MENU_KEY 4
#define HOST_MENU_KEY 5 // Unimplemented
#define JOINIP_MENU_KEY 6 // Unimplemented


protected:
    // Protected Variables
    QWindow win; // win is protected so that any child classes can access it, if need be.

private:
    // Private Variables
    MenuManager menuMan;
    Renderer renderer;
    std::unordered_map<int, QWidget *> widgetCache;
    int activeKey;

    // Private Functions
    void fetchAdjacent(QWidget *wpointer);
    void fetchByKey(int key);
    QWidget *cacheCheck(int key);
    QWidget *cacheMiss(int key);

public:
    // Public Functions
    GameWindow();
    void changeWidget(int key);
    void displayWidget();
    void hideWidget();



signals:
    Q_SIGNAL void callMenuMan(int key);

public slots:
    Q_SLOT void receiveMenu();

// Friends
friend class MenuManager;
friend class Game;
};



#endif // GAMEWINDOW_H
