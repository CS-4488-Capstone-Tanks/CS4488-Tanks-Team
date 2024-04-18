//
// Created by lunah on 4/17/2024.
//

#include "sfxmanager.h"

SFXManager::SFXManager(): explosion(this), tankFire(this), tankTread(this), collide(this){
    explosion.setSource(QUrl::fromLocalFile("./assets/sfx/explosion.wav"));
    explosion.setVolume(0.5f);

    tankFire.setSource(QUrl::fromLocalFile("./assets/sfx/tankFire.wav"));
    tankFire.setVolume(0.5f);

    tankTread.setSource(QUrl::fromLocalFile("./assets/sfx/tankTread.wav"));
    tankTread.setLoopCount(QSoundEffect::Infinite);
    tankTread.setVolume(1.0f);

    collide.setSource(QUrl::fromLocalFile("./assets/sfx/collide.wav"));
    collide.setVolume(0.25f);

    connect(this, &SFXManager::playExplosion, &explosion, &QSoundEffect::play);
    connect(this, &SFXManager::playTankFire, &tankFire, &QSoundEffect::play);
    connect(this, &SFXManager::playTankTread, &tankTread, &QSoundEffect::play);
    connect(this, &SFXManager::playCollide, &collide, &QSoundEffect::play);
    connect(this, &SFXManager::stopTankTread, &tankTread, &QSoundEffect::stop);
}

void SFXManager::manualExplosion() {
    explosion.play();
}

void SFXManager::manualTankFire() {
    tankFire.play();
}

void SFXManager::manualTankTread() {
    if (tankTread.isPlaying()) { return; }
    else{ tankTread.play(); }
}

void SFXManager::manualStopTankTread() {
    tankTread.stop();
}

void SFXManager::manualCollide() {
    return collide.play();
}
