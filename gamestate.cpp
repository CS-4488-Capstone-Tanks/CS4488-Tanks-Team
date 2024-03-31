#include "gamestate.h"
#include "jsonhelpers.h"
#include "Obstacle.h"

const char LEVELS_PATH[] = "assets/levels/";

// Game object keys
const char PLAYER_KEY[] = "playerTank";
const char ENEMY_KEY[] = "enemyTank";
const char OBSTACLES_KEY[] = "obstacles";

// Property keys
const char POS_KEY[] = "position";
const char DIR_KEY[] = "direction";
const char RAD_KEY[] = "radius";

GameState::GameState() {}

void GameState::startState()
{
    foreach (GameObject *const obj, objs) {
        obj->doStart();
    }
}

void GameState::updateState(float deltaTime)
{
    foreach (GameObject *const obj, objs) {
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

            // TODO: construct and add object
            // obj = PlayerTank(position, direction);
            // addObject(obj);
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

            // TODO: construct and add object
            // obj = EnemyTank(position, direction);
            // addObject(obj);
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
                float radius;

                if (const QJsonValue posVal = jsonObstacleObj[POS_KEY]; posVal.isArray())
                    position = JsonHelpers::getVec3FromJson(posVal.toArray());
                else
                    throw std::invalid_argument("Expected an array for \"position\"");

                if (const QJsonValue radVal = jsonObstacleObj[RAD_KEY]; radVal.isDouble())
                    radius = radVal.toDouble();
                else
                    throw std::invalid_argument("Expected a double for \"radius\"");

                addObject(new Obstacle(nullptr, 0, position, radius));
            }
            catch (std::invalid_argument &e) {
                qWarning("Error loading an obstacle: %s", e.what());
            }
        }
    }
}

int GameState::addObject(GameObject *const obj)
{
    obj->setEntityID(nextFreeEntityID);
    objs.push_back(obj);
    return nextFreeEntityID++;
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

std::vector<GameObject*>::const_iterator GameState::begin() const {
    return objs.begin();
}

std::vector<GameObject*>::const_iterator GameState::end() const {
    return objs.end();
}
