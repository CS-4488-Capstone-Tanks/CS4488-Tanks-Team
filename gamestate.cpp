#include "gamestate.h"

GameState::GameState()
{

}

void GameState::startState()
{
    foreach (GameObject *const obj, objs) {
        obj->doStart();
    }
}

void GameState::updateState(float deltaTime)
{
    foreach (GameObject *const obj, objs) {
        obj->doUpdate(deltaTime);
    }
}

void GameState::readState(std::string file)
{

}

void GameState::addObject(GameObject *const obj)
{
    obj->setEntityID(nextFreeEntityID++);
    objs.push_back(obj);
}

/**
 * Remove a GameObject from the GameState.
 * @param entityID
 */
void GameState::removeObject(uint32_t entityID)
{

}
