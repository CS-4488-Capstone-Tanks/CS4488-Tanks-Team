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
    QTimer timer;
    int activeKey;
    bool inGame;
    bool isAlive;


    // Private Functions

    void pause();
    void resume();
    void end();

    Game(int argc, char** argv);
    ~Game() override;

    static Game* instance;

private slots:
    void tick();


public:
    /**
     * Fetch or construct the game's instance. If you're calling this from main, you
     * should pass in argc/argv. Otherwise, they'll be ignored
     * @param argc the argument count from main
     * @param argv the argument vector from main
     * @return the game instance
     * @author Tyson Cox
     * @date SPRING 2024
     */
    static Game* getInstance(int argc = 0, char** argv = nullptr);

    /** Should only be called when the game is exiting entirely, just before main's return */
    static void destroyInstance();

    // Public Functions
    int start();
    void gameOver();
    void wonGame();

    void beginNewScene(std::string stateFilename);

    GameWindow* getWindow();

public slots:
    bool filterKeyEvent(QKeyEvent* event);
};

#endif //TANKS_GAME_H
