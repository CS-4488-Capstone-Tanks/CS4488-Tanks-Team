#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include "Tank.h"

class PlayerTank : public Tank {
Q_OBJECT

public:
    void doUpdate(float deltaTime) override {};

public slots:
    void moveForward();
    void turnRight();
    void turnLeft();
    void moveBackward();

private:
};

#endif // PLAYERTANK_H
