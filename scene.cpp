#include "scene.h"
#include "EnemyTank.h"
#include "Obstacle.h"
#include "PlayerTank.h"
#include "jsonhelpers.h"
#include "collisionMatrix.h"

const char LEVELS_PATH[] = "assets/levels/";

// Game object keys
const char PLAYER_KEY[] = "playerTank";
const char ENEMY_KEY[] = "enemyTank";
const char OBSTACLES_KEY[] = "obstacles";
const char MAP_PROPERTIES_KEY[] = "mapProperties";

// Property keys
const char POS_KEY[] = "position";
const char DIR_KEY[] = "direction";
const char RAD_KEY[] = "radius";
const char TYPE_KEY[] = "type";
const char XLENGTH_KEY[] = "XLength";
const char ZLENGTH_KEY[] = "ZLength";

const vec3 DEFAULT_DIRECTION = vec3(0.0f, 0.0f, 1.0f);

CollisionMatrix collisionMatrix = CollisionMatrix();

Scene::Scene() {}

Scene::~Scene()
{
    reset();

    delete instance; // it is safe to delete a nullptr if it is one
    instance = nullptr;
}

Scene *Scene::instance = nullptr;

Scene *Scene::getInstance()
{
    if (instance == nullptr)
        instance = new Scene();

    return instance;
}

void Scene::start()
{
    foreach (GameObject *const obj, objs) {
        obj->start();
    }
}

void Scene::update(float deltaTime)
{
    if (isPaused)
        return;

    // Update all objects, or remove them if they are queued for destruction
    foreach (GameObject *const obj, objs) {
        if (obj->isQueuedForDestruction())
            removeObject(obj->getEntityID());
        else
            obj->update(deltaTime);
    }

    // Check and handle collisions between objects
    foreach (GameObject *const obj, objs) {
        if (obj->hasChanged()) {
            foreach (GameObject *const other, objs) {
                if (obj != other && collisionMatrix.canCollide(obj->getType(), other->getType()) && obj->getCollider().collidesWith(other->getCollider())) {
                    qWarning("Collision detected between a %s(%u) and %s(%u)",
                             gameObjectTypeToString(obj->getType()).c_str(),
                             obj->getEntityID(),
                             gameObjectTypeToString(other->getType()).c_str(),
                             other->getEntityID());
                    obj->doCollision(other);
                    other->doCollision(obj);
                    obj->resetChanged();
                    other->resetChanged();
                }
            }
        }
    }
}

void Scene::load(std::string filename)
{
    QString filepath = LEVELS_PATH + QString::fromStdString(filename) + ".json";
    QFile stateFile(filepath);

    if (!stateFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw "read error: couldn't open file \"" + filepath.toStdString() + "\"";

    QByteArray stateData = stateFile.readAll();
    QJsonDocument saveDoc(QJsonDocument::fromJson(stateData));
    QJsonObject jsonObj = saveDoc.object();

    // Load map properties
    if (const QJsonValue v = jsonObj[MAP_PROPERTIES_KEY]; v.isObject()) {
        try {
            QJsonObject jsonPlayerObj = v.toObject();

            if(const QJsonValue XLength = jsonPlayerObj[XLENGTH_KEY]; XLength.isDouble())
                MapXLength = XLength.toDouble();
            else
                throw std::invalid_argument("Expected a double for \"XLength\"");

            if(const QJsonValue ZLength = jsonPlayerObj[ZLENGTH_KEY]; ZLength.isDouble())
                MapZLength = ZLength.toDouble();
            else
                throw std::invalid_argument("Expected a double for \"ZLength\"");

        } catch (std::invalid_argument &e) {
            qWarning("Error loading map properties: %s", e.what());
        }
    }

    // Load player tank
    if (const QJsonValue v = jsonObj[PLAYER_KEY]; v.isObject()) {
        try {
            QJsonObject jsonPlayerObj = v.toObject();
            vec3 position, direction;

            if (const QJsonValue posVal = jsonPlayerObj[POS_KEY]; posVal.isArray())
                position = JsonHelpers::getVec3FromJson(posVal.toArray());
            else
                throw std::invalid_argument("Expected an array for \"position\"");

            if (const QJsonValue dirVal = jsonPlayerObj[DIR_KEY]; dirVal.isArray())
                direction = JsonHelpers::getVec3FromJson(dirVal.toArray());
            else
                throw std::invalid_argument("Expected an array for \"direction\"");

            auto obj = new PlayerTank(getNextFreeEntityID(), position, direction);
            addObject(obj);
        } catch (std::invalid_argument &e) {
            qWarning("Error loading player tank: %s", e.what());
        }
    }

    // Load enemy tank
    if (const QJsonValue v = jsonObj[ENEMY_KEY]; v.isObject()) {
        try {
            QJsonObject jsonEnemyObj = v.toObject();
            vec3 position, direction;

            if (const QJsonValue posVal = jsonEnemyObj[POS_KEY]; posVal.isArray())
                position = JsonHelpers::getVec3FromJson(posVal.toArray());
            else
                throw std::invalid_argument("Expected an array for \"position\"");

            if (const QJsonValue dirVal = jsonEnemyObj[DIR_KEY]; dirVal.isArray())
                direction = JsonHelpers::getVec3FromJson(dirVal.toArray());
            else
                throw std::invalid_argument("Expected an array for \"direction\"");

            auto obj = new EnemyTank(getNextFreeEntityID(), position, direction);
            addObject(obj);
        } catch (std::invalid_argument &e) {
            qWarning("Error loading enemy tank: %s", e.what());
        }
    }

    // Load obstacles
    if (const QJsonValue v = jsonObj[OBSTACLES_KEY]; v.isArray()) {
        const QJsonArray jsonObstacleObjs = v.toArray();
        for (const QJsonValue &obsVal : jsonObstacleObjs) {
            try {
                QJsonObject jsonObstacleObj = obsVal.toObject();
                vec3 position;
                vec3 direction;
                float radius;
                std::string type;

                if (const QJsonValue posVal = jsonObstacleObj[POS_KEY]; posVal.isArray())
                    position = JsonHelpers::getVec3FromJson(posVal.toArray());
                else
                    throw std::invalid_argument("Expected an array for \"position\"");

                if (const QJsonValue dirVal = jsonObstacleObj[DIR_KEY]; dirVal.isArray())
                    direction = JsonHelpers::getVec3FromJson(dirVal.toArray());
                else {
                    qWarning(
                        "Warning: \"direction\" not found for obstacle, using default (%f,%f,%f)",
                        DEFAULT_DIRECTION.x,
                        DEFAULT_DIRECTION.y,
                        DEFAULT_DIRECTION.z);
                    direction = DEFAULT_DIRECTION;
                }

                if (const QJsonValue radVal = jsonObstacleObj[RAD_KEY]; radVal.isDouble())
                    radius = radVal.toDouble();
                else
                    throw std::invalid_argument("Expected a double for \"radius\"");

                if (const QJsonValue typeVal = jsonObstacleObj[TYPE_KEY]; typeVal.isString())
                    type = typeVal.toString().toStdString();
                else
                    throw std::invalid_argument("Expected a string for \"type\"");

                auto obj = new Obstacle(nullptr,
                                        getNextFreeEntityID(),
                                        position,
                                        radius,
                                        direction,
                                        Obstacle::convertNameToObstacleType(type));
                addObject(obj);
            } catch (std::invalid_argument &e) {
                qWarning("Error loading an obstacle: %s", e.what());
            }
        }
    }
}

void Scene::setPaused(bool p)
{
    isPaused = p;
}


int Scene::getNextFreeEntityID()
{
    return nextFreeEntityID++;
}

int Scene::addObject(GameObject *const obj)
{
    objs.push_back(obj);
    return obj->getEntityID();
}

void Scene::removeObject(uint32_t entityID)
{
    for (auto it = objs.begin(); it != objs.end(); ++it) {
        if ((*it)->getEntityID() == entityID) {
            delete *it;
            objs.erase(it);
            return;
        }
    }
}

GameObject *Scene::getGameObject(uint32_t entityID) const
{
    for (GameObject *const obj : objs) {
        if (obj->getEntityID() == entityID)
            return obj;
    }
    return nullptr;
}

GameObject *Scene::getGameObject(GameObjectType type) const
{
    for (GameObject *const obj : objs) {
        if (obj->getType() == type)
            return obj;
    }
    return nullptr;
}

std::vector<GameObject *> Scene::getGameObjects(GameObjectType type) const
{
    std::vector<GameObject *> result;
    for (GameObject *const obj : objs) {
        if (obj->getType() == type)
            result.push_back(obj);
    }
    return result;
}

std::vector<GameObject *>::const_iterator Scene::begin() const
{
    return objs.begin();
}

std::vector<GameObject *>::const_iterator Scene::end() const
{
    return objs.end();
}

double Scene::getZLength()
{
    return MapZLength;
}

double Scene::getXLength()
{
    return MapXLength;
}

void Scene::reset()
{
    for (GameObject *const obj : objs)
    {
        delete obj;
    }

    objs.clear();
    nextFreeEntityID = 0;
}
