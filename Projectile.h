//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "gameobject.h"
#include "CircleCollider.h"
#include <cstdint>
#include <QObject>
#include <glm/vec3.hpp>


/**
 * @brief Represents a moving projectile in the game
 * Inherits from GameObject and has a lifespan, direction, speed, and collider for collision detection.
 * It moves in a normalized direction at a specified speed.
 * Lifetime is decremented each frame and the projectile is removed when it expires.
 * Collision detection is managed through a CircleCollider.
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
     * @param colliderRadius Radius of the projectile's collider.
     * @param direction Initial normalized direction vector of the projectile.
     * @exception std::invalid_argument Thrown if direction is a zero vector or speed is not positive.
     * @author Parker Hyde
     * @date SPRING 2024
     */

    explicit Projectile(QObject *parent = nullptr, uint32_t entityID = 0, const glm::vec3& position = glm::vec3(0.0f), float colliderRadius = 1.0f, const glm::vec3& direction = glm::vec3(0, 0, -1));

    /**
     * @brief Initialization logic for the projectile
     * Currently empty as projectiles do not require initialization before the first update.
     * Can be overridden for specialized projectiles that require setup.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void doStart() override;

    /**
     * @brief Updates the projectile's state each frame
     * Moves the projectile based on its normalized direction and speed.
     * Updates the collider's position and decrements the projectile's lifetime.
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
     * @brief Sets the projectile's normalized direction vector
     * @param dir New normalized direction vector for the projectile.
     * @exception std::invalid_argument Thrown if dir is a zero vector.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void setDirection(const glm::vec3& dir);

    /**
     * @brief Sets the projectile's movement speed
     * @param spd The new speed of the projectile, must be positive.
     * @exception std::invalid_argument Thrown if spd is not positive.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void setSpeed(float spd);

    /**
     * @brief Retrieves the current speed of the projectile
     * @return The speed of the projectile as a scalar value.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    float getSpeed() const;

    /**
     * @brief Calculates and retrieves the current velocity of the projectile
     * Velocity is computed as the product of the normalized direction vector and the speed scalar.
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
    //The remaining lifetime of the projectile
    float lifetime;
    //The collider used for detecting collisions with other objects
    CircleCollider collider;
};

#endif //TANKS_PROJECTILE_H
