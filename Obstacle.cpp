//
// Created by Parker on 2/23/2024.
//

#include "Obstacle.h"

//Constructor for obstacles
Obstacle::Obstacle(QObject *parent, uint32_t entityID, const glm::vec3& position, float colliderRadius, const glm::vec3& direction)
        : GameObject(GameObjectType::Obstacle, entityID, position, direction, parent), collider(position, colliderRadius) {
}

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