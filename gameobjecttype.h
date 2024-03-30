#ifndef GAMEOBJECTTYPE_H
#define GAMEOBJECTTYPE_H

#include <string>

enum class GameObjectType
{
    PlayerTank,
    EnemyTank,
    Obstacle,
    Projectile
};

inline std::string gameObjectTypeToString(GameObjectType type)
{
    switch (type)
    {
        case GameObjectType::PlayerTank:
            return "PlayerTank";
        case GameObjectType::EnemyTank:
            return "EnemyTank";
        case GameObjectType::Obstacle:
            return "Obstacle";
        case GameObjectType::Projectile:
            return "Projectile";
        default:
            return "Unknown type";
    }
}

#endif // GAMEOBJECTTYPE_H
