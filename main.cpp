#include "game.h"

int main(int argc, char** argv) {
    Game* game = Game::getInstance(argc, argv);
    int code = game->start();
    Game::destroyInstance();
    return code;
}
