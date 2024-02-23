#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <glm/vec3.hpp>

using namespace glm;

class GameObject : public QObject
{
    Q_OBJECT
public:
    explicit GameObject(QObject *parent = nullptr);


    /**
     * This method gets called once immediately before the very first doUpdate() call. Initialize GameObject properties here.
     */
    virtual void doStart();


    /**
     * This method gets called once every update period on every GameObject.
     * Override it to change position, direction, or other properties over time.
     * @param float deltaTime: The time elapsed since the last update in seconds(?). This is a fixed value. Example: If you increment a property every frame by a number, its units will be units-per-frame (or meters-per-frame if you wish). If you want to work in units-per-second, multiply by deltaTime to convert to units-per-frame.
     */
    virtual void doUpdate(float deltaTime);



    // virtual void ObjectType getType();
    // virtual bool isDead();
    vec3 getPosition();
    vec3 getDirection();

protected:
    vec3 position = vec3(0,0,0);
    vec3 direction = vec3(0,0,0);
    float speed = 0.0f;

private:
    uint32_t entityID;

signals:
};

#endif // GAMEOBJECT_H
