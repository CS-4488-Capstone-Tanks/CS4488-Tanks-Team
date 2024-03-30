#include <QApplication>
#include <QLabel>
#include "scene.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QLabel label("Hello world");
    label.resize(200, 50);
    label.show();

    // example
    std::string stateName = "test_game_state";
    Scene scene(60.0f/1000, stateName);
    scene.start();
    scene.update(); //This will a slot called by signals
    // example

    return app.exec();
}
