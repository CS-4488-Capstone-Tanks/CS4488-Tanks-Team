//
// Created by Grant Madson on 3/4/2024.
//

#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "gameobject.h"

class Tank : public GameObject {
Q_OBJECT
public:
    explicit Tank(QObject *parent = nullptr, GameObjectType type = GameObjectType::EnemyTank): GameObject(type, parent) {
        this->collider = CircleCollider(vec3(0,0,0), 1.0f);
        // Initialize player tank position, speed, etc.
        // Connect key events to handleKeyPress() slot
    };

    void doUpdate(float deltaTime) override {};

    virtual void shoot(vec3 direction); //TODO spawn bullet outside collision box in direction tank is pointing (or mouse? unsure)

    ~Tank() override = default;

protected:
    const int MAX_COOLDOWN = 8000; //Cooldown for firing is 8 seconds as per requirements. Must be in millis.
    bool canShoot = true;

protected slots:
    void allowShot() { canShoot = true; };

};
#endif //TANKS_TANK_H
