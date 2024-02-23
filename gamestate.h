#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include "gameobject.h"

class GameState
{
public:
    GameState();
    void startState();
    void updateState(float deltaTime);
    void readState(std::string file);
    void addObject();
private:
    std::vector<GameObject*> objs;
    void removeObject(uint32_t entityID);
};

#endif // GAMESTATE_H
