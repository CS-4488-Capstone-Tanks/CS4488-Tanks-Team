#include "gamestate.h"
#include "jsonhelpers.h"
#include "Obstacle.h"
#include "PlayerTank.h"
#include "EnemyTank.h"

const char LEVELS_PATH[] = "assets/levels/";

// Game object keys
const char PLAYER_KEY[] = "playerTank";
const char ENEMY_KEY[] = "enemyTank";
const char OBSTACLES_KEY[] = "obstacles";

// Property keys
const char POS_KEY[] = "position";
const char DIR_KEY[] = "direction";
const char RAD_KEY[] = "radius";

const vec3 DEFAULT_DIRECTION = vec3(0.0f, 0.0f, 1.0f);

GameState::GameState() {}

GameState::~GameState() {
    for (GameObject *const obj : objs)
        delete obj;
    objs.clear();

    if (instance != nullptr)
        delete instance;
    instance = nullptr;
    nextFreeEntityID = 0;
}

GameState* GameState::instance = nullptr;

GameState* GameState::getInstance()
{
    if (instance == nullptr)
        instance = new GameState();

    return instance;
}

void GameState::startState()
{
    foreach (GameObject *const obj, objs) {
        obj->doStart();
    }
}

void GameState::updateState(float deltaTime)
{
    foreach (GameObject *const obj, objs) {
        if (obj->isQueuedForDestruction())
            removeObject(obj->getEntityID());
        else
            obj->doUpdate(deltaTime);
    }
}

void GameState::loadState(std::string filename)
{
    QString filepath = LEVELS_PATH + QString::fromStdString(filename) + ".json";
    QFile stateFile(filepath);

    if (!stateFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw "read error: couldn't open file \"" + filepath.toStdString() + "\"";

    QByteArray stateData = stateFile.readAll();
    QJsonDocument saveDoc(QJsonDocument::fromJson(stateData));
    QJsonObject jsonObj = saveDoc.object();

    // Load player tank
    if (const QJsonValue v = jsonObj[PLAYER_KEY]; v.isObject())
    {
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
        }
        catch (std::invalid_argument &e) {
            qWarning("Error loading player tank: %s", e.what());
        }
    }

    // Load enemy tank
    if (const QJsonValue v = jsonObj[ENEMY_KEY]; v.isObject())
    {
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
        }
        catch (std::invalid_argument &e) {
            qWarning("Error loading enemy tank: %s", e.what());
        }
    }

    // Load obstacles
    if (const QJsonValue v = jsonObj[OBSTACLES_KEY]; v.isArray())
    {
        const QJsonArray jsonObstacleObjs = v.toArray();
        for (const QJsonValue &obsVal : jsonObstacleObjs)
        {
            try {
                QJsonObject jsonObstacleObj = obsVal.toObject();
                vec3 position;
                vec3 direction;
                float radius;

                if (const QJsonValue posVal = jsonObstacleObj[POS_KEY]; posVal.isArray())
                    position = JsonHelpers::getVec3FromJson(posVal.toArray());
                else
                    throw std::invalid_argument("Expected an array for \"position\"");

                if (const QJsonValue dirVal = jsonObstacleObj[DIR_KEY]; dirVal.isArray())
                    direction = JsonHelpers::getVec3FromJson(dirVal.toArray());
                else
                {
                    qWarning("Warning: \"direction\" not found for obstacle, using default (%f,%f,%f)", DEFAULT_DIRECTION.x, DEFAULT_DIRECTION.y, DEFAULT_DIRECTION.z);
                    direction = DEFAULT_DIRECTION;
                }

                if (const QJsonValue radVal = jsonObstacleObj[RAD_KEY]; radVal.isDouble())
                    radius = radVal.toDouble();
                else
                    throw std::invalid_argument("Expected a double for \"radius\"");

                auto obj = new Obstacle(nullptr, getNextFreeEntityID(), position, radius);
                addObject(obj);
            }
            catch (std::invalid_argument &e) {
                qWarning("Error loading an obstacle: %s", e.what());
            }
        }
    }
}

int GameState::getNextFreeEntityID()
{
    return nextFreeEntityID++;
}

int GameState::addObject(GameObject *const obj)
{
    objs.push_back(obj);
    return obj->getEntityID();
}

void GameState::removeObject(uint32_t entityID)
{
    for (auto it = objs.begin(); it != objs.end(); ++it)
    {
        if ((*it)->getEntityID() == entityID)
        {
            delete *it;
            objs.erase(it);
            return;
        }
    }
}

GameObject* GameState::getGameObject(uint32_t entityID) const
{
    for (GameObject *const obj : objs)
    {
        if (obj->getEntityID() == entityID)
            return obj;
    }
    return nullptr;
}

GameObject* GameState::getGameObject(GameObjectType type) const
{
    for (GameObject *const obj : objs)
    {
        if (obj->getType() == type)
            return obj;
    }
    return nullptr;
}

std::vector<GameObject*> GameState::getGameObjects(GameObjectType type) const
{
    std::vector<GameObject*> result;
    for (GameObject *const obj : objs)
    {
        if (obj->getType() == type)
            result.push_back(obj);
    }
    return result;
}

std::vector<GameObject*>::const_iterator GameState::begin() const {
    return objs.begin();
}

std::vector<GameObject*>::const_iterator GameState::end() const {
    return objs.end();
}
