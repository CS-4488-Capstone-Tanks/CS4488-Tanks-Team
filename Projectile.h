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

    explicit Projectile(
        QObject *parent = nullptr,
        uint32_t entityID = 0,
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& direction = glm::vec3(0, 0, -1),
        GameObjectType type = GameObjectType::None
        );

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
     * @brief Called when the projectile collides with another GameObject
     * Currently just destroys the projectile, but we can get fancier in the future.
     * @param other The GameObject that the projectile collided with.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    void doCollision(GameObject* other) override;

    /**
     * @brief Determines if the projectile's lifetime has expired.
     * @return True if the projectile is dead (lifetime <= 0), otherwise false.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    bool isDead() const;

    /**
     * @brief Calculates and retrieves the current velocity of the projectile
     * Velocity is computed as the product of the normalized direction vector and the speed scalar.
     * @return The velocity vector of the projectile.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    glm::vec3 getVelocity() const;

private:
    //The remaining lifetime of the projectile
    float lifetime;
};

#endif //TANKS_PROJECTILE_H
