#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <QDataStream>
#include <QTimer>
#include "Tank.h"
#include <QKeyEvent>
#include <iostream>

class PlayerTank : public Tank {
Q_OBJECT

public:
    explicit PlayerTank(QObject *parent = nullptr);

    void doUpdate(float deltaTime) override;

private:
    QTimer shotTimer;
    void shoot(glm::vec3 direction) override;
    void moveForward();
    void turnRight();
    void turnLeft();
    void moveBackward();

    //started work about 7:30
public slots:
    void handleKeyEvent(QKeyEvent *event){
        switch(event->key()){
            case Qt::Key_W:
                this->moveForward();
                std::cout<<"forward";
                break;
            case Qt::Key_S:
                this->moveBackward();
                break;
            case Qt::Key_A:
                this->turnLeft();
                break;
            case Qt::Key_D:
                this->turnRight();
                break;
            default:
                break;
        }

    };

};

#endif // PLAYERTANK_H
