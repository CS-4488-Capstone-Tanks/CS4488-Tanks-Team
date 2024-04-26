#include "jsonhelpers.h"

vec3 JsonHelpers::getVec3FromJson(QJsonArray vArray)
{
    if (vArray.size() != 3)
        throw std::invalid_argument("Expected an array of size 3");
    return vec3(
        vArray[0].toDouble(),
        vArray[1].toDouble(),
        vArray[2].toDouble()
        );
}

QJsonArray JsonHelpers::getJsonFromVec3(vec3 v)
{
    return QJsonArray({v.x, v.y, v.z});
}