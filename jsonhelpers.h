#ifndef JSONHELPERS_H
#define JSONHELPERS_H

#include <QJsonArray>
#include <glm/vec3.hpp>

using namespace glm;

class JsonHelpers
{
public:
    static vec3 getVec3FromJson(QJsonArray vec3Array);
};

#endif // JSONHELPERS_H
