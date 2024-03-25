//
// Created by lunah on 3/11/2024.
//

#ifndef TANKS_GAME_H
#define TANKS_GAME_H

#include "scene.h"
#include "gamewindow.h"
#include <QObject>
#include <QApplication>
#include <QTimer>
#include <QKeyEvent>
#include <iostream>

class Game : public QApplication{
    Q_OBJECT
private:
    // Private Variables
    GameWindow* gw;
    Scene* sc;
    QTimer timer;
    int activeKey;
    bool inGame;


    // Private Functions

    void pause();
    void resume();
    void end();

private slots:
    void tick();


public:
    // Public Functions
    Game(int argc, char** argv);
    ~Game() override;
    int start();

public slots:
    void filterKeyEvent(QKeyEvent *event);

signals:
    void playerControlSignal(QKeyEvent *event);
};

#endif //TANKS_GAME_H
