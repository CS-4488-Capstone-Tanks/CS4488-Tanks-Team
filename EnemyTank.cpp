//
// Created by Grant Madson on 3/4/2024.
//

#include "EnemyTank.h"
#include "gamestate.h"
#include "glm/geometric.hpp"
#include "PlayerTank.h"
#include "Projectile.h"

void EnemyTank::doUpdate(float deltaTime) {

    vec pos = this->getPosition();
    float spd = this->getSpeed();

    
    vec3 dir = glm::normalize(glm::vec3(cos(angleInRadians), 0.0, sin(angleInRadians)));
    this->setDirection(dir);
    PlayerTank* player = dynamic_cast<PlayerTank*>(GameState::getInstance()->getGameObject(GameObjectType::PlayerTank));
    auto playerPos = player->getPosition();

    float desiredAngle = atan2(playerPos[2] - pos[2], playerPos[0] - pos[0]);

    if (desiredAngle  > angleInRadians) {
        angleInRadians += spd * deltaTime;
    }
    else {
        angleInRadians -= spd * deltaTime;
    }

    if (abs(desiredAngle - angleInRadians) < qDegreesToRadians(90)) {
        pos += dir * spd * deltaTime;
        this->setPosition(pos);
    }

    shotAccumulator += deltaTime;
    shoot(dir);
}

void EnemyTank::shoot(glm::vec3 direction) {
    if (shotAccumulator < shotThreshold) {
        return;
    }

    shotAccumulator = 0.0;
    GameState* gamestate = GameState::getInstance();

    // Don't spawn the bullet right on top of us
    auto bulletPos = this->getPosition() + this->getDirection();
    auto bulletDir = this->getDirection();
    auto bulletSize = 1.0f;

    auto bullet = new Projectile(nullptr, gamestate->getNextFreeEntityID(), bulletPos, bulletSize, bulletDir, this->getType());

    gamestate->addObject(bullet);
}

EnemyTank::EnemyTank(uint32_t entityID, const vec3& position, const vec3& direction, QObject* parent)
: Tank(GameObjectType::EnemyTank, entityID, position, direction, parent),
shotAccumulator(0),
shotThreshold(10)
{
    this->setSpeed(0.5);
}
