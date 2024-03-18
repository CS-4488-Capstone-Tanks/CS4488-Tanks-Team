/* Luna Steed S24
**** TODO:
*		Menu Manager
*		- Keep menus ready when GameWindow calls to display a menu
*       - Alleviates stress on GameWindow by abstracting menu handling
*		- GameWindow can keep a reference to an active instance of MenuManager to display menus
*		- Also allows communication from menus to GameWindow
*/
#include "menumanager.h"

#define MAIN_MENU_KEY 0
#define INGAME_MENU_KEY 2
#define OPTIONS_MENU_KEY 3
#define MULTIPLAYER_MENU_KEY 4
#define HOST_MENU_KEY 5
#define JOINIP_MENU_KEY 6

QWidget Main_Menu;
QWidget In_Game_Menu;
QWidget Options_Menu;
QWidget Multiplayer_Menu;
QWidget Host_Menu;
QWidget Join_IP_Menu;

///
/// \brief MenuManager: Constructor
///
MenuManager::MenuManager(){
    QW_Vec = {
                &Main_Menu,
                &In_Game_Menu,
                &Options_Menu,
                &Multiplayer_Menu,
                &Host_Menu,
                &Join_IP_Menu
             };
    active_menu = &Main_Menu;
}
/// LS S24
/// \brief sendSignal: Send out a signal (generally to GameWindow)
///
void MenuManager::send_signal(){

}

/// LS S24
/// \brief receiveSignal: Receive a signal (generally from GameWindow)
///
void MenuManager::receive_signal(){

}

/// LS S24
/// \brief fetch_menu: Fetches a QWidget menu.
/// \return Q Widget Pointer
///
QWidget* MenuManager::fetch_menu(int key){
    return QW_Vec[key];
}
