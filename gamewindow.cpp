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
GameWindow::GameWindow()
{
    // Set up the widget cache
    rend = new Renderer();
    mainMenu = new MainMenu();
    inGameMenu = new InGameMenu();
    optionsMenu = new OptionsMenu();

    widgets = {
            {GAME_KEY, rend},
            {MAIN_MENU_KEY, mainMenu},
            {INGAME_MENU_KEY, inGameMenu},
            {OPTIONS_MENU_KEY, optionsMenu}
    };

    activeKey = MAIN_MENU_KEY;

    changeWidget(activeKey);

    this->create();
    QSize qsize = QSize(1200, 900);
    this->setBaseSize(qsize);
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
        wpoint->hide();
    }
    catch(std::exception& e) {
        std::cerr << "Error encountered in GameWindow: " << e.what() << std::endl;
        throw e;
    }
}
