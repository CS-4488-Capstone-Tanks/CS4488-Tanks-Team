


///
/// \brief LS S24 The GameWindow class governs the main window that displays the game and all menus.
///

#include "gamewindow.h"

/* LS 3/7/24
 * This file is a part of the Tanks Game GUI/display system. It is the main window that will display the game.
 *
 * It has a protected window that will display each widget. It interacts with MenuManager via signals and slots
 * to find and display menus. When it is not displaying a menu (E.g. std::unordered_map contains only InGameMenu
 * and fetch_widget has not been called), it is displaying the renderer which is always kept loaded.
 *
 * While it is not a QMainWindow, it is our game's main window. We chose QWindows because we felt that they were
 * closer to our vision without menu bars, status bars, etc. It displays QWidgets that contain the actual
 * graphics for the game.
 *
 * ========================================================================
 * Standard Operating Flow:
 * Say GameWindow is currently displaying the Renderer widget, and
 *     widgetcache only has Renderer (Active) and InGameMenu
 *
 * At some point, ESC is pressed, which activates the parent QApp's slot
 *
 * Parent QApp will send signal to GameWindow that ESC has been pressed
 *
 * GameWindow will call displayWidget, which hides the renderer and
 *     displays the InGameMenu, then calls the fetch_widget method
 *
 * Once InGameMenu has been displayed, fetch_widget will send a
 *     signal to the Menu Manager saying, "Get me the Main Menu, in
 *     case we need to return to it from here."
 *
 * User can either Return, which switches back to the Renderer or they
 *     can Exit to Menu/Quit Game, which switches to the Main Menu and
 *     destroys the Renderer and InGameMenu.
 *
 * Once the main menu is displayed, the game will create a fresh Renderer,
 *     as well as fetch MultiplayerMenu to have on hand.
 *
 * ========================================================================
 *
 * The goal of this operating flow, where q_widgets gets added to and removed from, is to keep the game from
 * being resource intensive. Though our game is simple enough to where menus could simply be permanently
 * stored, we wanted to keep the option of scaling the game by adding more menus. This lets us simply
 * add more menus to the menumanager and change the signal/slot to accommodate without needing to hard-code q_widgets.
 *
 */


/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief fetchAdjacent: Call Menu Manager to return list of pointers to accessible Qwidgets, which are added to the cache
 * @details This method calls the Menu Manager to fetch a list of pointers to accessible Qwidgets. It then adds the widgets to the cache. It does not return anything.
 * @param wpointer Pointer to the current widget
 */
void GameWindow::fetchAdjacent(QWidget* wpointer)
{
    std::vector <QWidget*> adjacent = {};
    if (wpointer == &renderer) {
        adjacent.push_back(menuMan.fetch_menu(INGAME_MENU_KEY));
    }
    else if (wpointer == menuMan.fetch_menu(INGAME_MENU_KEY)) {
        adjacent.push_back(menuMan.fetch_menu(MAIN_MENU_KEY));
    }
    else if (wpointer == menuMan.fetch_menu(MAIN_MENU_KEY)){
        // adjacent.push_back(menuMan.fetch_menu(MULTIPLAYER_MENU_KEY));      Unimplemented
        adjacent.push_back(menuMan.fetch_menu(OPTIONS_MENU_KEY));
    }
    for (auto widget : adjacent) {
        widgetCache.insert({widgetCache.size(), widget});
    }
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief clearAndFetch: Call Menu Manager to add both desired menu and other accessible widgets
 * @details This method calls the Menu Manager to fetch a menu by a given key. It then calls fetchAdjacent to fetch adjacent widgets.
 * @param key: Key to send to the Menu Manager
 */
void GameWindow::clearAndFetch(int key)
{
    widgetCache = {
            {GAME_KEY, &renderer} // Keep renderer loaded at key 0, but remove other menus
    };
    widgetCache.insert({key, menuMan.fetch_menu(key)}); // Fetch desired widget
    fetchAdjacent(widgetCache[key]); // Fetch adjacent widgets
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief cacheCheck: Checks the cache for a key. If present, returns the Qwidget pointer. If not, fetches widget.
 * @details This method checks the cache for a widget. If the widget is present, it returns the pointer. If the widget is not present, it calls cacheMiss to fetch the widget.
 * @param key: Assigned key for the cache map
 * @return wpoint: Pointer to desired Qwidget
 */
QWidget* GameWindow::cacheCheck(int key)
{
    QWidget* wpoint;
    try {
        wpoint = widgetCache.at(key);
    }
    catch(std::out_of_range& e) {
        std::cerr << "Widget not found in cache. Fetching widget." << std::endl;
        wpoint = cacheMiss(key);
    }
    return wpoint;
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief cacheMiss: Fetches a widget from the Menu Manager
 * @details If the widget is not found in the cache, this method will fetch the widget from the Menu Manager.
 * @param key key for the widget
 * @return pointer to the widget
 */
QWidget* GameWindow::cacheMiss(int key)
{
    QWidget* wpoint;
    clearAndFetch(key);
    wpoint = widgetCache[key];
    return wpoint;
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief GameWindow: Constructor
 * @details Constructor for the GameWindow class. Initializes the MenuManager, QWindow, Renderer, and widgetCache.
 */
GameWindow::GameWindow(): menuMan(MenuManager()), win(QWindow()), renderer(Renderer()), widgetCache({})
{
    activeKey = MAIN_MENU_KEY; // Start at main menu
    changeWidget(activeKey);

    win.create();
    QSize qsize = QSize(1200, 900);
    win.setBaseSize(qsize);
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief changeWidget: Changes the active QWidget
 * @details This method changes the active QWidget. It hides the current widget and displays the new widget.
 * @param key
 */
void GameWindow::changeWidget(int key)
{
    if (key == activeKey) { // Don't change if already active
        return;
    }
    hideWidget(); // Hide current widget
    activeKey = key;
    displayWidget(); // Display new widget
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief displayWidget: Displays the active QWidget
 */
void GameWindow::displayWidget()
{
    try {
        QWidget *wpoint = cacheCheck(activeKey);
        wpoint->show();
    }
    catch(std::exception& e) {
        std::cerr << "Error encountered in GameWindow: " << e.what() << std::endl;
        throw e;
    }
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief hideWidget: Hide the active widget
 */
void GameWindow::hideWidget()
{
    try {
        QWidget *wpoint = cacheCheck(activeKey);
        wpoint->hide();
    }
    catch(std::exception& e) {
        std::cerr << "Error encountered in GameWindow: " << e.what() << std::endl;
        throw e;
    }
}
