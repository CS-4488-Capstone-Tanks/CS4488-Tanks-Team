//
// Created by Grant Madson on 3/4/2024.
//
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <QDataStream>
#include "PlayerTank.h"
#include "gamestate.h"
#include "Projectile.h"
#include "glm/ext/matrix_transform.inl"


void PlayerTank::doUpdate(float deltaTime) {
    vec3 pos = this->getPosition();
    float spd = this->getSpeed();
    
    
    vec3 dir = glm::vec3(cos(angleInRadians), 0, sin(angleInRadians));
    this->setDirection(dir);

    if (dirTable[0]) {
        pos += dir * spd * deltaTime;
        this->setPosition(pos);
    }

    if (dirTable[1]) {
        pos -= dir * spd * deltaTime;
        this->setPosition(pos);
    }

    if (dirTable[2]) {
        angleInRadians -= spd * deltaTime;
    }

    if (dirTable[3]) {
        angleInRadians += spd * deltaTime;
    }

    if (wantFire) {
        shoot(dir);
    }

    shotAccumulator += deltaTime;
}

void PlayerTank::doCollision(GameObject* other) {
    if (other->getType() != GameObjectType::PlayerProjectile)
        sfxManager->playCollide();
        sfxManager->playExplosion();
        sfxManager->stopTankTread();
        selfDestruct();
}

void PlayerTank::shoot(glm::vec3 direction) {
    if (shotAccumulator < shotThreshold) {
        return;
    }

    shotAccumulator = 0.0;
    GameState* gamestate = GameState::getInstance();

    // Don't spawn the bullet right on top of us
    auto bulletPos = this->getPosition() + this->getDirection();
    auto bulletDir = this->getDirection();

    auto bullet = new Projectile(nullptr, gamestate->getNextFreeEntityID(), bulletPos, bulletDir, GameObjectType::PlayerProjectile);

    gamestate->addObject(bullet);
    sfxManager->playTankFire();
}

PlayerTank::PlayerTank(uint32_t entityID, const vec3& position, const vec3& direction, QObject* parent)
: Tank(GameObjectType::PlayerTank, entityID, position, direction, parent),
shotAccumulator(0),
shotThreshold(10),
wantFire(false),
sfxManager(new SFXManager())
{
    for(auto& val : dirTable) {
        val = false;
    }

    this->setSpeed(0.8);
}

bool PlayerTank::handleKeyEvent(QKeyEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        switch (event->key()) {
            case Qt::Key_W:
            case Qt::Key_Up:
                dirTable[0] = true;
                sfxManager->playTankTread();
                return true;
            case Qt::Key_S:
            case Qt::Key_Down:
                dirTable[1] = true;
                sfxManager->playTankTread();
                return true;
            case Qt::Key_A:
            case Qt::Key_Left:
                dirTable[2] = true;
                sfxManager->playTankTread();
                return true;
            case Qt::Key_D:
            case Qt::Key_Right:
                dirTable[3] = true;
                sfxManager->playTankTread();
                return true;
            case Qt::Key_Space:
                wantFire = true;
                return true;
            default:
                return false;
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        switch (event->key()) {
            case Qt::Key_W:
            case Qt::Key_Up:
                dirTable[0] = false;
                sfxManager->stopTankTread();
                return true;
            case Qt::Key_S:
            case Qt::Key_Down:
                dirTable[1] = false;
                sfxManager->stopTankTread();
                return true;
            case Qt::Key_A:
            case Qt::Key_Left:
                dirTable[2] = false;
                sfxManager->stopTankTread();
                return true;
            case Qt::Key_D:
            case Qt::Key_Right:
                dirTable[3] = false;
                sfxManager->stopTankTread();
                return true;
            case Qt::Key_Space:
                wantFire = false;
                return true;
            default:
                return false;
        }
    }
    else {
        return false;
    }
}
