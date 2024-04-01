#include "gameobject.h"

// GameObject::GameObject(QObject *parent) { } //getting phased out
GameObject::GameObject(GameObjectType type, QObject *parent)
: type(type),
position(0),
direction(glm::vec3(0, 0, -1)),
speed(0)
{ }

vec3 GameObject::getPosition() const { return position; }
vec3 GameObject::getDirection() const { return direction; }
GameObjectType GameObject::getType() const { return type; }
uint32_t GameObject::getEntityID() const { return entityID; }
void GameObject::setEntityID(int id) {entityID = id; }
void GameObject::doStart(){}
void GameObject::doUpdate(float deltaTime){}
