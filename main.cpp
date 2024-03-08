#include <QApplication>
#include <QLabel>
#include "scene.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QLabel label("Hello world");
    label.resize(200, 50);
    label.show();
    Scene scene(60.0f/1000, "test_game_state");
    // Scene scene(60.0f/1000);
    scene.start();
    scene.update();
    return app.exec();
}
