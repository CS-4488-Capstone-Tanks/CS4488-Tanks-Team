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
    glm::vec3 displacement = glm::normalize(direction) * speed * deltaTime;

    this->position += displacement;

    this -> speed = 0;

    this->shoot(direction);
}

void EnemyTank::shoot(glm::vec3 direction) {
    if (canShoot){
        canShoot = false;
        //shotTimer->start(this->MAX_COOLDOWN);
        //TODO spawn and add projectile to gamestate
    }
}
