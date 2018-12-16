*Grip or Slip* is a little game made by two second year students of the **Videogame Design and Development** degree during the *Videogame Development* subject. It's a 2D platformer with two levels where the player must change between maps to complete the game. It has been made using C++, SDL and pugi libraries. The maps have been made using Tiled.

**TEAM MEMBERS**

<img src="https://drive.google.com/open?id=1SwIo5wclkMEWYVq_dS7QHXGA_UmMk6Nw">

[David Tello](https://github.com/DavidTello1)

[Marc Garcia](https://github.com/marcgreig)

**INSTRUCTIONS TO PLAY**

-Movement with WASD keys: A (left), D (right), W (up & grip), S (down)

-Jump with Space.

-Attack with M.

-Change Map with N.

-Pause Game with ESC.

-Go to Main Menu after Game Over with Space.

-Quit Game after Game Over with ESC.

**TIPS & TRICKS**

-You can jump after attacking or falling from a grid, box or the ground.

-To jump from walls you must stop sliding.

-Boxes have opposite positions in each map so, if you think you can't continue, try changing the map.

-While gripped to a box you can only jump and only after it has stopped moving.

**DEBUG KEYS**

-F1: Load the first level.

-F2: Restart the current level.

-F3: Load the second level.

-F5: Load the saved game.

-F6: Save the current game.

-F8: Debug UI.

-F9: View collider logic.

-F10: Activate/Deactivate God Mode.

-F11:Activate/Deactivate FrameCap.

**CORE SUBSYSTEMS**

This game has 5 main modules:

**Entity System** keeps and manages all entities such as the player, the enemies and the objects. This module makes sure all entities do correctly their logic.

The module **Pathfinding** is connected to the entity system module since it allows the entities to correctly create a path to their objective.

The **Map** which reads .tmx files that hold all information of the tile based level (done in the Tiled map editor)such as position and texture of each tile, collisions and entities' position.

The **UI** with a polymorphism system of UI elements that can be created and managed by the GUI module and executed from the scene module

Last but not least there's the **Scene Module**, which is in charge of sending to the renderer the current level, menu and entities on screen as well as managing the transitions and some interactions between them.

**INNOVATION**

-Moving Grids that the player can grip to and move.

-Static Boxes that the player can hop into and move following the box's path.

-Land Enemy sprite stays in map after death.

-Boxes opposite position to its relative in the other map.

-Save both map entities at the same time.

-Rotation of maps.

-Wall Jumping.

-Dash Attack.

-Time color changes to red when there are 60 or less seconds remaining.

**Grip or Slip's github [link](https://github.com/DavidTello1/Development)**

**Download last version of [Grip or Slip]()** //Needs link to latest release

**MIT License**

Copyright (c) 2018 DavidTello1

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
