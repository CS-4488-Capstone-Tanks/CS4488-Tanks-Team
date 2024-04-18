//
// Created by Grant Madson on 3/30/2024.
//

#include "Tank.h"
#include <glm/geometric.hpp>

const float COLLIDER_RADIUS = 0.5f;

void Tank::shoot(vec3 direction) {

}

Tank::Tank(GameObjectType type, uint32_t entityID, const vec3& position, const vec3& direction, QObject* parent)
: GameObject(type, entityID, position, direction, parent),
angleInRadians(0.0)
{
    this->collider = CircleCollider(COLLIDER_RADIUS); //Second arg is collider radius, set to something appropriate

    // Make tanks actually spawn respecting direction vector
    glm::vec3 xaxis(1, 0, 0);
    float costheta = glm::dot(xaxis, direction);
    float theta = acos(costheta);

    angleInRadians = theta;
}

