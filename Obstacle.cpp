//
// Created by Parker on 2/23/2024.
//

#include "Obstacle.h"


Obstacle::Obstacle(QObject *parent, uint32_t entityID, const glm::vec3& position, float colliderRadius, const glm::vec3& direction)
        : GameObject(GameObjectType::Obstacle, entityID, position, direction, parent), collider(position, colliderRadius) {
    // Additional initialization logic for Obstacle can be added here if necessary
}
/*
// Constructor updated to utilize the new GameObject constructor fully.
Obstacle::Obstacle(QObject *parent, uint32_t entityID, const glm::vec3& position, float colliderRadius)
        : GameObject(GameObjectType::Obstacle, entityID, position, glm::vec3(0.0f), parent),
          collider(position, colliderRadius) {
    // Additional initialization logic for Obstacle can be added here if necessary
}*/
/*
//Constructor initializing the obstacle with its position and collider
Obstacle::Obstacle(QObject *parent, uint32_t entityID, const glm::vec3& position, float colliderRadius)
        : GameObject(type, parent), collider(position, colliderRadius) {
    this->entityID = entityID;
    this->position = position;
    //Set the game object's type to Obstacle
    this->type = GameObjectType::Obstacle;
}
*/

void Obstacle::doStart() {
    //Method for initializing obstacle specific properties
}

void Obstacle::doUpdate(float deltaTime) {
    //Method for obstacle update logic. Not sure if necessary unless they can move, or are destructible
}


//Retrieves the collider used for collision detection
CircleCollider Obstacle::getCollider() const {
    return collider;
}