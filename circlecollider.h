//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_CIRCLECOLLIDER_H
#define TANKS_CIRCLECOLLIDER_H


#include <glm/vec3.hpp>

//The CircleCollider class represents a simple circular boundary that can be used for collision detection
class CircleCollider {
public:
    //Constructor that initializes a collider with a position and radius
    CircleCollider(const glm::vec3& position, float radius);

    //Updates the position of the collider to a new position
    void updatePosition(const glm::vec3& newPosition);
    //Checks if this collider intersects with another collider
    bool collidesWith(const CircleCollider& other) const;
    //Determines if a point is within the bounds of the collider
    bool containsPoint(const glm::vec3& point) const;

    //Retrieves the current position of the collider
    glm::vec3 getPosition() const;

    //Retrieves the radius of the collider
    float getRadius() const;

private:
    //The current position of the collider
    glm::vec3 colliderPosition;
    //The radius of the collider
    float colliderRadius;
};



#endif //TANKS_CIRCLECOLLIDER_H
