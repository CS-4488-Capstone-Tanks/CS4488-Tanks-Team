#include "tank.h"

Tank::Tank(GameObjectType type) : GameObject(type) {}
PlayerTank::PlayerTank() : Tank(GameObjectType::PlayerTank) {
    direction = vec3(0, 0, -1);
}

EnemyTank::EnemyTank() : Tank(GameObjectType::EnemyTank) {}

void PlayerTank::doUpdate(float deltaTime) {
    static float accum = 0.0;
    accum += 0.1 * deltaTime;

    direction = vec3(cos(accum), 0.0f, sin(accum));
}
