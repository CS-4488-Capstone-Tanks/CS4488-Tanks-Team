#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <glm/vec3.hpp>
#include "gameobjecttype.h"

using namespace glm;


/**
 * @brief The GameObject class is the base class for all objects in the game.
    It holds the properties that all objects have, such as position and direction.
    It also has methods for updating the object's state and for initializing the object.
 * @author Koda Koziol
 * @date SPRING 2024
 */
class GameObject : public QObject
{
    Q_OBJECT
	
public:

    /**
     * @brief Constructor for the GameObject.
     * @param type: The GameObjectType of the GameObject. This is used so
        the renderer can differentiate between different types of GameObjects.
     * @param parent: The parent of the GameObject.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    explicit GameObject(GameObjectType type, QObject *parent = nullptr);


    /**
     * @brief This method gets called once immediately before the very first
        doUpdate() call. Override and initialize GameObject properties here.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    virtual void doStart();

    /**
     * @brief This method gets called once every update period on every GameObject.
        Override it to change position, direction, or other properties over time.
     * @param float deltaTime: The time elapsed since the last update in
        seconds(?). This is a fixed value. Example: If you increment a
        property every frame by a number, its "speed" will be measured in
        units-per-frame (or meters-per-frame if you wish). If you want to
        work in units-per-second, multiply "speed" by deltaTime to convert to
        units-per-frame.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    virtual void doUpdate(float deltaTime);

    /**
     * @return The position of the GameObject in 3D space.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    vec3 getPosition() const;

    /**
     * @return The direction the GameObject is facing in 3D space.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    vec3 getDirection() const;

    /**
     * @return The GameObjectType of the GameObject. This is used to
        differentiate between different types of GameObjects.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    GameObjectType getType() const;

    /**
     * @return The Entity ID of the GameObject. This is a unique identifier
        for each GameObject in the game. It is used to differentiate between
        different GameObjects and is assigned by the GameState when the
        GameObject is added to the GameState.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    uint32_t getEntityID() const;

    /**
     * @brief Set the Entity ID of the GameObject. This should only be called by the GameState
        when the GameObject is added to the GameState. It should not be called by the GameObject
        itself.
     * @param id: The Entity ID to set for the GameObject. This should be a unique identifier
        for each GameObject in the game.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    void setEntityID(int id);

	/**
	 * @brief Queue the GameObject for destruction. This will mark the GameObject for
		destruction and it will be removed from the GameState at the next update.
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	void selfDestruct();

	/**
	 * @return True if the GameObject is queued for destruction, false otherwise.
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	bool isQueuedForDestruction() const;


protected:
    vec3 position = vec3(0,0,0);
    vec3 direction = vec3(0,0,0);
    float speed = 0.0f;
    uint32_t entityID;
    GameObjectType type;


private:
	bool _isQueuedForDestruction = false;

};

#endif // GAMEOBJECT_H
