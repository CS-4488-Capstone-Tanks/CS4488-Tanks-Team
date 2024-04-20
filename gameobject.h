#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <glm/vec3.hpp>
#include "gameobjecttype.h"
#include "CircleCollider.h"

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
      * @param entityID: The Entity ID of the GameObject. This is a unique identifier
         for each GameObject in the game. It is used to differentiate between
         different GameObjects and is assigned by the Scene when the
         GameObject is added to the Scene. You should call Scene::getNextFreeEntityID()
         to get a new Entity ID.
      * @param position: The position of the GameObject in 3D space.
      * @param direction: The direction the GameObject is facing in 3D space.
      * @param parent: The parent of the GameObject.
      * @author Koda Koziol
      * @date SPRING 2024
      */
    GameObject(
      GameObjectType type,
      uint32_t entityID,
      const vec3& position = vec3(0.0f),
      const vec3& direction = vec3(0.0f, 0.0f, -1.0f),
      QObject *parent = nullptr
      );


	/**
	 * @brief initialize the GameObject state. This method is called by the Scene.
	 * @note This method should not be called directly. It is called by the Scene
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	virtual void start();

	/**
	 * @brief Update the GameObject state. This method is called by the Scene.
	 * @param deltaTime: The time elapsed since the last update in seconds(?). This is a fixed value.
	 * @note This method should not be called directly. It is called by the Scene
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	virtual void update(float deltaTime);


	/**
	 * @brief This method gets called when a collision is detected between this GameObject
	 * and another GameObject. Override this method to implement collision behavior.
	 * @param other: The GameObject that this GameObject collided with.
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	virtual void doCollision(GameObject* other);

	/**
	 * @return The CircleCollider of the GameObject. This is used for collision detection.
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	CircleCollider getCollider() const;

    /**
     * @return The position of the GameObject in 3D space.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    vec3 getPosition() const;

	/**
	 * @brief Set the position of the GameObject in 3D space.
	 * @param pos: The new position of the GameObject in 3D space.
	 * @author Koda Koziol
	 * @date SPRING 2024
	 */
	void setPosition(const vec3& pos);

    /**
     * @return The direction the GameObject is facing in 3D space.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    vec3 getDirection() const;

	/**
	 * @brief Set the normalized direction the GameObject is facing in 3D space.
	 * @param dir: The new direction the GameObject is facing in 3D space.
	 * @note The direction vector should be normalized. If it is not, it will be normalized
	 * before being set as the direction of the GameObject. If the direction vector is the zero
	 * vector, an invalid_argument exception will be thrown.
	 * @author Koda Koziol
	 * @date SPRING 2024
	 */
	void setDirection(const vec3& dir);

    /**
	 * @return The speed of the GameObject.
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
    float getSpeed() const;

	/**
	 * @brief Set the speed of the GameObject.
	 * @param spd: The new speed of the GameObject. Must be positive.
	 * @note If the speed is not positive, an invalid_argument exception will be thrown.
	 * @author Koda Koziol
	 * @date SPRING 2024
	 */
	void setSpeed(float spd);

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
        different GameObjects and is assigned by the Scene when the
        GameObject is added to the Scene.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    uint32_t getEntityID() const;

	/**
	 * @brief Queue the GameObject for destruction. This will mark the GameObject for
		destruction and it will be removed from the Scene at the next update.
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

	/**
	 * @return True if the GameObject has changed since the last update, false otherwise.
	 * @note This is used to determine whether it's necessary to perform collision calculations (TODO).
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	bool hasChanged() const;

	/**
	 * @brief Reset the "hasChanged" flag to false. This should be called after collision calculations
	 * @author Koda Koziol
	 * @date SPRING 2024
	*/
	void resetChanged();


protected:
	// The unique identifier for the GameObject
    uint32_t entityID = -1;
	// The type of the GameObject, will be None by default
    GameObjectType type = GameObjectType::None;
    // The collider for the GameObject, will have radius 1 b default
    CircleCollider collider = CircleCollider(1.0f);

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



private:
	// The position of the GameObject in 3D space
	vec3 position = vec3(0.0f);
	// The direction the GameObject is facing in 3D space, should be normalized (don't confuse with velocity)
	vec3 direction = vec3(0.0f, 0.0f, 1.0f);
	// The speed of the GameObject (don't confuse with velocity)
	float speed = 0.0f;
	// Flag for whether the GameObject is queued for destruction
	bool _isQueuedForDestruction = false;
	// Flag for whether the GameObject has changed since the last update
	bool _hasChanged = false;
};

#endif // GAMEOBJECT_H
