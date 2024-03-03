#include "gamestate.h"
#include "jsonhelpers.h"

const char LEVELS_PATH[] = "/assets/levels/";

const char PLAYER_KEY[] = "playerTank";
const char ENEMY_KEY[] = "enemyTank";
const char OBSTACLES_KEY[] = "obstacles";

const char POS_KEY[] = "position";
const char DIR_KEY[] = "direction";

GameState::GameState()
{

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
        obj->doUpdate(deltaTime);
    }
}

void GameState::loadState(QString filename)
{
    QFile stateFile(LEVELS_PATH + filename + ".json");
    QByteArray stateData = stateFile.readAll();
    QJsonDocument saveDoc(QJsonDocument::fromJson(stateData));
    QJsonObject jsonObj = saveDoc.object();

    if (const QJsonValue v = jsonObj[PLAYER_KEY]; v.isObject())
    {
        QJsonObject jsonPlayerObj = v.toObject();

        vec3 position;
        vec3 direction;

        if (const QJsonValue posVal = jsonPlayerObj[POS_KEY]; posVal.isArray())
            position = JsonHelpers::getVec3FromJson(posVal.toArray());
        else
            qWarning("Expected an array for \"position\"");

        if (const QJsonValue dirVal = jsonPlayerObj[DIR_KEY]; dirVal.isArray())
            direction = JsonHelpers::getVec3FromJson(dirVal.toArray());
        else
            qWarning("Expected an array for \"direction\"");

        // TODO: construct and add object
        // obj = PlayerTank(position, direction);
        // addObject(obj);
    }

    if (const QJsonValue v = jsonObj[ENEMY_KEY]; v.isObject())
    {
        QJsonObject jsonEnemyObj = v.toObject();
        // TODO
    }

    if (const QJsonValue v = jsonObj[OBSTACLES_KEY]; v.isArray())
    {
        const QJsonArray jsonObstacleObjs = v.toArray();
        for (const QJsonValue &obsVal : jsonObstacleObjs)
        {
            QJsonObject jsonObstacleObj = obsVal.toObject();
            // TODO
        }
    }
}

void GameState::addObject(GameObject *const obj)
{
    obj->setEntityID(nextFreeEntityID++);
    objs.push_back(obj);
}

void GameState::removeObject(uint32_t entityID)
{

}


