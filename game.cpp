// Created by Luna Steed and Tyson Cox 03/2024

#include "game.h"

/**
 * @author Tyson Cox, Luna Steed
 * @time Spring 2024
 * @brief Game::Game constructor
 * @details Constructor for the Game class. Initializes the GameWindow and Scene objects, and sets inGame to false.
 */
Game::Game(int argc, char** argv) : QApplication(argc, argv), timer(new QTimer(this)) {
    gw = new GameWindow();
    gw->show();
    const char stateFilename[] = "test_game_state";
    sc = new Scene(60.0f/1000, const_cast<char *>(stateFilename));
    inGame = false;
}

/**
 * @author Tyson Cox, Luna Steed
 * @time Spring 2024
 * @brief Game::~Game destructor
 * @details Destructor for the Game class. Deletes the GameWindow and Scene objects.
 */
Game::~Game() {
    delete gw;
    delete sc;
}

/**
 * @author Tyson Cox, Luna Steed
 * @time Spring 2024
 * @brief Game::start: Start the game
 * @details Start the game when prompted by the signal. Changes the active widget to the renderer and sets inGame to true.
 */
int Game::start() {
    sc->start();

    connect(&timer, &QTimer::timeout, this, &Game::tick);

    timer.setInterval(16); // 16 msec, roughly 60 fps
    timer.start();

    inGame = true;

    gw->show();

    return Game::exec();
}

// Q Slots: tick();

/**
 * @author Tyson Cox, Luna Steed
 * @time Spring 2024
 * @brief Game::tick: Update the game
 * @details Update the game by calling the Scene's update method.
 */
void Game::tick() {
    sc->update();

    QWidget* widg = gw->changeWidget(GAME_KEY);
    auto* rend = dynamic_cast<Renderer*>(widg);

    for (const GameObject* obj : *sc){
        rend->drawObject(obj);
    }

    rend->doneWithFrame();
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::pause: Pause the game
 * @details Pause the game by stopping the timer and setting inGame to false.
 */
void Game::pause() {
    timer.stop();
    inGame = false;
    sc->setPaused(true);
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::resume: Resume the game
 * @details Resume the game by starting the timer and setting inGame to true.
 */
void Game::resume() {
    timer.start();
    inGame = true;
    sc->setPaused(false);
}