#include "gameobject.h"
#include <glm/glm.hpp>


GameObject::GameObject(GameObjectType type, QObject *parent) : type(type) { }
GameObject::GameObject(
        GameObjectType type,
        uint32_t entityID,
        const vec3 &position,
        const vec3 &direction,
        QObject *parent
) : type(type), entityID(entityID), position(position), direction(direction)
{
    if (direction == vec3(0.0f))
        throw std::invalid_argument("Direction cannot be the zero vector.");
}

CircleCollider GameObject::getCollider() const { return collider; }

vec3 GameObject::getPosition() const { return position; }
void GameObject::setPosition(const vec3 &pos)
{
	position = pos;
	_hasChanged = true;
}

vec3 GameObject::getDirection() const { return direction; }
void GameObject::setDirection(const vec3 &dir)
{
	if (dir == vec3(0.0f))
		throw std::invalid_argument("Direction cannot be the zero vector.");
	direction = glm::normalize(dir);
	//_hasChanged = true; // No need to enable this until non-circle colliders are implemented
}

float GameObject::getSpeed() const { return speed; }
void GameObject::setSpeed(float spd)
{
	if (spd <= 0)
		throw std::invalid_argument("Speed must be positive.");
	speed = spd;
}

GameObjectType GameObject::getType() const { return type; }
uint32_t GameObject::getEntityID() const { return entityID; }

void GameObject::selfDestruct() { _isQueuedForDestruction = true; }
bool GameObject::isQueuedForDestruction() const { return _isQueuedForDestruction; }
bool GameObject::hasChanged() const { return _hasChanged; }
void GameObject::resetChanged() { _hasChanged = false; }

void GameObject::startState(){
	doStart();
	collider.updatePosition(position);
}

void GameObject::updateState(float deltaTime){
	doUpdate(deltaTime);
	if (hasChanged())
		collider.updatePosition(position);
}

void GameObject::doStart(){}
void GameObject::doUpdate(float deltaTime){}
void GameObject::doCollision(GameObject *other){}
