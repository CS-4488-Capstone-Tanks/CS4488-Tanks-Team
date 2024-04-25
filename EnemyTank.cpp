//
// Created by Grant Madson on 3/4/2024.
//

#include "EnemyTank.h"
#include "scene.h"
#include "game.h"
#include "glm/geometric.hpp"
#include "PlayerTank.h"
#include "Projectile.h"

void EnemyTank::doUpdate(float deltaTime) {

    vec pos = this->getPosition();
    float spd = this->getSpeed();

    
    vec3 dir = glm::normalize(glm::vec3(cos(angleInRadians), 0.0, sin(angleInRadians)));
    this->setDirection(dir);
    PlayerTank* player = dynamic_cast<PlayerTank*>(Scene::getInstance()->getGameObject(GameObjectType::PlayerTank));
    if (!player)
        return;
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

    if (spd > 0.0) {
        sfxManager->playSound(SFXManager::Sounds::EnemyTreads);
    }
    else {
        sfxManager->stopSound(SFXManager::Sounds::EnemyTreads);
    }

    shotAccumulator += deltaTime;
    shoot(dir);
}

void EnemyTank::doCollision(GameObject* other) {
    sfxManager->playSound(SFXManager::Sounds::Collision);
    sfxManager->playSound(SFXManager::Sounds::Explosion);
    sfxManager->stopSound(SFXManager::Sounds::EnemyTreads);
    selfDestruct();
    //Show explosion
    //wait a second or two
    Game::getInstance()->wonGame();
}

void EnemyTank::shoot(glm::vec3 direction) {
    if (shotAccumulator < shotThreshold) {
        return;
    }

    shotAccumulator = 0.0;
    Scene* scene = Scene::getInstance();

    // Don't spawn the bullet right on top of us
    auto bulletPos = this->getPosition() + this->getDirection();
    auto bulletDir = this->getDirection();

    auto bullet = new Projectile(scene->getNextFreeEntityID(), bulletPos, bulletDir, GameObjectType::EnemyProjectile);

    scene->addObject(bullet);
    sfxManager->playSound(SFXManager::Sounds::Firing);
}

EnemyTank::EnemyTank(uint32_t entityID, const vec3& position, const vec3& direction)
: Tank(GameObjectType::EnemyTank, entityID, position, direction),
shotAccumulator(0),
shotThreshold(10),
sfxManager(new SFXManager())
{
    this->setSpeed(0.5);
}
