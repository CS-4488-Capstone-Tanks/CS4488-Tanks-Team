//
// Created by Parker on 2/23/2024.
//

#include "Projectile.h"
#include <stdexcept>
#include <glm/glm.hpp>

Projectile::Projectile(QObject* parent, uint32_t entityID, const vec3& position, float colliderRadius, const vec3& direction) :
    GameObject(GameObjectType::Projectile, entityID, position, direction, parent),
    lifetime(10.0f),
    collider(position, colliderRadius)
{
    this->speed = 5.0f;
}

//Empty since projectiles shouldn't need initialization before the first update
void Projectile::doStart() {
    //Initialization logic for projectile
}

//Updates the projectile's state. Called once per frame
void Projectile::doUpdate(float deltaTime) {
    if (!isDead()) {
        //Move the projectile based on velocity and speed
        position += direction * speed * deltaTime;
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

//Sets the projectile's speed. Throws an exception if the speed is not positive
void Projectile::setSpeed(float spd) {
    if (spd <= 0) {
        throw std::invalid_argument("Projectile speed must be positive.");
    }
    speed = spd;
}

//Gets the collider used for collision detection
CircleCollider Projectile::getCollider() const {
    return collider;
}

void Projectile::setDirection(const vec3& dir) {
    direction = dir;
}

float Projectile::getSpeed() const {
    return speed;
}

glm::vec3 Projectile::getVelocity() const {
    return glm::normalize(direction) * speed;
}


