//
// Created by Parker on 2/23/2024.
//

#include "Obstacle.h"

#include <iostream>


// This is the mapping from strings to the obstacle types. When you want to add a new
// obstacle type, you must:
// 1. Add an entry to the enum
// 2. Add an entry here for the string mapping
// 3. Create a mesh/texture with that name and put them in assets
// 4. Add one in the level.json
static const std::unordered_map<std::string, ObstacleType> oTypeNames = {
    {"tree", ObstacleType::Tree},
    {"boulder", ObstacleType::Boulder},
    {"house", ObstacleType::House}
};


//Constructor for obstacles
Obstacle::Obstacle(
    QObject *parent,
    uint32_t entityID,
    const glm::vec3& position,
    float colliderRadius,
    const glm::vec3& direction,
    ObstacleType obstacleType
) :
    GameObject(GameObjectType::Obstacle, entityID, position, direction, parent),
    obstacleType(obstacleType)
{
    this->collider = CircleCollider(colliderRadius);
}

void Obstacle::doStart() {
    //Method for initializing obstacle specific properties
}

void Obstacle::doUpdate(float deltaTime) {
    //Method for obstacle update logic. Not sure if necessary unless they can move, or are destructible
}



ObstacleType Obstacle::getObstacleType() const {
    return obstacleType;
}

// This has to be a template, because on windows, it might be a whcar_t instead of a char
// Essentially it's just a function that accepts a string, no matter what kind it is,
// and returns the same type of string as a lowercase
template<typename T>
std::basic_string<T> toLowercase(const std::basic_string<T>& string) {
    std::basic_string<T> result;

    for(const auto& c : string) {
        result += std::tolower(c);
    }

    return result;
}


ObstacleType Obstacle::convertNameToObstacleType(const std::string& name) {
    std::string lower = toLowercase(name);

    for(const auto& pair : oTypeNames) {
        if (lower == pair.first) {
            return pair.second;
        }
    }

    std::cerr << "Obstacle: Unknown obstacle type: " << name << "\n";
    return ObstacleType::Tree;
}

std::string Obstacle::convertObstacleTypeToName(ObstacleType type) {
    for(const auto& pair : oTypeNames) {
        if (pair.second == type) {
            return pair.first;
        }
    }

    return std::string();
}
