# CircleCollider

_Primary Author: Parker Hyde, SPRING 2024_

## Overview
The `CircleCollider` class currently uses a 3D vector (`glm::vec3`) to define position but operates primarily in two dimensions, using only the x and y components for collision detection. This makes it essentially a 2D collider in a 3D world.

## Implementation
The class defines basic collision detection capabilities such as determining if two circles overlap or if a point lies within a circle.

### Methods

The class offers several methods for managing collisions and collider properties:

```c++
// Constructor that initializes the collider with a specified radius
CircleCollider(float radius);

// Updates the collider's position to a new 3D coordinate
void updatePosition(const glm::vec3& newPosition);

// Checks if this collider intersects with another specified CircleCollider
bool collidesWith(const CircleCollider& other) const;

// Determines if a specific point in 3D space is within this collider
bool containsPoint(const glm::vec3& point) const;

// Retrieves the current position of the collider in 3D space
glm::vec3 getPosition() const;

// Retrieves the radius of the collider
float getRadius() const;
```

## Internal Details

The collider maintains its state through two main private member variables:

- `glm::vec3 colliderPosition`: Represents the current position of the collider in 3D space.
- `float colliderRadius`: Stores the radius of the collider.

Upon initialization, the constructor sets the radius of the collider and throws an exception if the radius is non-positive, ensuring the collider is always valid.

### Collision Detection Logic

Collision detection between two `CircleCollider` instances involves the following steps:

1. **Distance Calculation**: Compute the squared distance between the centers of the colliders using GLM's `distance2` function, which avoids the computational cost of extracting the square root, increasing performance.
2. **Radius Comparison**: Calculate the squared sum of the radii of both colliders.
3. **Collision Check**: If the squared distance is less than or equal to the squared radii sum, a collision is detected.

Using distance2 increases performance when dealing with a large number of collisions in real-time, at the cost of using an experimental glm feature. For this implementation this isn't completely necessary since there shouldn't be too many collisions happening at once, so it can be changed to avoid using experimental features if necessary.

### Position and Containment Checks

The collider's position can be updated at any time with a new vector in 3D space. Checking whether a point is within the collider involves calculating the squared distance from the point to the collider's center and comparing it to the squared radius of the collider.

## Other Notes

- The class uses features from the `glm/gtx/norm.hpp` library, specifically for the `distance2` function which might require enabling experimental features in GLM. If GLM changes things in the future, this may work differently.
- Exception handling is used to ensure the collider is created with a valid, positive radius.

## Potential Future Enhancements: Extending to SphereColliders

### Transforming to 3D Collisions
To evolve the game's collision detection to a fully 3-dimensional system, the following changes could be implemented:

1. **Modify Collider Geometry**:
   - Adjust the collider to use the z-component in all calculations, effectively treating it as a sphere rather than a circle.

2. **Update Collision Detection**:
   - Revise the `collidesWith` and `containsPoint` methods to account for three-dimensional space, ensuring that the z-axis is considered in distance calculations.

3. **Integrate with Game Logic**:
   - Update game logic to incorporate the implications of 3D collision, such as interactions at different altitudes or depths.


