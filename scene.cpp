#include "scene.h"
#include <QMessageBox>
#include <iostream>

Scene::Scene(float dT, char stateFilename[], QObject *parent)
    : QObject{parent}
{
    deltaTime = dT;
    try {
        gameState.loadState(stateFilename);
    }
    catch (std::string errStr){
        std::cout << "GameState failed to load: " + errStr << std::endl;
    }
}

void Scene::start()
{
    gameState.startState();
}

void Scene::setPaused(bool p)
{
    isPaused = p;
}

void Scene::update()
{
    if (!isPaused)
        gameState.updateState(deltaTime);
}
