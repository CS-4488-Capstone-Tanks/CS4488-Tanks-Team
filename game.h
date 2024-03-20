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

class Game : public QObject{
    Q_OBJECT
private:
    // Private Variables
    GameWindow gw;
    Scene sc;
    QApplication* app;
    QTimer* timer;
    bool inGame;

public:
    // Public Functions
    Game(QApplication *app = nullptr);

signals:
    Q_SIGNAL void switchWidget(int key); // Switch the active QWidget
    Q_SIGNAL void update(); // Update the game
    Q_SIGNAL void setPaused(bool ispaused); // Set the game to paused
    Q_SIGNAL void timerStart(); // Signal the QTimer to start
    Q_SIGNAL void timerStop(); // Signal the QTimer to stop

public slots:
    Q_SLOT void start(); // Start the game
    Q_SLOT void keyPress(int key); // Handle key presses
    Q_SLOT void timerHandle(); // Handle the QTimer signal

// Friends
friend class Scene;
friend class GameWindow;
};

#endif //TANKS_GAME_H
