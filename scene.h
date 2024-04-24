#ifndef SCENE_H
#define SCENE_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "gameobject.h"
#include <vector>

/**
 * The scene class manages the game objects in a virtual scene.
 * It is responsible for loading (and saving later) the state of the game.
 * It also updates all game objects in the scene.
 * 
 * Be aware that this is a singleton class, so you should not create an instance of it,
 * but instead use the getInstance() method to get the instance of the Scene.
 * 
 * Also be aware that this class is not currently thread-safe.
 * 
 * @author Koda Koziol
 * @date SPRING 2024
*/
class Scene
{
public:
    /**
	 * @brief Get the instance of the Scene. If the Scene has not been created yet,
		it will be created.
	 * @return Scene*: The instance of the Scene. This is a singleton class.
	 * @author Koda Koziol
	 * @date SPRING 2024
	 */
    static Scene *getInstance();

    /**
     * @brief Destroys all objects in the Scene. Used when loading a new Scene, or when cleaning up
     * @author Tyson Cox
     * @date SPRING 2024
     */
    void reset();

    /**
     * @brief This method allows GameObjects to initialize themselves before the level begins.
         This method should be once at the start of the level. Specifically, this method will call the doStart()
         method for each GameObject in the Scene.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void start();

    /**
     * @brief This method updates the states of all GameObjects in the Scene by calling doUpdate() on them,
         and then checking for collisions. It will also remove any GameObjects that are queued for destruction. 
     * This method should be called once per frame through the duration of a level.
     * @param deltaTime: The time elapsed since the last update in seconds(?).
        This is a fixed value. See doUpdate() in GameObject for more information.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void update(float deltaTime);

    /**
     * @brief Given a the name of a .json state file, this method will load the state, creating GameObjects and adding them to the Scene.
        Note that the state file must be located in the "assets/levels/"
        directory. Do not include ".json" in the filename.
     * @param filename
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void load(std::string filename);

    /**
     * @brief Pause or resume the scene
     * @param bool isPaused
     * @author Koda Koziol
     * @date SPRING 2024
    */
    void setPaused(bool isPaused);

    /**
	 * @brief Get the next free entity ID. This is used to assign a unique ID to each GameObject.
	 * @return int: The next free entity ID. This is a unique identifier for each GameObject in the game.
		It is used to differentiate between different GameObjects and is assigned by the Scene when the
		GameObject is added to the Scene. This method will increment the next free entity ID by 1.
	 * @author Koda Koziol
	 * @date SPRING 2024
	 */
    int getNextFreeEntityID();

    /**
     * @brief Add a GameObject to the Scene.
     * @param obj: The GameObject to add to the Scene.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    int addObject(GameObject *const obj);

    /**
     * @brief Remove a GameObject from the Scene.
     * @param entityID
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void removeObject(uint32_t entityID);

    /**
	 * @brief Get a GameObject by its entity ID.
	 * @param entityID: The entity ID of the GameObject to get.
	 * @return GameObject*: The GameObject with the given entity ID. If no GameObject
		with the given entity ID is found, nullptr is returned.
	 * @author Koda Koziol
	 * @date SPRING 2024
	 */
    GameObject *getGameObject(uint32_t entityID) const;

    /**
	 * @brief Get a GameObject by its type.
	 * @param type: The type of GameObject to get.
	 * @return GameObject*: The first GameObject of the given type found in the Scene.
	 * If no GameObject of the given type is found, nullptr is returned.
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
    GameObject *getGameObject(GameObjectType type) const;

    /**
	 * @brief Get all GameObjects of a type from the Scene.
	 * @param type: The type of GameObject to get.
	 * @return std::vector<GameObject*>: A vector of all GameObjects in the Scene.
	 * @author Koda Koziol
	 * @date SPRING 2024
	 */
    std::vector<GameObject *> getGameObjects(GameObjectType type) const;

    /**
     * @brief Gets the X length of the game map.
     * @return int: X length of the game map.
     * @author Grant Madson
     * @date SPRING 2024
     */
     static double getXLength();

    /**
    * @brief Gets the Z length of the game map.
    * @return int: Z length of the game map.
    * @author Grant Madson
    * @date SPRING 2024
    */
    static double getZLength();

    /**
	 * @return A constant iterator pointing to the beginning of the vector of GameObject pointers.
	 * @author Tyson Cox (implemented by Koda Koziol)
	 * @date SPRING 2024
	 */
    [[nodiscard]] std::vector<GameObject *>::const_iterator begin() const;

    /**
	 * @return A constant iterator pointing to the end of the vector of GameObject pointers.
	 * @author Tyson Cox (implemented by Koda Koziol)
	 * @date SPRING 2024
	 */
    [[nodiscard]] std::vector<GameObject *>::const_iterator end() const;

private:
    std::vector<GameObject *> objs;
    int nextFreeEntityID = 0;
    bool isPaused = false;
    static inline double MapXLength = 25.0;
    static inline double MapZLength = 25.0;

    /**
    * @brief Constructor for the Scene.
    * @author Koda Koziol
    * @date SPRING 2024
    */
    Scene();

    /**
	* @brief Destructor for the Scene. This will delete all GameObjects in the Scene.
	* @author Koda Koziol
	* @date SPRING 2024
	*/
    ~Scene();

    static Scene *instance;
};

#endif // SCENE_H
