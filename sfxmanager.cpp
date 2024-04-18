//
// Created by lunah on 4/17/2024.
//

#include "sfxmanager.h"
#include <iostream>


SFXManager::SFXManager() {
    sounds = {
        {Sounds::Explosion, new QSoundEffect()},
        {Sounds::Firing, new QSoundEffect()},
        {Sounds::PlayerTreads, new QSoundEffect()},
        {Sounds::EnemyTreads, new QSoundEffect()},
        {Sounds::Collision, new QSoundEffect}
    };

    sounds[Sounds::Explosion]->setSource(QUrl::fromLocalFile("./assets/sfx/explosion.wav"));
    sounds[Sounds::Explosion]->setVolume(0.5f);

    sounds[Sounds::Firing]->setSource(QUrl::fromLocalFile("./assets/sfx/tankFire.wav"));
    sounds[Sounds::Firing]->setVolume(0.5f);

    sounds[Sounds::PlayerTreads]->setSource(QUrl::fromLocalFile("./assets/sfx/tankTread.wav"));
    sounds[Sounds::PlayerTreads]->setLoopCount(QSoundEffect::Infinite);
    sounds[Sounds::PlayerTreads]->setVolume(1.0f);

    sounds[Sounds::EnemyTreads]->setSource(QUrl::fromLocalFile("./assets/sfx/tankTread.wav"));
    sounds[Sounds::EnemyTreads]->setLoopCount(QSoundEffect::Infinite);
    sounds[Sounds::EnemyTreads]->setVolume(1.0f);

    sounds[Sounds::Collision]->setSource(QUrl::fromLocalFile("./assets/sfx/collide.wav"));
    sounds[Sounds::Collision]->setVolume(0.25f);

}

void SFXManager::playSound(SFXManager::Sounds sound) {
    QSoundEffect* sfx = sounds.at(sound);

    if (!sfx->isPlaying()) {
        sfx->play();
    }
}

void SFXManager::stopSound(SFXManager::Sounds sound) {
    QSoundEffect* sfx = sounds.at(sound);

    if (sfx->isPlaying()) {
        sfx->stop();
    }
}

SFXManager::~SFXManager() {
    for(auto& pair : sounds) { delete pair.second; }
    sounds.clear();
}



