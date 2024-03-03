#include "gamestate.h"
#include "jsonhelpers.h"

const QString LEVELS_PATH = "/assets/levels/";

const QString PLAYER_KEY = "playerTank";
const QString ENEMY_KEY = "enemyTank";
const QString OBSTACLES_KEY = "obstacles";

const QString POS_KEY = "position";
const QString DIR_KEY = "direction";

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

/**
 * Given a the name of a state file, this method will load the state.
    Note that the state file must be located in the "/assets/levels/"
    directory. Do not include ".json" in the filename.
 * @param filename
 */
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

/**
 * Remove a GameObject from the GameState.
 * @param entityID
 */
void GameState::removeObject(uint32_t entityID)
{

}


