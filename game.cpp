// Created by Luna Steed and Tyson Cox 03/2024

#include "game.h"
#include "PlayerTank.h"


/**
 * @author Tyson Cox, Luna Steed
 * @time Spring 2024
 * @brief Game::Game constructor
 * @details Constructor for the Game class. Initializes the GameWindow and Scene objects, and sets inGame to false.
 */
Game::Game(int argc, char** argv) : QApplication(argc, argv), timer(new QTimer(this)) {
    activeKey = MAIN_MENU_KEY;
    gw = new GameWindow(this, activeKey);
    QSize qsize = QSize(800, 600); // Set the size of the window
    gw->setFixedSize(qsize); // Set the fixed size of the window (no resizing allowed
    gw->show();

    // Note! This string intentionally left blank so as to not load a scene in the menu, now
    // that menus are loading the scene at the correct time
    std::string stateFilename = "";
    sc = new Scene(60.0f/1000, stateFilename);

    inGame = false;

    connect(gw, &GameWindow::keySignal, this, &Game::filterKeyEvent); // Connect the GameWindow's keySignal to the Game's fi
    /*
     * // lterKeyEvent
    auto* playerTank = sc->getPlayerTank(); // The scene holds a reference to the player tank, so we retrieve it
    if (!playerTank)
        throw std::runtime_error("Player tank not found in scene");
    */

    installEventFilter(this);
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
    connect(&timer, &QTimer::timeout, this, &Game::tick);

    timer.setInterval(16); // 16 msec, roughly 60 fps
    timer.start();

    inGame = true;

    activeKey = MAIN_MENU_KEY;
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
    activeKey = PAUSE_MENU_KEY;
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
    activeKey = GAME_OVER_KEY;
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

    QWidget* widg = gw->getWidget(GAME_KEY);
    auto* rend = dynamic_cast<Renderer*>(widg);

    for (const GameObject* obj : *sc){
        rend->drawObject(obj);
    }

    rend->doneWithFrame();
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::filterKeyEvent: Filter key events
 * @details Filter key events with switch cases. Handle them in-house or pass them to the player tank object.
 * @param event The key event to filter
 */
bool Game::filterKeyEvent(QKeyEvent* event) {

    if (event->type() == QEvent::KeyPress) {
        switch (event->key())
        {
            case Qt::Key_Escape: // ESC
                if (inGame) { // in game
                    pause();
                    return true;
                }
                else if (activeKey == PAUSE_MENU_KEY) { // in in-game menu
                    resume();
                    return true;
                }
                else if (activeKey == MAIN_MENU_KEY){ // main menu
                    exit(0);
                    return true;
                }
                else{ // anywhere else, return to main menu
                    activeKey = MAIN_MENU_KEY;
                    gw->changeWidget(activeKey);
                    return true;
                }
                // Player controls. DOES NOT HANDLE PRESS/RELEASE EVENTS: PlayerTank must handle this.
                // WASD
            case Qt::Key_W:
            case Qt::Key_A:
            case Qt::Key_S:
            case Qt::Key_D:
            case Qt::Key_Up:
            case Qt::Key_Left:
            case Qt::Key_Down:
            case Qt::Key_Right:
            case Qt::Key_Space:
                if (inGame) {
                    PlayerTank* player = dynamic_cast<PlayerTank*>(sc->getPlayerTank());
                    if (player)
                        return player->handleKeyEvent(event);
                    else
                        return false;
                }
            case Qt::Key_1:
                if (inGame) {
                    QWidget* widg = gw->changeWidget(GAME_KEY);
                    auto* rend = dynamic_cast<Renderer*>(widg);
                    rend->setCameraMode(Renderer::CameraMode::Static);
                }
                return true;
            case Qt::Key_2:
                if (inGame) {
                    QWidget* widg = gw->changeWidget(GAME_KEY);
                    auto* rend = dynamic_cast<Renderer*>(widg);
                    rend->setCameraMode(Renderer::CameraMode::Periscope);
                }
                return true;
            case Qt::Key_3:
                if (inGame) {
                    QWidget* widg = gw->changeWidget(GAME_KEY);
                    auto* rend = dynamic_cast<Renderer*>(widg);
                    rend->setCameraMode(Renderer::CameraMode::Chasing);
                }
                return true;
            case Qt::Key_4:
                if (inGame) {
                    QWidget* widg = gw->changeWidget(GAME_KEY);
                    auto* rend = dynamic_cast<Renderer*>(widg);
                    rend->setCameraMode(Renderer::CameraMode::Orbiting);
                }
                return true;
            default:
                return false;
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        if (inGame) {
            PlayerTank* player = dynamic_cast<PlayerTank*>(sc->getPlayerTank());
            if (player)
                return player->handleKeyEvent(event);
            else
                return false;
        }
        else return false;
    }
    else return false;
}

GameWindow* Game::getWindow() {
    return gw;
}

Game* Game::instance = nullptr;

Game* Game::getInstance(int argc, char** argv) {
    if (Game::instance) {
        return Game::instance;
    }
    else {
        return Game::instance = new Game(argc, argv);
    }
}

void Game::destroyInstance() {
    delete instance;
    instance = nullptr;
}
