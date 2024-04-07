//
// Created by Grant Madson on 3/4/2024.
//

#include "EnemyTank.h"
#include "glm/geometric.hpp"

EnemyTank::EnemyTank() {

}

void EnemyTank::doUpdate(float deltaTime) {

    //TODO have Tank face direction of PlayerTank

    // Calculate the displacement vector based on speed, direction, and time
    // Normalized so that the magnitude of the direction vector is always 1.
    vec3 dir = this->getDirection();
    vec3 pos = this->getPosition();
    float spd = this->getSpeed();

    glm::vec3 displacement = glm::normalize(dir) * spd * deltaTime;

    pos += displacement;
    this->setPosition(pos);

    this->setSpeed(0.0);

    this->shoot(dir);
}

void EnemyTank::shoot(glm::vec3 direction) {
    if (canShoot){
        canShoot = false;
        //shotTimer->start(this->MAX_COOLDOWN);
        //TODO spawn and add projectile to gamestate
    }
}
