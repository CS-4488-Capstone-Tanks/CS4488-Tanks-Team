//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_OBSTACLE_H
#define TANKS_OBSTACLE_H


#include "gameobject.h"
#include <QObject>
#include "CircleCollider.h"

class Obstacle : public gameobject {
    Q_OBJECT

public:
    explicit Obstacle(QObject *parent = nullptr, uint32_t entityID = 0, const glm::vec3& position = glm::vec3(0.0f), float colliderRadius = 1.0f);

    //Initial setup for the obstacle
    void doStart() override;

    //Update method for the obstacle, executed every frame
    void doUpdate(float deltaTime) override;

    //Getter for the obstacle's collider
    CircleCollider getCollider() const;

private:
    CircleCollider collider;
};


#endif //TANKS_OBSTACLE_H
