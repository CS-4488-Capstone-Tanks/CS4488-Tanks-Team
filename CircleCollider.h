//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_CIRCLECOLLIDER_H
#define TANKS_CIRCLECOLLIDER_H


#include <glm/vec3.hpp>

/**
 * @brief The CircleCollider class represents a simple circular boundary that can be used for collision detection.
 * @date SPRING 2024
 */
class CircleCollider {
public:
    /**
     * @brief Constructor that initializes a collider with a position and radius.
     * @param position The initial position of the collider in 3D space.
     * @param radius The radius of the collider.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    CircleCollider(float radius);


    /**
     * @brief Updates the position of the collider to a new position.
     * @param newPosition The new position of the collider in 3D space.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    void updatePosition(const glm::vec3& newPosition);

    /**
     * @brief Checks if this collider intersects with another collider.
     * @param other The other CircleCollider to check collision against.
     * @return True if there is an intersection, false otherwise.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    bool collidesWith(const CircleCollider& other) const;

    /**
     * @brief Determines if a point is within the bounds of the collider.
     * @param point The point in 3D space to check.
     * @return True if the point is within the collider, false otherwise.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    bool containsPoint(const glm::vec3& point) const;

    /**
     * @brief Retrieves the current position of the collider.
     * @return The position of the collider in 3D space.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    glm::vec3 getPosition() const;

    /**
     * @brief Retrieves the radius of the collider.
     * @return The radius of the collider.
     * @author Parker Hyde
     * @date SPRING 2024
     */
    float getRadius() const;

private:
    //The current position of the collider
    glm::vec3 colliderPosition;
    //The radius of the collider
    float colliderRadius;
};



#endif //TANKS_CIRCLECOLLIDER_H
