#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <glm/vec3.hpp>

class GameObject : public QObject
{
    Q_OBJECT
public:
    explicit GameObject(QObject *parent = nullptr);
    virtual void doStart();
    virtual void doUpdate();
    // virtual void ObjectType getType();
    // virtual bool isDead();
    glm::vec3 getPosition();
    glm::vec3 getDirection();

private:
    glm::vec3 position;
    glm::vec3 direction;
    uint32_t entityID;
    float speed;

signals:
};

#endif // GAMEOBJECT_H
