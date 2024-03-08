//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "GameObject.h"
#include "CircleCollider.h"
#include <cstdint>
#include <QObject>

//The Projectile class represents a moving game entity that can interact with other game entities
//It inherits from GameObject, and it has a lifespan, velocity, and collider for collision detection
class Projectile : public GameObject {
Q_OBJECT

public:
    //Constructor sets up the projectile with a parent, entity ID, default velocity, and collider
    explicit Projectile(QObject *parent = nullptr, uint32_t entityID = 0);

    //doStart is called before the game starts, so it can be used for initialization
    void doStart() override;

    //called each frame. Used to update the projectile's state such as moving it and reducing its lifetime
    void doUpdate(float deltaTime) override;

    //checks if the projectile's lifetime has expired
    bool isDead() const;

    //Setters and getters for the projectile's velocity and speed
    void setVelocity(const glm::vec3& vel);
    void setSpeed(float spd);
    glm::vec3 getVelocity() const;

    //Returns the collider for collision detection
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
