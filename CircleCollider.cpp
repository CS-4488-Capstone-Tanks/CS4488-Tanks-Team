//
// Created by Parker on 2/23/2024.
//

#include "CircleCollider.h"
#include <stdexcept>

// Note! This file uses experimental features of GLM, so you either have to not update
// GLM, or if you do and it breaks the distance2 function, you'll have to change the code to make it work
#define GLM_ENABLE_EXPERIMENTAL

//for distance2 function
#include <glm/gtx/norm.hpp>

//Constructor with initial position and radius. Throws an exception if the radius is not positive.
CircleCollider::CircleCollider(const glm::vec3& position, float radius)
        : colliderPosition(position), colliderRadius(radius) {
    if (radius <= 0.0f) {
        throw std::invalid_argument("Radius must be positive.");
    }
}

//Update method for collider position
void CircleCollider::updatePosition(const glm::vec3& newPosition) {
    colliderPosition = newPosition;
}

//Checks collision with another CircleCollider
bool CircleCollider::collidesWith(const CircleCollider& other) const {
    float distanceSquared = glm::distance2(colliderPosition, other.colliderPosition);
    float radiusSumSquared = (colliderRadius + other.colliderRadius) * (colliderRadius + other.colliderRadius);
    return distanceSquared <= radiusSumSquared;
}

//Checks if a point is inside the collider's area
bool CircleCollider::containsPoint(const glm::vec3& point) const {
    return glm::distance2(colliderPosition, point) <= (colliderRadius * colliderRadius);
}

//Returns the position of the collider
glm::vec3 CircleCollider::getPosition() const {
    return colliderPosition;
}

// Returns the radius of the collider
float CircleCollider::getRadius() const {
    return colliderRadius;
}