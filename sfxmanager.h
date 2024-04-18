//
// Created by lunah on 4/17/2024.
//

#ifndef CS4488_TANKS_TEAM_SFX_MANAGER_H
#define CS4488_TANKS_TEAM_SFX_MANAGER_H

#include <QSoundEffect>
#include <QObject>

class SFXManager  {

public:
    enum class Sounds {
        Explosion,
        Firing,
        PlayerTreads,
        EnemyTreads,
        Collision
    };
private:
    std::unordered_map<Sounds, QSoundEffect*> sounds;
public:

    SFXManager();
    ~SFXManager();

    void playSound(Sounds sound);
    void stopSound(Sounds sound);
};


#endif //CS4488_TANKS_TEAM_SFX_MANAGER_H
