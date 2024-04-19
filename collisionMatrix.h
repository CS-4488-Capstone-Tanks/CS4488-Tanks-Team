#ifndef COLLISIONMATRIX_H
#define COLLISIONMATRIX_H

#include "gameobjecttype.h"

/**
 * @brief The CollisionMatrix class is a helper class that stores a matrix of whether different types of objects can collide with each other.
*/
class CollisionMatrix
{
public:

    /**
     * @brief Construct a new Collision Matrix object.
     * @note Alter this constructor definition to change the default collision matrix.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    CollisionMatrix()
    {
        setAllCanCollide(true);
        setCanCollide(GameObjectType::Obstacle, GameObjectType::Obstacle, false);
        setCanCollide(GameObjectType::PlayerTank, GameObjectType::PlayerProjectile, false);
        setCanCollide(GameObjectType::EnemyTank, GameObjectType::EnemyProjectile, false);
    }

    
    /**
     * @param type1 The first type of object
     * @param type2 The second type of object
     * @return True if type1 can collide with type2, false otherwise. Note that this is not symmetric.
     * @author Koda Koziol
     * @date SPRING 2024
     */
    inline bool canCollide(GameObjectType type1, GameObjectType type2) const
    {
        return matrix[static_cast<int>(type1)][static_cast<int>(type2)];
    }
    

    /**
     * @brief Set whether two types of objects can collide with each other.
     * @param type1 The first type of object
     * @param type2 The second type of object
     * @param canCollide True if type1 should collide with type2, false otherwise
     * @param symmetric If false, the collision will not be symmetric (i.e. type1 can collide with type2 but not vice versa). Default is true.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    inline void setCanCollide(GameObjectType type1, GameObjectType type2, bool canCollide, bool symmetric = true)
    {
        int t1 = static_cast<int>(type1);
        int t2 = static_cast<int>(type2);
        matrix[t1][t2] = canCollide;
        if (symmetric)
            matrix[t2][t1] = canCollide;
    }


    /**
     * @brief Set whether a type of object can collide with all other types of objects.
     * @param type1 The type of object
     * @param canCollide True if type1 should collide with all other types, false otherwise
     * @param symmetric If false, the collision will not be symmetric (i.e. type1 can collide with all other types but not vice versa). Default is true.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    inline void setCanCollide(GameObjectType type1, bool canCollide, bool symmetric = true)
    {
        for (int i = 0; i < NUM_GAME_OBJECT_TYPES; i++)
        {
            matrix[static_cast<int>(type1)][i] = canCollide;
            if (symmetric)
                matrix[i][static_cast<int>(type1)] = canCollide;
        }
    }


    /**
     * @brief Set whether all types of objects can collide with each other.
     * @param canCollide True if all types should collide with each other, false otherwise.
     * @author Koda Koziol
     * @date SPRING 2024
    */
    inline void setAllCanCollide(bool canCollide)
    {
        for (int i = 0; i < NUM_GAME_OBJECT_TYPES; i++)
        {
            for (int j = 0; j < NUM_GAME_OBJECT_TYPES; j++)
                matrix[i][j] = canCollide;
        }
    }

private:
    bool matrix[NUM_GAME_OBJECT_TYPES][NUM_GAME_OBJECT_TYPES];
};

#endif // COLLISIONMATRIX_H
