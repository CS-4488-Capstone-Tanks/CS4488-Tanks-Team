//
// Created by lunah on 4/17/2024.
//

#ifndef CS4488_TANKS_TEAM_SFX_MANAGER_H
#define CS4488_TANKS_TEAM_SFX_MANAGER_H

#include <QSoundEffect>
#include <QObject>

class SFXManager: public QSoundEffect {
Q_OBJECT
signals:
    void playExplosion();
    void playTankFire();
    void playTankTread();
    void playCollide();
    void stopTankTread();
public:
    SFXManager();
    ~SFXManager() = default;
    void manualExplosion(); // Manual functions in case slots fail
    void manualTankFire();
    void manualTankTread();
    void manualCollide();
    void manualStopTankTread();

private:
    QSoundEffect explosion;
    QSoundEffect tankFire;
    QSoundEffect tankTread;
    QSoundEffect collide;

};


#endif //CS4488_TANKS_TEAM_SFX_MANAGER_H
