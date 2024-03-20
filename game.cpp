/* Luna Steed S24
**** TODO:
*		Game
*		- Connect the windows and most notably the renderer to the 'back-end'
*		- Passes information such as key presses along to where they need to go
*		- Allows gameobjects, etc. to speak to the gamewindow, which controls the renderer.
*/

#include "game.h"

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::Game constructor
 * @details Constructor for the Game class. Initializes the GameWindow and Scene objects, and sets inGame to false. Sets parent app if provided.
 * @param application pointer to QApplication
 */
Game::Game(QApplication* application) : gw(), sc(0, reinterpret_cast<char *>('Game')), timer(new QTimer(this)) {
    app = application;
    gw.show();
    inGame = false;
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::start: Start the game
 * @details Start the game when prompted by the signal. Changes the active widget to the renderer and sets inGame to true.
 */
void Game::start() {
    gw.changeWidget(RENDERER_KEY);
    inGame = true;
    timerStart();
}

// QSIGNALS: switchWidget, update, setPaused, timerStart, timerStop

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief signal_format: Format a signal
 * @details Format a signal using a string and arguments
 * @param format format string. Intended format:"{}({})"
 * @param method method to call. Intended method to call (eg "switchWidget")
 * @param args arguments to pass. Intended arguments to pass (eg "RENDER_KEY")
 * @return formatted signal
 */

char * signal_format(std::string& method, std::string& args){
    // Format declaration and conversion to char *
    std::string format = "{}({})"; // Intended format for the signal. {} is the method, ({}) is the arguments.
    char * conv_format = new char[format.size() + 1]; // Create a char* to hold the format
    std::strcpy(conv_format, format.c_str()); // Copy the chars from format to the char*
    // Intended result: conv_format == ['{', '}', '(', '{', '}', '\0']

    // Convert method into a char*
    char * conv_method = new char[method.size() + 1];
    std::strcpy(conv_method, method.c_str()); // Copy the chars from method to the char*
    // Example Intended result: conv_method == ['s', 'w', 'i', 't', 'c', 'h', 'W', 'i', 'd', 'g', 'e', 't', '\0']

    // Convert the args into a char*
    char * conv_args = new char[args.size() + 1];
    std::strcpy(conv_args, args.c_str()); // Copy the chars from args to the char*
    // Example Intended result: conv_args == ['R', 'E', 'N', 'D', 'E', 'R', '_', 'K', 'E', 'Y', '\0']

    // Format the signal with the method and arguments
    // To be implemented.
 /*   char * signal = reinterpret_cast<char *>(std::snprintf(conv_format, conv_method, "%s", conv_args)); */
 /*   return signal; // Return the formatted signal */
    // Example Intended Return:
    // signal == ['s', 'w', 'i', 't', 'c', 'h', 'W', 'i', 'd', 'g', 'e', 't', '(', 'R', 'E', 'N', 'D', 'E', 'R', '_', 'K', 'E', 'Y', ')', '\0']
    // We want this because it will allow us to define signals dynamically.
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::switchWidget: Switch the active QWidget
 * @details Switch the active QWidget when prompted by the signal
 * @param key key for the widget
 */
void Game::switchWidget(int key) {
    const QObject * sender = this;
    const char *signal;
    std::string k = std::to_string(key);
    signal = (char *) std::snprintf("switchWidget({})", k);
}

/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::timerStart: Start the QTimer
 */
void Game::timerStart() { //
    const QObject * sender = this;
    const char * signal = "start(1000/30)"; // Emit a signal to start the QTimer with an interval of 30 frames per second
    const QObject * receiver = timer;
    const char * method = "start(1000/30)";
    QObject::connect(sender, signal, receiver, method);
}



// QSLOTS: keyPress, timerHandle, update, setPaused
/**
 * @author Luna Steed
 * @time Spring 2024
 * @brief Game::keyPress: Handle key presses
 * @details Handle key presses when prompted by the application
 */
void Game::keyPress(int key)
{
    if (key == Qt::Key_Escape && !inGame) { // If not in game, quit
        app->quit();
    }
    else if (key == Qt::Key_Escape) { // If in game, pause and switch to in-game menu
        setPaused(!sc.isPaused);
        if (sc.isPaused) {
            gw.changeWidget(INGAME_MENU_KEY); // If the game is now paused, switch to the ingame menu
        }
        else {
            gw.changeWidget(RENDERER_KEY); // If the game is now unpaused, switch to the renderer
        }
    }

    if (inGame) { // When in game, set these keys to control the player's movement and actions
        if (key == Qt::Key_Left || key == Qt::Key_A) {
            sc;
        }
        if (key == Qt::Key_Right || key == Qt::Key_D) {
            // Rotate player rightwards
        }
        if (key == Qt::Key_Up || key == Qt::Key_W) {
            // Move player forward
        }
        if (key == Qt::Key_Down || key == Qt::Key_S) {
            // Move player backward
        }
        if (key == Qt::Key_Space) {
            // Fire
        }
    }
}

/**
 * @brief Game::update: Update the game
 * @details Update the game on a QTimer signal using Scene's update method.
 */
void Game::update() {
    sc.update();
}

/**
 * @brief Game::setPaused: Set the game to paused
 * @details Set the game to paused when prompted by the signal
 */
void Game::setPaused(bool paused) {
    sc.setPaused(paused);
}

/**
 * @brief Game::timerHandle: Handle the QTimer signal
 * @details Handle the QTimer signal by updating the game
 */
void Game::timerHandle() {
    const QObject * sender = timer;
    const char * signal = "timeout()";
    const QObject * receiver = this;
    const char * method = "update()";
    QObject::connect(sender, signal, receiver, method);
}