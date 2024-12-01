# Brick-Breaker

**Brick Breaker** is a classic Arkanoid game created using the SFML (Simple and Fast Multimedia Library) framework. The game involves bouncing a ball and breaking blocks that appear on the screen. The player controls a paddle, and the goal is to destroy all the blocks and advance to the next level. The game is based on the [SFML](https://github.com/SFML/SFML) library.

## Features
- Classic Arkanoid-style gameplay.
- Randomly generated power-ups that drop after destroying a block (check list below).
- Randomly playing background songs
- The game is adapted to different resolutions.
- Custom error screen, game over, and level completed.
- A simple level generation system based on an image.
- Advanced power-ups (ball trajectory, cannon etc.)

### Powerups
- Wider paddle
- Narrower paddle
- Extra balls
- Slower ball
- Faster ball
- Extra life
- Cannon/gun
- Glue
- Skip level - only if there are a few blocks left to destroy

## Screenshot
![image](https://github.com/user-attachments/assets/722ceeda-4707-4d36-b0c2-d96fde60de5c)

## Gameplay
The game involves bouncing a ball with a paddle and destroying blocks. A power-up can randomly drop from each block, which may either make the game easier or temporarily harder. Each power-up is temporary. Your goal is to destroy all the blocks.

### Controls
- The paddle is moved by moving the mouse left and right.
- If you've collected a weapon power-up, use the right mouse button (RMB) to shoot.
- If you've lost all your lives or completed the help section, press the spacebar to continue.

## Level generation
Levels are generated based on an image called ``template.png``. This is a simple template that contains a level visualization. Each block is represented by a 2-pixel green (0, 255, 0) border/stroke. In the file **CBricks.cpp**, set **LEVELS_DEBUG_ACTIVE** to **true**, and then run the game. Once the game starts, you can close it. A file named ``level_gen.dat`` will be generated in the main directory. Move it to the ``data/`` directory and rename it to ``level_x.dat``, where ``x`` represents the next number. After generating the file, set **LEVELS_DEBUG_ACTIVE** back to **false**. An example ``template.png`` file can be found [here](https://github.com/FileEX/Brick-Breaker/blob/main/template.png).

## Building the project
### Requirements
1. [CMake](https://cmake.org/download/)
2. SFML 2-6-1 - included in the ``lib/`` directory.

### Compilation
1. Clone the repository:
   ```bash
   git clone https://github.com/FileEX/Brick-Breaker.git
2. Execute ``generate_project.bat`` file in the project directory (make sure you have [CMake](https://cmake.org/download/) installed).
3. Run the ``.sln`` file in the ``build/`` directory and compile the solution.
4. If you want to run the project from Visual Studio using the debugging options (with or without the debugger), you need to set the **Brick_Breaker** project as the startup project (right-click on the project -> **Set as Startup Project**). It's a bit silly, but I don't know how to solve it.

## Resources
### Graphics
All the graphics in the game were created by me using [Figma](https://www.figma.com) and the **svg-repo** plugin. The background is from [vecteezy](https://static.vecteezy.com/system/resources/previews/000/834/435/non_2x/beautiful-space-background-vector.jpg)

### Audio (SFX)
The sound effects used in the game are from the [soundsnap](https://www.soundsnap.com/) and [pixabay](https://pixabay.com/sound-effects/)

### Music
The music comes from various sources, mainly **YouTube**. In the ``music/`` directory, there is a ``credits.txt`` file containing a list of all the songs and their authors.

## Acknowledgement
- [SFML](https://github.com/SFML/SFML) - for providing an amazing C++ library.
- [soundsnap](https://www.soundsnap.com/) and [pixabay](https://pixabay.com/sound-effects/) - for providing an amazing sound effects.
- Everyone listed in **music/credits.txt** - for the wonderful retro-style songs.
- Chirokung for the background image.

### License
The project is licensed under the [MIT](https://opensource.org/license/mit) License, so feel free to edit and share it!
