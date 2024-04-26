# PlayerTank
The player tank class is responsible for the logic of the players tank. While the 
controls are partly outsourced, the main logic still lives here. Much of the logic
is inherited from other classes, such as game object and the abstract tank class.

### Player Tank logic flow
Every tick, doUpdate is called. When called, the tank will reference a boolean
table that tells which keys are pressed. The will then update the position or
rotation of the tank based on the values in the boolean table. The values in 
the boolean table are updated based on key events received from gamewindow.

### Enemy Tank logic flow
The enemy tank is always moving in the direction of the player. More 
sophisticated logic could be added, but this initial implementation allows the game
to be playable and have the instant-win strategy of hiding behind a boulder to allow
the enemy to drive into it.

### Shooting
The shots are restricted based on a shot accumulator that is based on ticks
and incremented in doUpdate. When shoot is called, a check is done to see if 
enough time has passed for another shot to be fired. If a shot can be fired, 
then a new projectile is added to the game with the same direction vec3 as 
the tank.

### Collision
Any collision with the PlayerTank will result in a game over. In this initial
implementation, we didn't have the time to implement more complex logic, so
we decided it would be a game over for every type of collision. On a PlayerTank
collision, it is game over.