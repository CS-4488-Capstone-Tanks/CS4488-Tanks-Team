#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "gamestate.h"


/**
 * @brief The Scene class is responsible for managing the game state and updating the game objects.
 * @author Koda Koziol
 * @date SPRING 2024
*/
class Scene : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Scene object
     * @param float deltaTime
     * @param char stateFilename[]
     * @param QObject* parent = nullptr
     * @author Koda Koziol
     * @date SPRING 2024
    */
    explicit Scene(float deltaTime, std::string stateFilename, QObject *parent = nullptr);

    /**
     * @brief Start the scene. This should be called once at the start of the game,
        before the update loop begins.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    void start();

    // void keyPress();

    /**
     * @brief Pause or resume the scene
     * @param bool isPaused
     * @author Koda Koziol
     * @date SPRING 2024
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
     * @brief Update the scene. This should be called once per frame.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    void update();
};

#endif // SCENE_H
