#include "jsonhelpers.h"


vec3 JsonHelpers::getVec3FromJson(QJsonArray vec3Array)
{
    return vec3(
        vec3Array[0].toDouble(),
        vec3Array[1].toDouble(),
        vec3Array[2].toDouble()
        );
}
