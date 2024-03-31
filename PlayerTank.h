#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <QDataStream>
#include <QTimer>
#include "Tank.h"

class PlayerTank : public Tank {
Q_OBJECT

public:
    explicit PlayerTank(QObject *parent = nullptr);

    void doUpdate(float deltaTime) override;

private:
    QTimer shotTimer;
    void shoot(glm::vec3 direction) override;

public slots:
    void moveForward();
    void turnRight();
    void turnLeft();
    void moveBackward();

};

#endif // PLAYERTANK_H
