#include "gameobject.h"

GameObject::GameObject(QObject *parent)
    : QObject{parent}
{

}

vec3 GameObject::getPosition() { return position; }
vec3 GameObject::getDirection() { return direction; }
GameObjectType GameObject::getType() { return type; }
uint32_t GameObject::getEntityID() { return entityID; }
void GameObject::setEntityID(int id) {entityID = id; }
void GameObject::doStart(){}
void GameObject::doUpdate(float deltaTime){}
