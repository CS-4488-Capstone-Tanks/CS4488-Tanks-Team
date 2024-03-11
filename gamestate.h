#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "gameobject.h"

class GameState
{
public:
    GameState();
    void startState();
    void updateState(float deltaTime);

    /**
     * Given a the name of a .json state file, this method will load the state.
        Note that the state file must be located in the "assets/levels/"
        directory. Do not include ".json" in the filename.
     * @param filename
     */
    void loadState(char filename[]);
    void addObject(GameObject *const obj);


private:
    std::vector<GameObject*> objs;
    int nextFreeEntityID = 0;

    /**
     * Remove a GameObject from the GameState.
     * @param entityID
     */
    void removeObject(uint32_t entityID);
};

#endif // GAMESTATE_H
