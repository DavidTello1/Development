# GRIP OR SLIP
This is a little game made by two second year students of the Videogame Design and Development degree. It's a 2D platformer with two levels where the player must change between maps to complete the game. It has been made using C++, SDL and pugi libraries. The maps have been made using Tiled.

### Controls
-Movement with WASD keys: A (left), D (right), W (up & grip), S (down)

-Jump with Space.

-Attack with M.

-Change Map with N.

-Pause Game with ESC.

-Go to Main Menu after Game Over with Space.

-Quit Game after Game Over with ESC.

### Tips
You can jump after attacking or falling from a grid, box or the ground.

To jump from walls you must stop sliding.

Boxes have opposite positions in each map so, if you think you can't continue, try changing the map.

While gripped to a box you can only jump and only after it has stopped moving.


### Debug Controls
F1: Load the first level. F2: Restart the current level. F3: Load the second level. F5: Load the saved game. F6: Save the current game. F8: Debug UI. F9: View collider logic. F10: Activate/Deactivate God Mode. F11:Activate/Deactivate FrameCap.


### Innovation Features
-Moving Grids that the player can grip to and move.

-Land Enemy sprite stays in map after death.

-Boxes opposite position to its pair in the other map.

-Save both map entities at the same time.

-Rotation of maps.

-Wall Jumping.

-Dash Attack.

-Time color changes to red when there are 60 or less seconds remaining.


### Authors
-David Tello Panea

-Marc Garcia Reig


### Webpage
https://davidtello1.github.io/Development/


### Disclosure
We do not own any of the sprites or tilesets presented in this game.

-Player Texture: The Legendary Starfy 4 https://www.spriters-resource.com/ds_dsi/thelegendarystarfy4/sheet/13166/

-Tileset: New Super Mario Bros https://www.spriters-resource.com/ds_dsi/newsupermariobros/sheet/8415/

-Backgrounds: New Super Mario Bros https://www.spriters-resource.com/ds_dsi/newsupermariobros/sheet/26066/

-Land Enemy: The Legendary Starfy https://www.spriters-resource.com/ds_dsi/legendarystarfy/sheet/37442/

-Flying Enemy: The Legendary Starfy 2 https://www.spriters-resource.com/game_boy_advance/thelegendarystarfy2/sheet/26327/

-GUI: Kenney https://opengameart.org/content/platformer-art-deluxe , https://opengameart.org/content/ui-pack-rpg-extension

-Game Over: http://www.allfinweb.com/single/113513976-game-over-1920x1080.html

-Main Menu Background: http://in2gpu.com/category/game-development/

###Accomplishments

- David Tello: EntitySystem, Player(Attack, animations, godmode), Grid, Enemies (without pathfinding), Brofiler, UML, Map Rotation

- Marc Garcia: FrameCap, Pathfinding, Enemies pathfinding, Normalized movement.


## License
### MIT License

Copyright (c) 2018 David Tello, Marc Garcia

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
