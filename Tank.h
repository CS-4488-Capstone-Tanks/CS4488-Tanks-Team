//
// Created by Grant Madson on 3/4/2024.
//

#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "gameobject.h"

class Tank : public GameObject {
Q_OBJECT
public:
    explicit Tank(QObject *parent = nullptr): GameObject(parent) {
        // Initialize player tank position, speed, etc.
        // Connect key events to handleKeyPress() slot
    };

    virtual void shoot(vec3 position, vec3 angle) = 0;
    void doUpdate(float deltaTime) override {};
    void shoot(vec3 direction); //TODO spawn bullet outside collision box in direction tank is pointing (or mouse? unsure)

    ~Tank() override = default;

protected:
    int cooldown = 8; //Cooldown for firing is 8 seconds as per requirements.
    const int MAX_COOLDOWN = 8;
};
#endif //TANKS_TANK_H
