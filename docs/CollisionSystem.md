### Collision System

_Primary Authors: Parker Hyde, Koda Koziol, SPRING 2024_

#### Overview

The collider system is used for managing interactions between game entities such as tanks, projectiles, and static obstacles. It ensures that collisions are detected and handled appropriately. The class used for collision detection is `CircleCollider`.

#### Core Components

1. **CircleCollider**
   - **Description**: Represents a simple circular boundary for collision detection.
   - **Usage**:
     - Attached to game entities like tanks and projectiles.
     - Used to check for intersections and containment of points within its boundary.
   - **Methods**:
     - `collidesWith(const CircleCollider&)`: Determines if two colliders intersect.
     - `containsPoint(const glm::vec3&)`: Checks if a point is within the collider's boundary.

For more detailed information, see the document on the CircleCollider class itself.

2. **CollisionMatrix**
   - **Description**: Manages rules for which game object types can collide with each other.
   - **Usage**:
     - Used to quickly determine if two objects should check for collisions based on their types.
	 


#### Classes Using Colliders

1. **GameObject**
   - Base class for all objects in the game.
   - Manages a `CircleCollider` instance.
   - Collision detection triggered during game updates.

2. **Tank (PlayerTank and EnemyTank)**
   - Inherits `GameObject`.
   - Uses `CircleCollider` for detecting collisions with other tanks, projectiles, and obstacles.
   - Specific reactions and behaviors upon collisions (e.g., taking damage or self destruction).

3. **Projectile**
   - Inherits `GameObject`.
   - Uses `CircleCollider` to detect collisions with tanks and obstacles.
   - On collision, triggers effects like explosions and then destroys itself.

4. **Obstacle**
   - Static game entities.
   - Uses `CircleCollider` to interact with tanks and projectiles.
   - Cab trigger game logic changes (e.g., blocking movement or causing damage).

#### Collision Detection Process

1. **Update Loop**
   - Each game object updates its `CircleCollider` position based on its movement.
   - Collision checks are performed between objects if their types are set to interact in the `CollisionMatrix`.

2. **Handling Collisions**
   - When a collision is detected, the `doCollision(GameObject*)` method is triggered on both objects.
   - Each object handles the collision based on its specific game logic.

#### Workflow

1. Make sure that each game object:
   - Has a `CircleCollider` attached with the appropriate radius. As of writing, the collider radius property has no relation to the visual size of the object, so this may take some trial and error.
   - Has been set to the intended GameObjectType. Not only is this critical for rendering, but it also determines which other objects this object can collide with.

2. Modify the constructor of the `CollisionMatrix` in [`collisionMatrix.h`](../collisionMatrix.h) to define which GameObjectTypes can collide. There are a number of methods there to help with this.

3. Implement the `doCollision(GameObject*)` method in each game object class to handle specific collision interactions.

#### Potential Improvements

- Implement a base collider2D class, of which `CircleCollider` is a subclass, and extend it with new collider types like rectangles or polygons.

- 3D colliders (for sphere this should be easy)

- Figure out a better way to set the collision matrix. Maybe it can be loaded from a state file?

- Move beyond mere collision detection with realistic, physics-based collision responses (e.g., blocking, redirecting, bouncing, momentum transfer...).

