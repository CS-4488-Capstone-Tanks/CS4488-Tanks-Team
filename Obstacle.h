//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_OBSTACLE_H
#define TANKS_OBSTACLE_H

#include "gameobject.h"
#include "CircleCollider.h"
#include <QObject>

/**
 * @brief The Obstacle class represents a static game entity that can't move but can be collided with.
 * It inherits from GameObject and has a collider to handle collisions.
 * @author Parker Hyde
 * @date SPRING 2024
 */
class Obstacle : public GameObject {
Q_OBJECT

public:
    /**
     * @brief Constructor for creating an Obstacle.
     * @param parent The QObject parent of this game object.
     * @param entityID The unique identifier for this game object.
     * @param position The initial position of this game object in the game world.
     * @param colliderRadius The radius of the collider for this obstacle.
     * @param direction The initial facing direction of this game object. Initialized to (0,0,-1)
     * @author Parker Hyde
     * @date SPRING 2024
     */
    explicit Obstacle(QObject *parent = nullptr, uint32_t entityID = 0, const glm::vec3& position = glm::vec3(0.0f), float colliderRadius = 1.0f, const glm::vec3& direction = glm::vec3(0, 0, -1));

    /**
     * @brief Called before the game starts, so it can be used for initialization
     * Override to set up any obstacle-specific properties or behaviors.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void doStart() override;

    /**
     * @brief Updates the obstacle each frame.
     * Override for any obstacles that might change position or direction, although they are typically static
     * @param deltaTime Time elapsed since the last frame update, in seconds.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void doUpdate(float deltaTime) override;

};

#endif //TANKS_OBSTACLE_H
