//
// Created by Grant Madson on 3/4/2024.
//

#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "gameobject.h"

class Tank : public GameObject {
public:
    explicit Tank(
        GameObjectType type,
        uint32_t entityID,
        const vec3& position = vec3(0.0f),
        const vec3& direction = vec3(0.0f, 0.0f, -1.0f)
    );

    void doUpdate(float deltaTime) override {};

    virtual void shoot(vec3 direction); //TODO spawn bullet outside collision box in direction tank is pointing (or mouse? unsure)

protected:
    const int MAX_COOLDOWN = 8000; //Cooldown for firing is 8 seconds as per requirements. Must be in millis.
    bool canShoot = true;

    float angleInRadians;

protected:
    void allowShot() { canShoot = true; };

};
#endif //TANKS_TANK_H
