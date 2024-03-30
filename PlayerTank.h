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

public slots:
    void moveForward();
    void turnRight();
    void turnLeft();
    void moveBackward();

};

#endif // PLAYERTANK_H
