//
// Created by lunah on 4/17/2024.
//

#ifndef CS4488_TANKS_TEAM_SFX_MANAGER_H
#define CS4488_TANKS_TEAM_SFX_MANAGER_H

#include <QSoundEffect>
#include <QObject>

class SFXManager: QSoundEffect {
Q_OBJECT
public signals:
    playExplosion();
    playTankFire();
    playTankTread();
    playCollide();
public:
    SFXManager();
    ~SFXManager() = default;
    manualExplosion(); // Manual functions in case slots fail
    manualTankFire();
    manualTankTread();
    manualCollide();

private:
    QSoundEffect explosion;
    QSoundEffect tankFire;
    QSoundEffect tankTread;
    QSoundEffect collide;

};


#endif //CS4488_TANKS_TEAM_SFX_MANAGER_H
