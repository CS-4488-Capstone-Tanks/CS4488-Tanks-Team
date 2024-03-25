#include "game.h"

int main(int argc, char** argv) {
#if 1
        Game game(argc, argv);
        return game.start();
#else

    QApplication app(argc, argv);

    Renderer renderer;
    renderer.show();

    PlayerTank tank;

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&renderer, &tank]() {
        renderer.drawObject(&tank);
        renderer.doneWithFrame();
    });

    timer.start(16);

    return app.exec();
#endif
}
