///
/// \brief LS S24 The GameWindow class governs the main window that displays the game and all menus.
///

#include "gamewindow.h"

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief GameWindow: Constructor
 * @details Constructor for the GameWindow class. Initializes the MenuManager, QWindow, Renderer, and widgetCache.
 */
GameWindow::GameWindow(QObject *parent, int startKey)
{
    // Set up the widget cache
    rend = new Renderer();
    mainMenu = new MainMenu();
    pauseMenu = new PauseMenu();
    gameOver = new GameOver();
    levelMenu = new LevelMenu();

    widgets = {
            {GAME_KEY, rend},
            {MAIN_MENU_KEY, mainMenu},
            {PAUSE_MENU_KEY, pauseMenu},
            {GAME_OVER_KEY, gameOver},
            {LEVEL_MENU_KEY, levelMenu}
    };

    activeKey = startKey;

    changeWidget(activeKey);

    this->create();
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief changeWidget: Changes the active QWidget
 * @details This method changes the active QWidget. It hides the current widget and displays the new widget.
 * @return QWidget* The active widget
 * @param key
 */
QWidget* GameWindow::changeWidget(int key)
{
    if (key == activeKey) { // Don't change if already active
        displayWidget();
        return widgets.at(activeKey);
    }
    else {
        hideWidget(); // Hide current widget
        activeKey = key;
        displayWidget(); // Display new widget
        return widgets.at(activeKey);
    }
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief displayWidget: Displays the active QWidget
 */
void GameWindow::displayWidget()
{
    try {
        QWidget* wpoint = widgets.at(activeKey);
        this->setCentralWidget(wpoint);
        wpoint->setParent(this);
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
        QWidget *wpoint = widgets.at(activeKey);
        wpoint->setParent(nullptr);
        this->setCentralWidget(nullptr);
    }
    catch(std::exception& e) {
        std::cerr << "Error encountered in GameWindow: " << e.what() << std::endl;
        throw e;
    }
}

// Public Q Slots: keyPressEvent(QKeyEvent *event), keyReleaseEvent(QKeyEvent *event)
/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief GameWindow::keyPressEvent: Handle key press events
 * @details Handle key press events by passing them to the game window.
 * @param event The key press event
 */
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    emit keySignal(event);
}


/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief GameWindow::keyReleaseEvent: Handle key release events
 * @details Handle key release events by passing them on to
 * @param event The key release event
 */
void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    emit keySignal(event);
}

/**
 * @author Tyson Cox
 * @date Spring 2024
 * @param key The key of the widget to fetch
 * @return the QWidget* of the menu that the key represents
 */
QWidget* GameWindow::getWidget(int key) {
    return widgets.at(key);
}
