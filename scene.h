#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "gamestate.h"


/**
 * @brief The Scene class is responsible for managing the game state and updating the game objects.
 * @author Koda Koziol
 * @date SPRING 2024
*/
class Scene : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Scene object
     * @param float deltaTime
     * @param char stateFilename[]
     * @param QObject* parent = nullptr
     * @author Koda Koziol
     * @date SPRING 2024
    */
    explicit Scene(float deltaTime, std::string stateFilename, QObject *parent = nullptr);

    /**
     * @brief Start the scene. This should be called once at the start of the game,
        before the update loop begins.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    void start();

    // void keyPress();

    /**
     * @brief Pause or resume the scene
     * @param bool isPaused
     * @author Koda Koziol
     * @date SPRING 2024
    */
    void setPaused(bool isPaused);

	/**
	 * @return A constant iterator pointing to the beginning of the vector of GameObject pointers.
     * @author Tyson Cox (implemented by Koda Koziol)
     * @date SPRING 2024
	 */
    std::vector<GameObject*>::const_iterator begin() const;

    /**
     * @return A constant iterator pointing to the end of the vector of GameObject pointers.
     * @author Tyson Cox (implemented by Koda Koziol)
     * @date SPRING 2024
     */
    std::vector<GameObject*>::const_iterator end() const;



private:
    // NetworkManager networkManager
    GameState gameState;
    float deltaTime; // This should probably be initialized in game.h, where the QTimer is
    bool isPaused = false;

signals:
public slots:
    /**
     * @author Luna Steed
     * @time Spring 2024
     * @brief Scene::getPlayerTank() returns the player's tank
     */
     GameObject* getPlayerTank();

    /**
     * @brief Update the scene. This should be called once per frame.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    void update();
};

#endif // SCENE_H
