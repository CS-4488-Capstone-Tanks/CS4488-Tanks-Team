//
// Created by Grant Madson on 3/4/2024.
//

#ifndef TANKS_ENEMYTANK_H
#define TANKS_ENEMYTANK_H


#include <QTimer>
#include "Tank.h"
#include "sfxmanager.h"

class EnemyTank : public Tank {
    Q_OBJECT

public:
    explicit EnemyTank(
        uint32_t entityID,
        const vec3& position = vec3(0.0f),
        const vec3& direction = vec3(0.0f, 0.0f, -1.0f),
        QObject *parent = nullptr
    );

    void doUpdate(float deltaTime) override;
    void doCollision(GameObject* other) override;

private:
    void shoot(glm::vec3 direction) override;
    float shotAccumulator;
    float shotThreshold;
    SFXManager* sfxManager;

//TODO: implement collider to detect when an obstacle has been hit. Use this for AI logic
//TODO: Implement shooting at player
};


#endif //TANKS_ENEMYTANK_H
