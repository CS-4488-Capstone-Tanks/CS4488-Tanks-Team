#include "gameobject.h"

GameObject::GameObject(GameObjectType type, QObject *parent) : type(type) { }

vec3 GameObject::getPosition() const { return position; }
vec3 GameObject::getDirection() const { return direction; }
GameObjectType GameObject::getType() const { return type; }
uint32_t GameObject::getEntityID() const { return entityID; }
void GameObject::selfDestruct() { _isQueuedForDestruction = true; }
bool GameObject::isQueuedForDestruction() const { return _isQueuedForDestruction; }
void GameObject::doStart(){}
void GameObject::doUpdate(float deltaTime){}
