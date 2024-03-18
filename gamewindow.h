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
// Default key values for mapping to QWidget Pointers
#define MAIN_MENU_KEY 0
#define RENDERER_KEY 1
#define INGAME_MENU_KEY 2
#define OPTIONS_MENU_KEY 3
#define MULTIPLAYER_MENU_KEY 4
#define HOST_MENU_KEY 5
#define JOINIP_MENU_KEY 6


protected:
    QWindow win;

private:
    MenuManager menuMan;
    Renderer renderer;
    std::unordered_map<int, QWidget *> widgetCache;
    int activeKey;

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief fetchAdjacent: Call Menu Manager to return list of pointers to accessible Qwidgets, which are added to the cache
     * @param wpointer
     *
     * NOTE: fetchAdjacent does not return anything, it only adds widgets to the cache. MenuManager is returning to this method.
     */
    void fetchAdjacent(QWidget *wpointer);

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief fetchByKey: Call Menu Manager to add both desired menu and other accessible widgets
     * @param key: Key to send to the Menu Manager
     */
    void fetchByKey(int key);

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief cacheCheck: Checks the cache for a key. If present, returns the Qwidget pointer. If not, fetches widget.
     * @param key: Assigned key for the cache map
     * @return wpoint: Pointer to desired Qwidget
     */
    QWidget *cacheCheck(int key);

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief cacheMiss: Fetches a widget from the Menu Manager
     * @param key key for the widget
     * @return pointer to the widget
     */
    QWidget *cacheMiss(int key);

public:

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief GameWindow: Constructor
     */
    GameWindow();

    /**
     * @a+uthor Luna Steed
     * @time Spring 2024
     * @brief changeWidget: Changes the active QWidget
     * @param key
     */
    void changeWidget(int key);

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief displayWidget: Displays the active QWidget
     */
    void displayWidget();

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief hideWidget: Hide the active widget
     */
    void hideWidget();

    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief connect: Send Qsignal to MenuManager
     */
    Q_SIGNAL void callMenuMan(int key);
    Q_SLOT void receiveMenu();

    friend class MenuManager;
    friend class Game;
};



#endif // GAMEWINDOW_H
