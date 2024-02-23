#include "scene.h"

Scene::Scene(QObject *parent)
    : QObject{parent}
{

}

void Scene::start()
{

}

void Scene::pauseClock()
{

}

void Scene::update()
{
    gameState.updateState(deltaTime);
}
