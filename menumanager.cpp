/* Luna Steed S24
*		Menu Manager
*		- Keep menus ready when GameWindow calls to display a menu
*       - Alleviates stress on GameWindow by abstracting menu handling
*		- GameWindow can keep a reference to an active instance of MenuManager to display menus
*		- Also allows communication from menus to GameWindow
*/
#include "menumanager.h"

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief MenuManager: Constructor
 * @details Constructor for the MenuManager class
 */
MenuManager::MenuManager() : Main_Menu(), renderer(), In_Game_Menu(), Options_Menu(){
    QW_Vec = {
                &Main_Menu,
                &renderer,
                &In_Game_Menu,
                &Options_Menu //,
                //&Multiplayer_Menu,
                //&Host_Menu,
                //&Join_IP_Menu
             };
    active_widget = &Main_Menu;
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief MenuManager: Destructor
 * @details Destructor for the MenuManager class. Deletes all menus/QWidgets.
 */
MenuManager::~MenuManager() {
    delete active_widget; // Delete active menu
    std::vector<QWidget*>().swap(QW_Vec); // Clear vector and free memory
}

/**
 * @brief setActiveMenu: Set the active menu
 * @details Sets the active menu to the given QWidget pointer.
 * @param menu: QWidget pointer
 */
void MenuManager::setActiveMenu(QWidget* menu) {
    active_widget = menu;
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief checkActiveMenu: Check if the active menu is the given menu
 * @details Checks if the active menu is the given QWidget pointer.
 * @param menu: QWidget pointer
 * @return bool: True if the active menu is the given menu, false otherwise
 */
bool MenuManager::checkActiveMenu(QWidget* menu) {
    return active_widget == menu;
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief fetch_menu: Fetches a QWidget menu.
 * @return Q Widget Pointer
 */
QWidget* MenuManager::fetch_menu(int key){
    QWidget* menu = QW_Vec[key];
    if (!checkActiveMenu(menu)) {
        setActiveMenu(menu);
    }
    return active_widget;
}
