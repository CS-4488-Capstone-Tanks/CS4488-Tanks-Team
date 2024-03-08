#include <QApplication>
#include <QObject>
#include <QLabel>
#include <QWindow>
#include <QWidget>
#include "Renderer.h"
#include "MenuManager.h"

/* Luna Steed S24
**** TODO:
*		Method to display a menu that takes in a key from QT Slot and displays a menu. -- Interact with MenuManager
*/

///
/// \brief LS S24 The GameWindow class governs the main window that displays the game and all menus.
///

/* LS 3/7/24
 * This file is a part of the Tanks Game GUI/display system.
 *
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
 *     case we need to return to it from here.
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

class GameWindow : public QWindow
{
// Default key values for mapping to QWidget Pointers
#define MAIN_MENU_KEY 0
#define RENDERER_KEY 1
#define INGAME_MENU_KEY 2
#define OPTIONS_MENU_KEY 3
#define MULTIPLAYER_MENU_KEY 4
#define HOST_MENU_KEY 5
#define JOINIP_MENU_KEY 6



private:
    MenuManager menuman;
    Renderer renderer;
    std::unordered_map<int, QWidget*> widget_cache;
    int active_key;

    /// LS S24
    /// \brief change_active_key: Change active widget
    /// \param new_widget_key: New active widget key ('Active' is visible)
    ///
    void change_active(int new_widget_key)
    {
        active_key = new_widget_key;
    }

    /// LS S24
    ///
    /// \brief fetch_adjacent: Call Menu Manager to return list of pointers to accessible Qwidgets, which are added to the cache
    /// \param wpointer: Pointer to the desired widget, gathered from widget cache
    ///
    /// NOTE: fetch_adjacent does not return anything, it only adds widgets to the cache. MenuManager is returning to this method.
    ///
    void fetch_adjacent(QWidget* wpointer)
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
    void fetch_by_key(int key)
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
    QWidget* cache_check(int key)
    {
        QWidget* wpoint = widget_cache[key];
        return wpoint;
    }

    // To be implemented. Will utilize fetch_by_key to load desired menu from Menu Manager into cache and return it.
    /* QWidget* cache_miss(int key)
       {

       }
    */

public:
    QWindow win;

    /// LS S24
    ///
    /// \brief GameWindow: Constructs basic window
    ///
    GameWindow()
    {
        change_active(MAIN_MENU_KEY);
        change_widget(active_key);
        menuman = MenuManager();


        win.create();
        QSize qsize = QSize(1200, 900);
        win.setBaseSize(qsize);
        win.showNormal();
    }

    /// LS S24
    ///
    /// \brief display_widget: grab new active Qwidget from widget_cache, change active widget.
    ///
    void change_widget(int key)
    {
        this->change_active(key);

        /* TO DO:
         * Use Cache Check to get a pointer
         * Dereference pointer to get QWidget
         * Set active display to QWidget
         */
    }

    ///
    /// \brief display_widget: Displays the active QWidget
    ///
    void display_widget()
    {
        /* TO DO:
         * Display Widget
         */
    }

    /// LS S24
    ///
    /// \brief hide_widget: Hide the active widget
    ///
    void hide_widget()
    {
        /* TO DO:
         * Hide Widget
         */
    }



    /// LS S24
    ///
    /// \brief receive_signal: Qslot to receive a Qsignal
    ///
    void receive_signal()
    {
        /* TO DO:
         * Set up QSlots/QSignals between Game and QWindow's interacting classes
         */
    }

    /// LS S24
    ///
    /// \brief send_signal: Qsignal for Qslot
    ///
    void send_signal()
    {
        /* TO DO:
         * Set up QSlots/QSignals between Game and QWindow's interacting classes
         */
    }

    /// LS S24
    ///
    /// \brief create_renderer: Create a renderer, load it into cache, and transport information
    ///
    void create_renderer()
    {
        /* TO DO:
         * Set up QSlots/QSignals between Game and QWindow's interacting classes
         */
    }
};
