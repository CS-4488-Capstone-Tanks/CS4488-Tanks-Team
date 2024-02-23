#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "gamestate.h"

class Scene : public QObject
{
    Q_OBJECT

public:
    explicit Scene(float deltaTime, QObject *parent = nullptr);
    void start();
    // void keyPress();
    void setPaused(bool isPaused);
private:
    // NetworkManager networkManager
    GameState gameState;
    float deltaTime; // This should probably be initialized in game.h, where the QTimer is
    bool isPaused;

signals:
public slots:
    void update();
};

#endif // SCENE_H
