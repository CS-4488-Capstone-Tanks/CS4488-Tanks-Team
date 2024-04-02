//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "gameobject.h"
#include "CircleCollider.h"
#include <cstdint>
#include <QObject>


/**
 * @brief Represents a moving game entity that can interact with other game entities
 * Inherits from GameObject and has a lifespan, velocity, and collider for collision detection.
 * @author Parker Hyde
 * @date SPRING 2024
 */
class Projectile : public GameObject {
Q_OBJECT

public:
    /**
     * @brief Constructor for creating a projectile
     * @param parent Optional parent object.
     * @param entityID Unique identifier for the projectile.
     * @param position Initial position in 3D space.
     * @param velocity Initial velocity vector.
     * @param lifetime Time until the projectile expires.
     * @param colliderRadius Radius of the projectile's collider.
     * @param direction Facing direction of the projectile.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    explicit Projectile(QObject *parent = nullptr, uint32_t entityID = 0, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& velocity = glm::vec3(0.0f), float lifetime = 10.0f, float colliderRadius = 1.0f, const glm::vec3& direction = glm::vec3(0, 0, -1));


    /**
     * @brief Called before the game starts for initialization purposes.
     * Override to configure specific projectile behaviors or properties as needed.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void doStart() override;

    /**
     * @brief Updates the projectile's state each frame, handling movement and lifetime.
     * @param deltaTime Time elapsed since the last frame update, in seconds.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void doUpdate(float deltaTime) override;

    /**
     * @brief Determines if the projectile's lifetime has expired.
     * @return True if the projectile is dead (lifetime <= 0), otherwise false.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    bool isDead() const;

    /**
     * @brief Sets the projectile's velocity vector.
     * @param vel New velocity vector for the projectile.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void setVelocity(const glm::vec3& vel);
    /**
     * @brief Sets the projectile's movement speed.
     * Throws an exception if a non-positive speed is provided.
     * @param spd The new speed of the projectile, must be positive.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void setSpeed(float spd);
    /**
     * @brief Retrieves the current velocity of the projectile.
     * @return The velocity vector of the projectile.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    glm::vec3 getVelocity() const;

    /**
     * @brief Retrieves the collider used for collision detection.
     * @return A CircleCollider defining the projectile's collision boundaries.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    CircleCollider getCollider() const;

private:
    //The current velocity vector of the projectile
    glm::vec3 velocity;
    //The remaining lifetime of the projectile
    float lifetime;
    //The collider used for detecting collisions with other objects
    CircleCollider collider;
};

#endif //TANKS_PROJECTILE_H
