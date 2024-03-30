//
// Created by Grant Madson on 3/4/2024.
//
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <QDataStream>
#include "PlayerTank.h"
#include "glm/ext/matrix_transform.inl"

explicit PlayerTank::PlayerTank(){
    shotTimer = new QTimer(this);
    connect(shotTimer, &QTimer::timeout, this, &PlayerTank::allowShot);
}


void PlayerTank::doUpdate(float deltaTime) {

    // Calculate the displacement vector based on speed, direction, and time
    // Normalized so that the magnitude of the direction vector is always 1.
    glm::vec3 displacement = glm::normalize(direction) * speed * deltaTime;

    this->position += displacement;

    this -> speed = 0;
}

void PlayerTank::moveForward() {
    this->speed = 1.0;
}

void PlayerTank::turnRight() {
    // Angle in radians to rotate by
    float angleRadians = glm::radians(45.0f);

    // Create rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    // Rotate the vector
    this->direction = glm::vec3(rotationMatrix * glm::vec4(this->direction, 1.0f));
}

void PlayerTank::turnLeft() {
    // Angle in radians to rotate by
    float angleRadians = glm::radians(-45.0f);

    // Create rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    // Rotate the vector
    this->direction = glm::vec3(rotationMatrix * glm::vec4(this->direction, 1.0f));
}

void PlayerTank::moveBackward() {
    this->speed = -1.0;
}

void PlayerTank::shoot(glm::vec3 direction) {
    if (canShoot){
        canShoot = false;
        shotTimer->start(this->MAX_COOLDOWN);
        //TODO spawn and add projectile to gamestate
    }
}