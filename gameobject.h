#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <glm/vec3.hpp>
#include "gameobjecttype.h"

using namespace glm;

class GameObject : public QObject
{
    Q_OBJECT
public:
    explicit GameObject(QObject *parent = nullptr);


    /**
     * This method gets called once immediately before the very first
        doUpdate() call. Initialize GameObject properties here.
     */
    virtual void doStart();

    /**
     * This method gets called once every update period on every GameObject.
     * Override it to change position, direction,
       or other properties over time.
     * @param float deltaTime: The time elapsed since the last update in
        seconds(?). This is a fixed value. Example: If you increment a
        property every frame by a number, its units will be
        units-per-frame (or meters-per-frame if you wish). If you want to
        work in units-per-second, multiply by deltaTime to convert to
        units-per-frame.
     */
    virtual void doUpdate(float deltaTime);

    vec3 getPosition();
    vec3 getDirection();
    GameObjectType getType();
    uint32_t getEntityID();
    void setEntityID(int id);


protected:
    vec3 position = vec3(0,0,0);
    vec3 direction = vec3(0,0,0);
    float speed = 0.0f;
    uint32_t entityID;
    GameObjectType type;

signals:
};

#endif // GAMEOBJECT_H
