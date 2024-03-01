//
// Created by Parker on 2/23/2024.
//

#ifndef TANKS_PROJECTILE_H
#define TANKS_PROJECTILE_H

#include "gameobject.h"
#include "CircleCollider.h"
#include <cstdint>
#include <QObject>

//Projectile class represents a moving game object that has a lifespan and can collide with other game objects
class Projectile : public gameobject {
    Q_OBJECT

public:
    //Constructor for Projectile. Sets up default values and initial state
    explicit Projectile(QObject *parent = nullptr, uint32_t entityID = 0);

    void doStart() override;
    void doUpdate(float deltaTime) override;
    bool isDead() const;

    //Getter and setter methods
    void setVelocity(const glm::vec3& vel);
    void setSpeed(float spd);
    glm::vec3 getVelocity() const;
    CircleCollider getCollider() const;

private:
    glm::vec3 velocity;
    //Lifetime of projectile
    float lifetime;
    CircleCollider collider;
};


#endif //TANKS_PROJECTILE_H
