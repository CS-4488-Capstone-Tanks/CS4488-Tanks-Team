#include "scene.h"
#include <QMessageBox>
#include <iostream>

Scene::Scene(float dT, std::string stateFilename, QObject *parent)
    : QObject{parent}
{
    deltaTime = dT;
    try {
        gameState->loadState(stateFilename);
    }
    catch (std::string errStr){
        std::cout << "GameState failed to load: " + errStr << std::endl;
    }
}

void Scene::start()
{
    gameState->startState();

    GameObject* test1 = gameState->getGameObject(0);
    std::vector<GameObject*> test2 = gameState->getGameObjects(GameObjectType::Obstacle);
}

void Scene::setPaused(bool p)
{
    isPaused = p;
}


std::vector<GameObject*>::const_iterator Scene::begin() const {
    return gameState->begin();
}

std::vector<GameObject*>::const_iterator Scene::end() const {
    return gameState->end();
}

GameObject* Scene::getPlayerTank(){
    for (auto it : *this){
        if (it->getType() == GameObjectType::PlayerTank){
            return it;
        }
    }
    return nullptr;
}

void Scene::update()
{
    if (!isPaused)
        gameState->updateState(deltaTime);
}
