#include "scene.h"

Scene::Scene(QObject *parent)
    : QObject{parent}
{

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
