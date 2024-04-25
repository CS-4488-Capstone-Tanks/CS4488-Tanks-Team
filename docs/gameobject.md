# GameObject

_Primary Author: Koda Koziol, SPRING 2024_

The GameObject class is the base class for all objects in the [Scene](scene.md). It holds the properties that all objects have, such as position and direction. It also has methods for updating the object's state and for initializing the object.

## Types of GameObjects

The GameObject class has an enum called `GameObjectType` that defines the different types of GameObjects. The following types are currently defined:

- `PlayerTank` - This type is used for the player GameObject in the Scene. The player GameObject is the GameObject that the player controls.

- `EnemyTank` - This type is used for enemy GameObjects in the Scene. Enemy GameObjects are controlled by the AI.

- `Obstacle` - This type is used for obstacles in the Scene. Obstacles are GameObjects that the player and enemy GameObjects cannot pass through.

- `PlayerProjectile` - This type is used for projectiles fired by the player GameObject.

- `EnemyProjectile` - This type is used for projectiles fired by enemy GameObjects.

- `None` - This is the default type for a GameObject. It should probably not be used for any GameObjects in the Scene.

These types are used to identify the type of GameObject in the Scene for determining whether to trigger a collision. The type is also used by the renderer to determine what model to use for the GameObject.

## Creating a GameObject

The GameObject constructor takes the following parameters:

- `GameObjectType type` - The type of the GameObject. This is used to identify the GameObject in the Scene. It's also needed for the renderer to know what model to use for the GameObject.

- `uint32_t entityID` - The entityID of the GameObject. This should be a unique identifier for the GameObject in the Scene. To get a unique entityID, use the `getNextFreeEntityID()` method of the Scene.

- `glm::vec3 position` - The position of the GameObject in the Scene.

- `glm::vec3 direction` - The direction the GameObject is facing. Note this cannot be (0, 0, 0), and it should not be confused with velocity. Also note that whatever argument is passed in will be normalized.

## Destroying a GameObject

From the Scene, you can call `removeObject(entityID)` to remove a GameObject from the Scene. This will also delete the GameObject from memory. From the GameObject itself, you can call `selfDestruct()` which will queue the GameObject for destruction by the Scene before the next update. You can check if a GameObject is queued for destruction by calling `isQueuedForDestruction()`.

## The GameObject Loop

The GameObject loop is driven by the Scene object that the GameObject is a part of. The Scene object will call the `start()` method of the GameObject once at the start of the scene, and then call the `update()` method regularly (usually every frame) for the rest of the lifetime of the GameObject.

It's important to know that the Scene only keeps track of each GameObject's position and whether it is colliding with another GameObject. Movement and collision handling are the responsibility of each GameObject itself. In other words, the Scene has no sense of velocity or acceleration. Despite the GameObject's `speed` property, the Scene is only aware of the GameObject's position, so it's up to each GameObject to update its own position appropriately based on its speed and direction. As for collisions, the Scene will detect and trigger a collision with the `doCollision()` method, but what happens when a collision occurs is also left up to each GameObject itself. Right now, most GameObjects will just self-destruct when they collide with another GameObject.

Note that as of writing, GameObjects that are added to the Scene after the Scene has started (like projectiles) will not have their `start()` method called at all.

Also note that GameObjects should not call the methods in this section directly, these methods are called by the Scene object that the GameObject is a part of.

- `start()` - This method allows the GameObject to initialize itself before the scene begins updating. This method is called once at the start of the scene, by the Scene, before the first update Precisely, this method will call `doStart()` then set its collider's position to its position.

- `update(float deltaTime)` - This method updates the state of the GameObject. This method is called regularly (usually every frame) by the Scene. Precisely, this method will call `doUpdate()` then update the collider's position to its position if it has changed. The `deltaTime` parameter is the time in seconds since the last update call (i.e. current seconds per frame).

The following virtual methods should be overridden by derived classes:

- `doStart()` - This method should be overridden by derived classes to initialize the GameObject before the scene begins updating. This method is called by the `start()` method, before the first update. This method should be used to initialize the GameObject's state, such as setting the initial speed or direction of the GameObject.

- `doUpdate(float deltaTime)` - This method should be overridden by derived classes to update the state of the GameObject. This method is called by the `update()` method regularly (usually every frame). The `deltaTime` parameter is the time in seconds since the last update call (i.e. current seconds per frame). This method should be used to update the GameObject's state, such as moving the GameObject based on its speed and direction.

- `doCollision(GameObject* other)` - This method should be overridden by derived classes to handle collisions with other GameObjects. This method is called from the `update()` method in the Scene when a collision is detected between this GameObject and another GameObject. The `other` parameter is a pointer to the GameObject that this GameObject collided with, which can be used by the child class to determine how to handle the collision.

## Getting and Setting GameObject Properties

The following methods can be used to get and set the properties of a GameObject:

- `getPosition()` and `setPosition(glm::vec3 position)` - These methods get and set the position of the GameObject in 3D space.

- `getDirection()` and `setDirection(glm::vec3 direction)` - These methods get and set the direction the GameObject is facing. Note that the direction should not be (0, 0, 0), and it should not be confused with velocity. Also note that whatever argument is passed in will be normalized, so the direction will always have a magnitude of 1, no need to normalize it yourself.

- `getSpeed()` and `setSpeed(float speed)` - These methods get and set the speed property of the GameObject. Note that setting the speed will not change the actual velocity of the GameObject as displayed, because the Scene only cares about the GameObject's position. You must manually update the GameObject's position based on its speed and direction in the `doUpdate()` method.

- `getType()` - This method returns the type of the GameObject.

- `getEntityID()` - This method returns the (should be unique) entityID of the GameObject.

- `getCollider()` - This method returns a pointer to the collider of the GameObject. The collider is used by the Scene to check for collisions between GameObjects.

- `isQueuedForDestruction()` - This method returns true if the GameObject is queued for destruction by the Scene.

- `selfDestruct()` - This method queues the GameObject for destruction by the Scene before the next update.

- `hasChanged()` - This method returns true if the GameObject's position has changed since the last update. This is used by the Scene to determine if the GameObject's collider needs to be updated.

- `resetChanged()` - This method resets the flag that indicates whether the GameObject's position has changed. This is called by the Scene after updating the GameObject's collider.


## Potential Improvements for the future
- For GameObjects added after Scene start, call `start()` on them after they are added to the Scene and before they are first updated.
- Remove the `speed` property from the GameObject base class. Not all GameObjects will have a speed, and having a `setSpeed()` method on the base class is misleading when whether and how the GameObject actually moves is up to the derived class. Non-intrinsic properties and methods should be left to derived classes to handle if they need it.