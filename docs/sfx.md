# Sound Effects
Sound Effects in the game are handled by the SFXManager class. The SFXManager
class loads several sounds including Explosion, Firing, PlayerTreads, EnemyTreads,
and Collision by creating them as QSoundEffects. The sound effects system doesn't 
handle any music, as QMultimedia would be better for that. Sounds must be saved as
.wav files in /assets/sfx.

### Internal Details
The class should probably be made a singleton, but the current implementation is
such that wherever you want a sound effect to play (player moving, enemy moving,
firing, collision detections e.g.) you add #include sfxmanager.h to the header file,
create an instance of it, and then use playSound(sound).

Sounds is an unordered map of Sounds to QSoundEffects. It is used in playSound and stopSound.

### Methods
SFXManager(), ~SFXManager() - constructor and destructors
playSound(Sounds sound) - Play a sound. If the sound is already playing, don't play another.
stopSound(Sounds sound) - Stop a sound from playing.
sound: The name of the sound you would like to play/stop, e.g. Explosion, Collision