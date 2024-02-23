#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "gamestate.h"

class Scene : public QObject
{
    Q_OBJECT

public:
    explicit Scene(QObject *parent = nullptr);
    void start();
    // void keyPress();
    void pauseClock();
private:
    // NetworkManager networkManager
    GameState gameState;
    float deltaTime;
    bool isPaused;

signals:
public slots:
    void update();
};

#endif // SCENE_H
