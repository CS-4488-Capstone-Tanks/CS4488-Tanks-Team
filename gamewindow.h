#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Renderer.h"
#include "qwindow.h"
#include "menumanager.h"
#include <QObject>
#include <unordered_map>
#include <QWidget>
#include "game.h"


class GameWindow : public QWindow {
    Q_OBJECT

protected:
    // Protected Variables
    QWindow win; // win is protected so that any child classes can access it, if need be.

private:
    // Private Variables
    MenuManager menuMan;
    Renderer renderer;
    std::unordered_map<int, QWidget *> widgetCache;
    int activeKey;

    // Private Functions
    void fetchAdjacent(QWidget *wpointer);
    void clearAndFetch(int key);
    QWidget *cacheMiss(int key);

public:
    // Public Functions
    GameWindow();
    void changeWidget(int key);
    void displayWidget();
    void hideWidget();
    QWidget *cacheCheck(int key);
};



#endif // GAMEWINDOW_H
