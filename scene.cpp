#include "scene.h"

Scene::Scene(float dT, QObject *parent)
    : QObject{parent}
{
    deltaTime = dT;
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
