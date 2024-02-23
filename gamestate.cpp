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

void GameState::addObject()
{

}

void GameState::removeObject(uint32_t entityID)
{

}
