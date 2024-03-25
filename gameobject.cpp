#include "gameobject.h"

// GameObject::GameObject(QObject *parent) { } //getting phased out
GameObject::GameObject(GameObjectType typ, QObject *parent) : type(type) { }

vec3 GameObject::getPosition() const { return position; }
vec3 GameObject::getDirection() const { return direction; }
GameObjectType GameObject::getType() const { return type; }
uint32_t GameObject::getEntityID() const { return entityID; }
void GameObject::setEntityID(int id) {entityID = id; }
void GameObject::doStart(){}
void GameObject::doUpdate(float deltaTime){}
