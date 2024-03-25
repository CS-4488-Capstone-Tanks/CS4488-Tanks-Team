#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "renderer.h"
#include "menumanager.h"
#include <unordered_map>
#include <iostream>


class GameWindow : public QMainWindow {
    Q_OBJECT

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
