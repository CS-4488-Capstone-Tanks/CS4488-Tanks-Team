//
// Created by Parker on 2/23/2024.
//

#include "projectile.h"
#include <stdexcept>

//Constructor for the Projectile class
Projectile::Projectile(uint32_t entityID)
        : GameObject(GameObjectType::Projectile), velocity(glm::vec3(0.0f)), lifetime(10.0f), collider(glm::vec3(0.0f), 1.0f) {
    this->entityID = entityID;
    //Set the game object's type to Projectile
    this->type = GameObjectType::Projectile;
}

//Empty since projectiles shouldn't need initialization before the first update
void Projectile::doStart() {
    //Initialization logic for projectile
}

//Updates the projectile's state. Called once per frame
void Projectile::doUpdate(float deltaTime) {
    if (!isDead()) {
        //Move the projectile based on velocity and speed
        position += velocity * speed * deltaTime;
        //Update the collider's position to follow the projectile
        collider.updatePosition(position);
        //Reduce the lifetime of the projectile with each frame
        lifetime -= deltaTime;
    }
}

//Checks whether the projectile's lifetime has run out
bool Projectile::isDead() const {
    return lifetime <= 0.0f;
}

//Sets the projectile's velocity
void Projectile::setVelocity(const glm::vec3& vel) {
    velocity = vel;
}

//Sets the projectile's speed. Throws an exception if the speed is not positive
void Projectile::setSpeed(float spd) {
    if (spd <= 0) {
        throw std::invalid_argument("Projectile speed must be positive.");
    }
    speed = spd;
}

//Gets the projectile's current velocity
glm::vec3 Projectile::getVelocity() const {
    return velocity;
}

//Gets the collider used for collision detection
CircleCollider Projectile::getCollider() const {
    return collider;
}
