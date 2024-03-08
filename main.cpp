#include <QApplication>
#include <QLabel>
#include "GameWindow.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    GameWindow gw = GameWindow();
    gw.show();
    QLabel label("Hello world");
    label.resize(200, 50);
    label.show();
    return app.exec();
}
