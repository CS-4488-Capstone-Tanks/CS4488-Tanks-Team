# Scene

_Primary Author: Koda Koziol, SPRING 2024_

The scene class defined in [scene.h](../scene.h) and [scene.cpp](../scene.cpp) holds and manages the game objects in a virtual scene. It is responsible for loading (and saving later) the state of the game. It also updates all game objects in the scene.

Be aware that this is a singleton class, so you should not create an instance of it, but instead use the `getInstance()` method to get the instance of the Scene. Also be aware that this class is not currently thread-safe.

## Creating and setting the Scene

This section is basically the TL;DR of the Scene class.

To create a scene, you can use the `getInstance()` method to get the instance of the Scene. This will create a new Scene if one does not already exist, and return a reference to the existing Scene if one does exist.

Setting a scene from scratch will involve these steps:
- Get the instance of the Scene using `getInstance()`
- Reset the Scene using the [`reset()`](#resetting-the-scene) method, which will clear any existing GameObjects and reset the `nextFreeEntityID` counter to 0.
- Load GameObjects from a file(s) using the [`load()`](#loading-the-scene-from-file) method.
- Add any additional GameObjects to the scene using the [`addObject()`](#adding-gameobjects-to-the-scene) method (you should avoid this, [it's what scene state files are for](#loading-the-scene-from-file)).
- Call `start()` on the scene to initialize GameObjects.
- Call `setPaused(false)` if the scene isn't already unpaused.

At this point, the Scene has been reset, loaded with gameobjects, has initialized them, and is ready to start the `update()` loop.

## Resetting the Scene

The `reset()` method can be used to clear the scene of all GameObjects and reset the `nextFreeEntityID` counter to 0. This method should be called before loading a new scene scratch, say for a new level.

## Adding GameObjects to the Scene

GameObjects are added to the scene using the `addObject()` method. The Scene assumes that every GameObject added has a unique entityID, and you should use the `getNextFreeEntityID()` method on the Scene to generate a unique id. No checking is done to ensure that the GameObject's entityID is unique before it is added though, so use a custom entityID at your own risk.

The `load()` method can also be used to add GameObjects to the scene from a file. See the [Loading the Scene from file](#loading-the-scene-from-file) section for more information.

## Getting GameObjects from the Scene

There are multiple methods that can be used to get GameObjects from the scene, depending on your use case.

- `getGameObject(entityID)` - Returns a reference to the GameObject with the given entityID
- `getGameObject(GameObjectType)` - Returns a reference to the first GameObject with the given GameObjectType. This should probably only be used if you know there is only one GameObject of that type in the scene.
- `getGameObjects(GameObjectType)` - Returns a vector of references to all GameObjects with the given GameObjectType.
- `begin()` and `end()` - Returns iterators to the beginning and end of the GameObjects in the scene. This can be used to iterate over all GameObjects in the scene.

## Removing GameObjects from the Scene

GameObjects are removed from the scene using the `removeObject()` method. Given an entityID, this method will remove the GameObject with that id from the scene and also delete the GameObject from memory.

`clearScene()` can be used to remove all GameObjects from the scene. This will also delete all GameObjects from memory, and reset the `nextFreeEntityID` counter to 0.

## Getting Map Size

There are two methods, `getXLength()` and `getZLength()`, that return the size of the map in the scene. This is useful for determining the bounds of the map when setting GameObject positions.

Note that currently, the renderer is not actually using these values to determine the size of the ground drawn, but instead has hardcoded values in [`renderer.h`](../renderer.h). In fact, as of writing, these methods and propertied are not actually used anywhere in the codebase, so what these values mean is undefined.

## The Scene Loop

The scene loop was modeled after other game engines. The structure is, after any GameObjects are loaded from file, the `start()` method is called once, then the `update()` method is called regularly (usually every frame), for the rest of the lifetime of the Scene.
- `start()` - This method allows GameObjects to initialize themselves before the scene begins updating, and it should be once at the start of the scene. Precisely, this method will call the start() method for each GameObject in the Scene.
- `update()` - This method updates the states of all GameObjects in the Scene by calling update() on them, and then checking for collisions. It will also remove any GameObjects that are queued for destruction. This method should be called regularly (usually once per frame) through the duration of a scene.
- `setPaused(bool)` - This method allows the Scene to be paused or unpaused. When paused, the Scene will not call `update()`. It's worth noting though that last I checked, pausing is being handled by stopping the timer that calls update() in `game.cpp`, so currently this method appears to be redundant.

## Loading the Scene from file

The `load(std::string filename)` method can be used to load GameObjects and scene properties from a file. Scene state files should be stored in `assets/levels/` and should be in the format of a JSON file. The `filename` parameter should be the name of the file without the `.json` extension.

Note that the `load()` method will NOT call `reset()` before loading the scene from file. This allows for loading level state from multiple files, but it also means that you should call `reset()` before loading a new scene from scratch.

Here is an example scene state file:
```json
{
    "playerTank": {
        "position": [1, 0, 3],
        "direction": [0, 0, 1]
    },
    "enemyTank": {
        "type": "scrub",
        "position": [0, 0, 50],
        "direction": [0, 0, -1]
    },
    "obstacles": [
        {
            "type": "Boulder",
            "position": [-2, 0, 0],
            "radius": 1
        },
        {
            "type": "Boulder",
            "position": [-3, 0, 0],
            "radius": 0.5
        },
        {
            "type": "Tree",
            "position": [7, 0, 0],
            "radius": 0.5
        }
    ],
    "mapProperties":{
        "XLength": 30.0,
        "ZLength": 30.0
    }
}
```

Note that all vectors (like `position` and `direction`) are in the format of `[x, y, z]`. Ideally, `direction` should be a unit vector, but since it is normalized on assignment to the GameObject anyway, it technically doesn't have to be.

### Loading map properties

The map properties are loaded from the `mapProperties` object in the JSON file. The `XLength` and `ZLength` numeral properties are used to set the size of the map in the Scene.

### Loading the player tank

The player tank is loaded from the `playerTank` object in the JSON file. The `position` vector property is used to set the position of the player tank, and the `direction` vector property is used to set the direction of the player tank.

### Loading the enemy tank

The enemy tank is loaded from the `enemyTank` object in the JSON file. The `position` vector property is used to set the position of the enemy tank, and the `direction` vector property is used to set the direction of the enemy tank. The `type` string property hasn't actually been implemented yet, but it is intended to be used to set the type of enemy tank.

### Loading obstacles

Obstacles are loaded from the `obstacles` array in the JSON file. Each object in the array should have a `type`, `position`, and `radius` property. The `type` string property is used to determine what type of obstacle to create, the `position` vector property is used to set the position of the obstacle, and the `radius` numeral property is used to set the radius of the obstacle's collider.

## Potential improvements for the future

- Most of the code in `load()` should probably be extracted into its own class dedicated to building GameObjects from file, so this method can just add them to the Scene.
- Set up an architecture to load different parts of the scene from different files. For example, one file for the map/environment, one file for the player, one file for the enemies, etc. That way they can be easily swapped out or mixed and matched.
- Save the scene state to a file, so the game can be saved and loaded later.
- Currently, to trigger game over, a GameObject calls a method on the `Game()`. It might be better to isolate the GameObjects from the Game class, and have the Scene have its own method to trigger game over.
- Add more properties for initializing GameObjects, like scale, or direction for the obstacles.