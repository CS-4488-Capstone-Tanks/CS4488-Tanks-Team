#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "gameobject.h"

class Tank : public GameObject {
protected:
    int cooldown;
    Tank(GameObjectType type);
};

class PlayerTank : public Tank {
public:
    PlayerTank();
    void doUpdate(float deltaTime) override;
};

class EnemyTank : public Tank {
public:
    EnemyTank();
};

#endif //TANKS_TANK_H
