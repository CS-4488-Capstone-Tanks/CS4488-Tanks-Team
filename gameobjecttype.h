#ifndef GAMEOBJECTTYPE_H
#define GAMEOBJECTTYPE_H

#include <string>

enum class GameObjectType
{
    PlayerTank,
    EnemyTank,
    Obstacle,
    PlayerProjectile,
    EnemyProjectile,
    None
};

const int NUM_GAME_OBJECT_TYPES = 5; // IMPORTANT: Update this if you add more types (do not include None)

inline std::string gameObjectTypeToString(GameObjectType type)
{
    switch (type)
    {
        case GameObjectType::None:
            return "None";
        case GameObjectType::PlayerTank:
            return "PlayerTank";
        case GameObjectType::EnemyTank:
            return "EnemyTank";
        case GameObjectType::Obstacle:
            return "Obstacle";
        case GameObjectType::PlayerProjectile:
            return "PlayerProjectile";
        case GameObjectType::EnemyProjectile:
            return "EnemyProjectile";
        default:
            return "Unknown type";
    }
}

#endif // GAMEOBJECTTYPE_H
