#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "renderer.h"
#include "mainmenu.h"
#include "pausemenu.h"
#include "gameover.h"
#include "levelmenu.h"

#include <QMainWindow>
#include <unordered_map>
#include <iostream>

#define GAME_KEY 0
#define MAIN_MENU_KEY 1
#define PAUSE_MENU_KEY 2
#define GAME_OVER_KEY 3
#define LEVEL_MENU_KEY 4
#define MULTIPLAYER_MENU_KEY 5 // unimplemented
#define HOST_MENU_KEY 6 // unimplemented
#define JOINIP_MENU_KEY 7 // unimplemented

class GameWindow : public QMainWindow {
    Q_OBJECT

private:
    // Private Variables
    Renderer *rend;
    MainMenu *mainMenu;
    PauseMenu *pauseMenu;
    GameOver *gameOver;
    LevelMenu* levelMenu;

    std::unordered_map<int, QWidget *> widgets;
    int activeKey;

public:
    // Public Functions
    explicit GameWindow(QObject *parent = nullptr, int startKey = MAIN_MENU_KEY);
    QWidget* changeWidget(int key);
    QWidget* getWidget(int key);
    void displayWidget();
    void hideWidget();

public slots:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
signals:
    void keySignal(QKeyEvent *event);
};



#endif // GAMEWINDOW_H
