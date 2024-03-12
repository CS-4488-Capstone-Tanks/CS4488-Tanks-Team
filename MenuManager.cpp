/* Luna Steed S24
**** TODO:
*		Menu Manager
*		- Keep menus ready when GameWindow calls to display a menu
*       - Alleviates stress on GameWindow by abstracting menu handling
*		- GameWindow can keep a reference to an active instance of MenuManager to display menus
*		- Also allows communication from menus to GameWindow
*/
#include "MenuManager.h"

class MenuManager : QObject{
#define MAIN_MENU_KEY 0
#define INGAME_MENU_KEY 2
#define OPTIONS_MENU_KEY 3
#define MULTIPLAYER_MENU_KEY 4
#define HOST_MENU_KEY 5
#define JOINIP_MENU_KEY 6

    std::vector<QWidget*> QW_Vec;

    QWidget Main_Menu;
    QWidget In_Game_Menu;
    QWidget Options_Menu;
    QWidget Multiplayer_Menu;
    QWidget Host_Menu;
    QWidget Join_IP_Menu;

    ///
    /// \brief MenuManager: Constructor
    ///
    MenuManager(){
        QW_Vec = {};
    }
    /// LS S24
    /// \brief send_signal: Send out a signal (generally to GameWindow)
    ///
    void send_signal(){

    }

    /// LS S24
    /// \brief receive_signal: Receive a signal (generally from GameWindow)
    ///
    void receive_signal(){

    }

    /// LS S24
    /// \brief fetch_menu: Fetches a QWidget menu.
    /// \return Q Widget Pointer
    ///
    QWidget* fetch_menu(){

    }
};
