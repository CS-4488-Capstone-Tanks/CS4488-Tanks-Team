#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <QDataStream>
#include <QTimer>
#include <QKeyEvent>
#include "Tank.h"
#include "sfxmanager.h"
#include "game.h"

class PlayerTank : public Tank {
Q_OBJECT

public:
    explicit PlayerTank(
        uint32_t entityID,
        const vec3& position = vec3(0.0f),
        const vec3& direction = vec3(0.0f, 0.0f, -1.0f),
        QObject *parent = nullptr
    );

    void doUpdate(float deltaTime) override;
    void doCollision(GameObject* other) override;
    bool handleKeyEvent(QKeyEvent* event);
private:
    bool dirTable[4];
    bool wantFire;
    float shotAccumulator;
    float shotThreshold;
    SFXManager* sfxManager;
    void shoot(glm::vec3 direction) override;

};

#endif // PLAYERTANK_H
