#include "GameWindow.h"


///
/// \brief LS S24 The GameWindow class governs the main window that displays the game and all menus.
///

/* LS 3/7/24
 * This file is a part of the Tanks Game GUI/display system. It is the main window that will display the game.
 *
 * It has a protected window that will display each widget. It interacts with MenuManager via signals and slots
 * to find and display menus. When it is not displaying a menu (Eg. std::unordered_map contains only InGameMenu
 * and fetch_widget has not been called), it is displaying the renderer which is always kept loaded.
 *
 * While it is not a QMainWindow, it is our game's main window. We chose QWindows because we felt that they were
 * closer to our vision without menu bars, status bars, etc. It displays QWidgets that contain the actual
 * graphics for the game.
 *
 * ========================================================================
 * Standard Operating Flow:
 * Say GameWindow is currently displaying the Renderer widget, and
 *     widget_cache only has Renderer (Active) and InGameMenu
 *
 * At some point, ESC is pressed, which activates the parent QApp's slot
 *
 * Parent QApp will send signal to GameWindow that ESC has been pressed
 *
 * GameWindow will call display_widget, which hides the renderer and
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
 * add more menus and change the signal/slot to accommodate without needing to hard-code q_widgets.
 *
 */

MenuManager menuman;
Renderer renderer;
std::unordered_map<int, QWidget*> widget_cache;
int active_key;


/// LS S24
///
/// \brief fetch_adjacent: Call Menu Manager to return list of pointers to accessible Qwidgets, which are added to the cache
/// \param wpointer: Pointer to the desired widget, gathered from widget cache
///
/// NOTE: fetch_adjacent does not return anything, it only adds widgets to the cache. MenuManager is returning to this method.
///
void GameWindow::fetch_adjacent(QWidget* wpointer)
{
    /* TO DO:
     * Create menu manager, basic menus, and add methods to fetch accessible menus by a given menu.
     */
}

/// LS S24
/// \brief fetch_by_key: Call Menu Manager to add both desired menu and other accessible widgets
/// \param key: Key to send to the Menu Manager
///
/// Mostly used for Cache Misses
///
void GameWindow::fetch_by_key(int key)
{
    /* TO DO:
     * Create menu manager, basic menus, and add methods to fetch a menu by a given key.
     */
}

/// LS S24
/// \brief cache_check: Checks the cache for a key. If present, returns the Qwidget pointer. If not, fetches widget.
/// \param key: Assigned key for the cache map
/// \return wpoint: Pointer to desired Qwidget
///
QWidget* GameWindow::cache_check(int key)
{
    QWidget* wpoint = widget_cache[key];
    return wpoint;
}

QWidget* GameWindow::cache_miss(int key)
{

}

QWindow win;

/// LS S24
///
/// \brief GameWindow: Constructs basic window
///
GameWindow::GameWindow(): menuman(MenuManager())
{
    active_key = MAIN_MENU_KEY;
    change_widget(active_key);

    win.create();
    QSize qsize = QSize(1200, 900);
    win.setBaseSize(qsize);
    win.showNormal();
}

/// LS S24
///
/// \brief display_widget: grab new active Qwidget from widget_cache, change active widget.
///
void GameWindow::change_widget(int key)
{

    /* TO DO:
     * Use Cache Check to get a pointer
     * Dereference pointer to get QWidget
     * Set active display to QWidget
     */
}

///
/// \brief display_widget: Displays the active QWidget
///
void GameWindow::display_widget()
{
    /* TO DO:
     * Display Widget
     */
}

/// LS S24
///
/// \brief hide_widget: Hide the active widget
///
void GameWindow::hide_widget()
{
    /* TO DO:
     * Hide Widget
     */
}



/// LS S24
///
/// \brief receive_signal: Qslot to receive a Qsignal
///
void GameWindow::receive_signal()
{
    /* TO DO:
     * Set up QSlots/QSignals between Game and QWindow's interacting classes
     */
}

/// LS S24
///
/// \brief send_signal: Qsignal for Qslot
///
void GameWindow::send_signal()
{
    /* TO DO:
     * Set up QSlots/QSignals between Game and QWindow's interacting classes
     */
}
/// LS S24
///
/// \brief create_renderer: Create a renderer, load it into cache, and transport information
///
void GameWindow::create_renderer()
{
    /* TO DO:
     * Set up QSlots/QSignals between Game and QWindow's interacting classes
     */
}
