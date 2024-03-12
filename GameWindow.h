#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "Renderer.h"
#include "qwindow.h"
#include "MenuManager.h"


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


protected:

    QWindow win;

private:
    // MenuManager::MenuManager menuman; // Doesn't work yet, will figure out soon
    Renderer renderer;
    std::unordered_map<int, QWidget*> widget_cache;
    int active_key;

    /// LS S24
    /// \brief change_active_key: Change active widget
    /// \param new_widget_key: New active widget key ('Active' is visible)
    ///
    void change_active(int new_widget_key)
    {

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

    }

    /// LS S24
    /// \brief fetch_by_key: Call Menu Manager to add both desired menu and other accessible widgets
    /// \param key: Key to send to the Menu Manager
    ///
    /// Mostly used for Cache Misses
    ///
    void fetch_by_key(int key)
    {

    }

    /// LS S24
    /// \brief cache_check: Checks the cache for a key. If present, returns the Qwidget pointer. If not, fetches widget.
    /// \param key: Assigned key for the cache map
    /// \return wpoint: Pointer to desired Qwidget
    ///
    QWidget* cache_check(int key)
    {
    }

    // To be implemented. Will utilize fetch_by_key to load desired menu from Menu Manager into cache and return it.
    /* QWidget* cache_miss(int key)
       {

       }
    */

public:

    /// LS S24
    ///
    /// \brief GameWindow: Constructs basic window
    ///
    GameWindow()
    {
    }

    /// LS S24
    ///
    /// \brief display_widget: grab new active Qwidget from widget_cache, change active widget.
    ///
    void change_widget(int key)
    {

    }

    ///
    /// \brief display_widget: Displays the active QWidget
    ///
    void display_widget()
    {

    }

    /// LS S24
    ///
    /// \brief hide_widget: Hide the active widget
    ///
    void hide_widget()
    {

    }



    /// LS S24
    ///
    /// \brief receive_signal: Qslot to receive a Qsignal
    ///
    void receive_signal()
    {

    }

    /// LS S24
    ///
    /// \brief send_signal: Qsignal for Qslot
    ///
    void send_signal()
    {

    }

    /// LS S24
    ///
    /// \brief create_renderer: Create a renderer, load it into cache, and transport information
    ///
    void create_renderer()
    {

    }
};

#endif // GAMEWINDOW_H
