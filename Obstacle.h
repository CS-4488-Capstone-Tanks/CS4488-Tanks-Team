//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_OBSTACLE_H
#define TANKS_OBSTACLE_H

#include "GameObject.h"
#include "CircleCollider.h"
#include <QObject>

//The Obstacle class is a static game entity that can't move but can be collided with
//It inherits from GameObject and has a collider to handle collisions
class Obstacle : public GameObject {
Q_OBJECT

public:
    //Constructor sets up the obstacle with a parent, entity ID, position, and collider radius
    explicit Obstacle(QObject *parent = nullptr, uint32_t entityID = 0, const glm::vec3& position = glm::vec3(0.0f), float colliderRadius = 1.0f);

    //doStart is called before the game starts, so it can be used for initialization
    void doStart() override;

    //doUpdate is called each frame, but since obstacles are static they may not need to update
    void doUpdate(float deltaTime) override;

    //Returns the collider for collision detection
    CircleCollider getCollider() const;

private:
    //The collider used for detecting collisions with other objects
    CircleCollider collider;
};

#endif //TANKS_OBSTACLE_H
