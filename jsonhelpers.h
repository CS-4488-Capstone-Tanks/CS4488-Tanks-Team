#ifndef JSONHELPERS_H
#define JSONHELPERS_H

#include <QJsonArray>
#include <glm/vec3.hpp>

using namespace glm;


/**
 * @brief This class contains helper methods to make parsing and generating
    QT Json stuff a little less painful.
 * @author Koda Koziol
 * @date SPRING 2024
 */
class JsonHelpers
{
public:
    /**
     * @brief Given a QJsonArray with 3 numeric elements representing
        x, y, and z in that order, return a vec3.
     * @param vec3Array
     * @return vec3
     * @throws std::invalid_argument if the array is not of size 3
     * @author Koda Koziol
     * @date SPRING 2024
     */
    static vec3 getVec3FromJson(QJsonArray vec3Array);

      /**
      * @brief Given a vec3, return a QJsonArray with 3 numeric elements
         representing x, y, and z in that order.
      * @param vec
      * @return QJsonArray
      * @author Koda Koziol
      * @date SPRING 2024
      */
    static QJsonArray getJsonFromVec3(vec3 vec);
};

#endif // JSONHELPERS_H
