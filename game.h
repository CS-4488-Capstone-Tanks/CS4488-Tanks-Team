//
// Created by lunah on 3/11/2024.
//

#ifndef TANKS_GAME_H
#define TANKS_GAME_H

#include "Scene.h"
#include "gamewindow.h"
#include <QApplication>
#include <QTimer>
#include <format>
#include <iostream>

class Game : public QApplication{
private:
    // Private Variables
    GameWindow* gw;
    Scene* sc;
    QTimer timer;
    bool inGame;

private slots:
    void tick();

public:
    // Public Functions
    Game(int argc, char** argv);
    ~Game() override;

// Private Functions
int start();
};

#endif //TANKS_GAME_H
