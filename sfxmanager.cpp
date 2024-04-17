//
// Created by lunah on 4/17/2024.
//

#include "sfxmanager.h"

SFXManager::SFXManager(): explosion(this), tankFire(this), tankTread(this), collide(this){
    explosion.setSource(QUrl::fromLocalFile(QFile(./resources/sfx/explosion.wav).absoluteFilePath()));
    explosion.setVolume(0.5f);

    tankFire.setSource(QUrl::fromLocalFile(QFile(./resources/sfx/tankFire.wav).absoluteFilePath()));
    tankFire.setVolume(0.5f);

    tankTread.setSource(QUrl::fromLocalFile(QFile(./resources/sfx/tankTread.wav).absoluteFilePath()));
    tankTread.setLoopCount(QSoundEffect::Infinite);
    tankTread.setVolume(0.25f);

    collide.setSource(QUrl::fromLocalFile(QFile(./resources/sfx/collide.wav).absoluteFilePath()));
    collide.setVolume(0.25f);

    connect(&explosion, &QSoundEffect::play, this, &SFXManager::playExplosion);
    connect(&tankFire, &QSoundEffect::play, this, &SFXManager::playTankFire);
    connect(&tankTread, &QSoundEffect::play, this, &SFXManager::playTankTread);
    connect(&collide, &QSoundEffect::play, this, &SFXManager::playCollide);
}

int SFXManager::manualExplosion() {
    return explosion.play();
}

int SFXManager::manualTankFire() {
    return tankFire.play();
}

int SFXManager::manualTankTread() {
    if tankTread.isPlaying() {
        return tankTread.stop();
    }
    return tankTread.play();
}

int SFXManager::manualCollide() {
    return collide.play();
}
