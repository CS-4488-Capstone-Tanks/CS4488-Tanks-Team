//
// Created by Grant Madson on 3/4/2024.
//
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <QDataStream>
#include "PlayerTank.h"
#include "glm/ext/matrix_transform.inl"

PlayerTank::PlayerTank(QObject *parent): Tank(parent, GameObjectType::PlayerTank){
    //shotTimer = new QTimer(this);
    //connect(shotTimer, &QTimer::timeout, this, &PlayerTank::allowShot);
}


void PlayerTank::doUpdate(float deltaTime) {

    // Calculate the displacement vector based on speed, direction, and time
    // Normalized so that the magnitude of the direction vector is always 1.
    vec3 pos = this->getPosition();
    vec3 dir = this->getDirection();
    float spd = this->getSpeed();
    glm::vec3 displacement = dir * spd * deltaTime;

    pos += displacement;
    this->setPosition(pos);

    this->setSpeed(0.0);
}

void PlayerTank::moveForward() {
    this->setSpeed(1.0);
}

void PlayerTank::turnRight() {
    // Angle in radians to rotate by
    float angleRadians = glm::radians(45.0f);

    // Create rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    // Rotate the vector
    vec3 dir = this->getDirection();
    this->setDirection(glm::vec3(rotationMatrix * glm::vec4(dir, 1.0f)));
}

void PlayerTank::turnLeft() {
    // Angle in radians to rotate by
    float angleRadians = glm::radians(-45.0f);

    // Create rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

    // Rotate the vector
    vec3 dir = this->getDirection();
    this->setDirection(glm::vec3(rotationMatrix * glm::vec4(dir, 1.0f)));
}

void PlayerTank::moveBackward() {
    this->setSpeed(-1.0);
}

void PlayerTank::shoot(glm::vec3 direction) {
    if (canShoot){
        canShoot = false;
        //shotTimer->start(this->MAX_COOLDOWN);
        //TODO spawn and add projectile to gamestate
    }
}