#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "gameobject.h"


/**
 * The GameState class manages the game objects in the Scene.
 * It is responsible for loading (and saving later) the state of the game.
 * It also updates all game objects in the scene.
 * @author Koda Koziol
 * @date SPRING 2024
*/
class GameState
{
public:
    /**
     * @brief Constructor for the GameState.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    GameState();

    /**
     * @brief Should be once at the start of the game. This method will call the doStart()
        method for each GameObject in the GameState.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void startState();

    /**
     * @brief Should be called once per frame. This method will call the doUpdate() method
        for each GameObject in the GameState.
     * @param deltaTime: The time elapsed since the last update in seconds(?).
        This is a fixed value. See doUpdate() in GameObject for more information.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void updateState(float deltaTime);

    /**
     * @brief Given a the name of a .json state file, this method will load the state.
        Note that the state file must be located in the "assets/levels/"
        directory. Do not include ".json" in the filename.
     * @param filename
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void loadState(std::string filename);

    /**
     * @brief Add a GameObject to the GameState.
     * @param obj: The GameObject to add to the GameState.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void addObject(GameObject *const obj);

    /**
     * @brief Remove a GameObject from the GameState.
     * @param entityID
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void removeObject(uint32_t entityID);




	/**
	 * @return A constant iterator pointing to the beginning of the vector of GameObject pointers.
	 * @author Tyson Cox (implemented by Koda Koziol)
	 * @date SPRING 2024
	 */
    [[nodiscard]] std::vector<GameObject*>::const_iterator begin() const;

	/**
	 * @return A constant iterator pointing to the end of the vector of GameObject pointers.
	 * @author Tyson Cox (implemented by Koda Koziol)
	 * @date SPRING 2024
	 */
    [[nodiscard]] std::vector<GameObject*>::const_iterator end() const;


private:
    std::vector<GameObject*> objs;
    int nextFreeEntityID = 0;
};

#endif // GAMESTATE_H
