#include "gameobject.h"

GameObject::GameObject(QObject *parent)
    : QObject{parent}
{

}

void GameObject::doStart(){}
void GameObject::doUpdate(float deltaTime){}

vec3 GameObject::getPosition()
{
    return position;
}

vec3 GameObject::getDirection()
{
    return direction;
}
