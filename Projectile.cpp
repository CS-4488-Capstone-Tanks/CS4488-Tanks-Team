//
// Created by Parker on 2/23/2024.
//

#include "Projectile.h"
#include <stdexcept>

//Constructor for the Projectile class. Initializes entity ID, velocity, lifetime, and collider
Projectile::Projectile(QObject *parent, uint32_t entityID)
        : gameobject(parent), velocity(glm::vec3(0.0f)), lifetime(10.0f), collider(glm::vec3(0.0f), 1.0f) // Default values as placeholders
{
    this->entityID = entityID;
}

//Initialization method for the projectile
void Projectile::doStart() {

}

//Update method for the projectile, called with each frame of the game loop
void Projectile::doUpdate(float deltaTime) {
    //Update the position based on velocity and deltaTime if the projectile is still active
    if (!isDead()) {
        //Move the projectile
        position += velocity * speed * deltaTime;
        //Update collider position
        collider.updatePosition(position);
        //Reduce lifetime each update
        lifetime -= deltaTime;
    }
}

//Check if the projectile's lifetime has expired
bool Projectile::isDead() const {
    return lifetime <= 0.0f;
}

//Sets the projectile's velocity
void Projectile::setVelocity(const glm::vec3& vel) {
    velocity = vel;
}

//Sets the projectile's speed. Throws an exception if the speed is non-positive
void Projectile::setSpeed(float spd) {
    if (spd <= 0) {
        throw std::invalid_argument("Projectile speed must be positive.");
    }
    speed = spd;
}

//Retrieves the projectile's velocity
glm::vec3 Projectile::getVelocity() const {
    return velocity;
}

//Returns the collider associated with the projectile
CircleCollider Projectile::getCollider() const {
    return collider;
}
