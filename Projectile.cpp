//
// Created by Parker on 2/23/2024.
//

#include "Projectile.h"
#include <glm/glm.hpp>

const float COLLIDER_RADIUS = 0.05f;

Projectile::Projectile(uint32_t entityID, const vec3& position, const vec3& direction, GameObjectType type) :
    GameObject(type, entityID, position, direction),
    lifetime(10.0f)
{
    this->setSpeed(5.0f);
    this->collider = CircleCollider(COLLIDER_RADIUS);
}

//Empty since projectiles shouldn't need initialization before the first update
void Projectile::doStart() {
    //Initialization logic for projectile
}

//Updates the projectile's state. Called once per frame
void Projectile::doUpdate(float deltaTime) {
    if (!isDead()) {
        //Move the projectile based on velocity and speed
        vec3 pos = getPosition();
        pos += this->getDirection() * this->getSpeed() * deltaTime;
        setPosition(pos);
        lifetime -= deltaTime;
    }
    else
        selfDestruct();
}

//Called when the projectile collides with another GameObject
void Projectile::doCollision(GameObject* other) {
    // *cool explosion effects and noises*
    selfDestruct();
}

//Checks whether the projectile's lifetime has run out
bool Projectile::isDead() const {
    return lifetime <= 0.0f;
}

glm::vec3 Projectile::getVelocity() const {
    return getDirection() * getSpeed();
}


