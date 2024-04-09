//
// Created by Grant Madson on 3/4/2024.
//

#include "EnemyTank.h"
#include "gamestate.h"
#include "glm/geometric.hpp"
#include "PlayerTank.h"
#include "Projectile.h"

void EnemyTank::doUpdate(float deltaTime) {

    this->direction = glm::normalize(glm::vec3(cos(angleInRadians), 0.0, sin(angleInRadians)));
    PlayerTank* player = dynamic_cast<PlayerTank*>(GameState::getInstance()->getGameObject(GameObjectType::PlayerTank));
    auto playerPos = player->getPosition();

    float desiredAngle = atan2(playerPos[2] - position[2], playerPos[0] - position[0]);

    if (desiredAngle  > angleInRadians) {
        angleInRadians += speed * deltaTime;
    }
    else {
        angleInRadians -= speed * deltaTime;
    }

    if (abs(desiredAngle - angleInRadians) < qDegreesToRadians(90)) {
        position += direction * speed * deltaTime;
    }

    shotAccumulator += deltaTime;
    shoot(this->direction);
}

void EnemyTank::shoot(glm::vec3 direction) {
    if (shotAccumulator < shotThreshold) {
        return;
    }

    shotAccumulator = 0.0;
    GameState* gamestate = GameState::getInstance();

    // Don't spawn the bullet right on top of us
    auto bulletPos = this->position + this->direction;
    auto bulletDir = this->direction;
    auto bulletSize = 1.0f;

    auto bullet = new Projectile(nullptr, gamestate->getNextFreeEntityID(), bulletPos, bulletSize, bulletDir);

    gamestate->addObject(bullet);
}

EnemyTank::EnemyTank(uint32_t entityID, const vec3& position, const vec3& direction, QObject* parent)
: Tank(GameObjectType::EnemyTank, entityID, position, direction, parent),
shotAccumulator(0),
shotThreshold(10)
{
    speed = 0.5;
}
