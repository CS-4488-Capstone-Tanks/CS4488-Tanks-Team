// Created by Luna Steed and Tyson Cox 03/2024

#include "game.h"


/**
 * @author Tyson Cox, Luna Steed
 * @time Spring 2024
 * @brief Game::Game constructor
 * @details Constructor for the Game class. Initializes the GameWindow and Scene objects, and sets inGame to false.
 */
Game::Game(int argc, char** argv) : QApplication(argc, argv), timer(new QTimer(this)) {
    activeKey = MAIN_MENU_KEY;
    gw = new GameWindow();
    gw->show();
    std::string stateFilename = "test_game_state";
    sc = new Scene(60.0f/1000, stateFilename);
    inGame = false;

    connect(gw, &GameWindow::keySignal, this, &Game::filterKeyEvent); // Connect the GameWindow's keySignal to the Game's filterKeyEvent
    auto *playerTank = sc->getPlayerTank(); // The scene holds a reference to the player tank, so we retrieve it
    // connect(this, &Game::playerControlSignal, playerTank, Tank::handleKeyEvent);
    // Waiting on Tank::handleKeyEvent to be implemented
}


/**
 * @author Tyson Cox, Luna Steed
 * @time Spring 2024
 * @brief Game::~Game destructor
 * @details Destructor for the Game class. Deletes the GameWindow and Scene objects.
 */
Game::~Game() {
    delete &timer;
    delete gw;
    delete sc;
    delete this;
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

    activeKey = GAME_KEY;
    gw->changeWidget(activeKey);
    gw->show();

    return Game::exec();
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
    activeKey = INGAME_MENU_KEY;
    gw->changeWidget(activeKey);
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
    activeKey = GAME_KEY;
    gw->changeWidget(activeKey);
    sc->setPaused(false);
}


/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::end: End the game
 * @details End the game by stopping the timer, setting inGame to false, destroying and remaking the scene, and changing the active widget to the main menu.
 */
void Game::end() {
    timer.stop();
    inGame = false;
    sc->~Scene();
    sc = new Scene(60.0f / 1000, "test_game_state");
    activeKey = MAIN_MENU_KEY;
    gw->changeWidget(activeKey);
}


// PRIVATE Q Slots: tick();
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


// PUBLIC Q Slots: keyPressEvent(QKeyEvent *event), keyReleaseEvent(QKeyEvent *event)
/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::filterKeyEvent: Filter key events
 * @details Filter key events with switch cases. Handle them in-house or pass them to the player tank object.
 * @param event The key event to filter
 */
void Game::filterKeyEvent(QKeyEvent *event) {
    switch (event->key())
    {
        case Qt::Key_Escape: // ESC
            if (inGame) { // in game
                pause();
                break;
            }
            else if (activeKey == INGAME_MENU_KEY) { // in in-game menu
                resume();
                break;
            }
            else if (activeKey == MAIN_MENU_KEY){ // main menu
                this->~Game();
                break;
            }
            else{ // anywhere else, return to main menu
                activeKey = MAIN_MENU_KEY;
                gw->changeWidget(activeKey);
                break;
            }
        // Player controls. DOES NOT HANDLE PRESS/RELEASE EVENTS: PlayerTank must handle this.
        // WASD
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        // Arrow keys
        case Qt::Key_Up:
        case Qt::Key_Left:
        case Qt::Key_Down:
        case Qt::Key_Right:
        // Firing
        case Qt::Key_Space:
        {
            if (inGame) {
                emit Game::playerControlSignal(event);
                break;
            }
            else { // Functionality should only be available in-game
                break;
            }
        }
    }
}