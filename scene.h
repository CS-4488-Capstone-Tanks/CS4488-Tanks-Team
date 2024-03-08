#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "gamestate.h"

class Scene : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Scene object
     * 
     * @param float deltaTime
     * @param char stateFilename[]
     * @param QObject* parent = nullptr
    */
    explicit Scene(float deltaTime, char stateFilename[], QObject *parent = nullptr);

    /**
     * @brief Start the scene
    */
    void start();

    // void keyPress();

    /**
     * @brief Pause or resume the scene
     * 
     * @param bool isPaused
    */
    void setPaused(bool isPaused);
private:
    // NetworkManager networkManager
    GameState gameState;
    float deltaTime; // This should probably be initialized in game.h, where the QTimer is
    bool isPaused;

signals:
public slots:

    /**
     * @brief Update the scene
    */
    void update();
};

#endif // SCENE_H
