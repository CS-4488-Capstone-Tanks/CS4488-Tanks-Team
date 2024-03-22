/* Luna Steed S24
**** TODO:
*		Game
*		- Connect the windows and most notably the renderer to the 'back-end'
*		- Passes information such as key presses along to where they need to go
*		- Allows gameobjects, etc. to speak to the gamewindow, which controls the renderer.
*/

#include "game.h"
#include "renderer.h"

Game::Game(int argc, char** argv) : QApplication(argc, argv) {
    scene = new Scene(60.0f/1000, "test_game_state");
    window = new GameWindow();
}

Game::~Game() {
    delete scene;
    delete window;
}

void Game::tick() {
    scene->update();

    QWidget* widget = window->retrieveMenu(GAME_KEY);
    Renderer* renderer = dynamic_cast<Renderer*>(widget);

    for(const GameObject* obj : *scene) {
        renderer->drawObject(obj);
    }

    renderer->doneWithFrame();
}

int Game::start() {
    scene->start();

    connect(&timer, &QTimer::timeout, this, &Game::tick);

    // 16 ms, or roughly 60 fps
    timer.setInterval(16);
    timer.start();

    window->show();

    return this->exec();
}