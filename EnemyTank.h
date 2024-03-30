//
// Created by Grant Madson on 3/4/2024.
//

#ifndef TANKS_ENEMYTANK_H
#define TANKS_ENEMYTANK_H


#include <QTimer>
#include "Tank.h"

class EnemyTank : public Tank {
    Q_OBJECT

public:
    explicit EnemyTank();

    void doUpdate(float deltaTime) override;

private:
    QTimer shotTimer;

//TODO: implement collider to detect when an obstacle has been hit. Use this for AI logic
//TODO: Implement shooting at player
};


#endif //TANKS_ENEMYTANK_H
