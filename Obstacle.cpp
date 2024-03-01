//
// Created by Parker on 2/23/2024.
//

#include "Obstacle.h"

//Constructor initializing the obstacle and its collider with the given parameters
Obstacle::Obstacle(QObject *parent, uint32_t entityID, const glm::vec3& position, float colliderRadius)
        : gameobject(parent), collider(CircleCollider(position, colliderRadius)) {
    this->entityID = entityID;
    this->position = position;
    //I'm assuming that the obstacles don't move, so its speed is zero
    this->speed = 0.0f;
}

//Function for initializing the obstacle
void Obstacle::doStart() {

}

//Function for updates if necessary
void Obstacle::doUpdate(float deltaTime) {

}

//Returns the collider associated with the obstacle
CircleCollider Obstacle::getCollider() const {
    return collider;
}
