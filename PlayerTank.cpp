//
// Created by Grant Madson on 3/4/2024.
//
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <QDataStream>
#include "game.h"
#include "PlayerTank.h"
#include "scene.h"
#include "Projectile.h"
#include "glm/ext/matrix_transform.hpp"

/**
 * @authors Grant Madson, Tyson Cox, Koda Koziol
 * @param deltaTime
 * @brief The update logic. Moves the position and rotation of the player tank and increments the shotAccumulator.
 */
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

    if (std::any_of(std::begin(dirTable), std::end(dirTable), [](bool b){return b; })) {
        sfxManager->playSound(SFXManager::Sounds::PlayerTreads);
    }
    else {
        sfxManager->stopSound(SFXManager::Sounds::PlayerTreads);
    }

    shotAccumulator += deltaTime;
}

/**
 * @authors Tyson Cox, Koda Koziol
 * @param other
 * @brief Game over on collision and plays some collision sounds.
 */
void PlayerTank::doCollision(GameObject* other) {
    sfxManager->playSound(SFXManager::Sounds::Collision);
    sfxManager->playSound(SFXManager::Sounds::Explosion);
    sfxManager->stopSound(SFXManager::Sounds::PlayerTreads);
    selfDestruct();
    //Show explosion
    //wait a second or two
    Game::getInstance()->gameOver();
}

/**
 * @authors Grant Madson, Tyson Cox
 * @param direction
 * @brief checks if the tank is able to fire and then does so if able. Shoots in the same direction the tank is facing.
 */
void PlayerTank::shoot(glm::vec3 direction) {
    if (shotAccumulator < shotThreshold) {
        return;
    }

    shotAccumulator = 0.0;
    Scene* scene = Scene::getInstance();

    // Don't spawn the bullet right on top of us
    auto bulletPos = this->getPosition() + this->getDirection();
    auto bulletDir = this->getDirection();

    auto bullet = new Projectile(scene->getNextFreeEntityID(), bulletPos, bulletDir, GameObjectType::PlayerProjectile);

    scene->addObject(bullet);
    sfxManager->playSound(SFXManager::Sounds::Firing);
}

/**
 * @authors Grant Madson, Tyson Cox, Luna Steed
 * @param entityID
 * @param position
 * @param direction
 * @param parent
 * @brief Sets internal values for the PlayerTank and sets the sfx speed.
 */
PlayerTank::PlayerTank(uint32_t entityID, const vec3& position, const vec3& direction)
: Tank(GameObjectType::PlayerTank, entityID, position, direction),
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

/**
 * @authors Grant Madson, Tyson Cox
 * @param event
 * @return
 * @brief Sets the boolean table that controls the movement of the PLayerTank based on input given by the gamewindow. handles both key presses and key releases.
 */
bool PlayerTank::handleKeyEvent(QKeyEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        switch (event->key()) {
            case Qt::Key_W:
            case Qt::Key_Up:
                dirTable[0] = true;
                return true;
            case Qt::Key_S:
            case Qt::Key_Down:
                dirTable[1] = true;
                return true;
            case Qt::Key_A:
            case Qt::Key_Left:
                dirTable[2] = true;
                return true;
            case Qt::Key_D:
            case Qt::Key_Right:
                dirTable[3] = true;
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
                return true;
            case Qt::Key_S:
            case Qt::Key_Down:
                dirTable[1] = false;
                return true;
            case Qt::Key_A:
            case Qt::Key_Left:
                dirTable[2] = false;
                return true;
            case Qt::Key_D:
            case Qt::Key_Right:
                dirTable[3] = false;
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
