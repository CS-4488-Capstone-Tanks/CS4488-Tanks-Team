//
// Created by Parker on 2/23/2024.
//

#include "Projectile.h"
#include <stdexcept>
#include <glm/glm.hpp>

//Constructor for Projectiles
Projectile::Projectile(QObject *parent, uint32_t entityID, const glm::vec3& position, float speed, float lifetime, float colliderRadius, const glm::vec3& direction)
        : GameObject(GameObjectType::Projectile, entityID, position, glm::normalize(direction), parent), speed(speed), lifetime(lifetime), collider(position, colliderRadius) {
    if (glm::length(direction) == 0.0f) {
        throw std::invalid_argument("Direction vector cannot be zero.");
    }
}

//Empty since projectiles shouldn't need initialization before the first update
void Projectile::doStart() {
    //Initialization logic for projectile
}

//Updates the projectile's state. Called once per frame
//Movement is based on normalized direction and speed
//Collider's position is updated and lifetime is reduced
void Projectile::doUpdate(float deltaTime) {
    if (!isDead()) {
        //Use normalized direction and speed to determine velocity
        vec3 pos = getPosition();
        pos += glm::normalize(direction) * speed * deltaTime;
        collider.updatePosition(pos);
        setPosition(pos);
        lifetime -= deltaTime;
    }
}


//Checks if the projectile's lifetime has run out
bool Projectile::isDead() const {
    return lifetime <= 0.0f;
}

//Sets the projectile's direction to a normalized version of the provided vector
//Throws an invalid_argument exception if the direction vector is zero
void Projectile::setDirection(const glm::vec3& dir) {
    if (glm::length(dir) == 0.0f) {
        throw std::invalid_argument("Direction vector cannot be zero.");
    }
    direction = glm::normalize(dir);
}

//Sets the projectile's speed. Throws an exception if the speed is not positive
void Projectile::setSpeed(float spd) {
    if (spd <= 0) {
        throw std::invalid_argument("Projectile speed must be positive.");
    }
    speed = spd;
}

//Gets the projectile's direction
glm::vec3 Projectile::getDirection() const {
    return direction;
}

//Gets the projectiles speed
float Projectile::getSpeed() const {
    return speed;
}

//Calculates velocity as direction multiplied by speed
glm::vec3 Projectile::getVelocity() const {

    return glm::normalize(direction) * speed;
}

//Gets the collider used for collision detection
CircleCollider Projectile::getCollider() const {
    return collider;
}
