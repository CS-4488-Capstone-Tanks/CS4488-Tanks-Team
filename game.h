//
// Created by lunah on 3/11/2024.
//

#ifndef TANKS_GAME_H
#define TANKS_GAME_H

#include "scene.h"
#include "GameWindow.h"
#include <QTimer>
#include <QApplication>

class Game : public QApplication {
    GameWindow* window;
    Scene* scene;
    QTimer timer;

private slots:
    void tick();

public:
    Game(int argc, char** argv);
    ~Game();

    int start();
};

#endif //TANKS_GAME_H
