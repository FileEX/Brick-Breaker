# Brick-Breaker
Brick Breaker is a simple 2D game in the Arkanoid style, created using the [SFML](https://github.com/SFML/SFML).

# Gameplay
Brick Breaker is an Arkanoid-style game where players break blocks using a ball and a paddle. The game consists of various levels, and the objective is to destroy all blocks on the board while having three lives. During gameplay, different power-ups and power-downs may drop from the blocks.

List of Power-ups and Power-downs:

- Faster ball
- Slower ball
- Wider paddle
- Narrower paddle
- Gun (cannon)
- Extra life
- Inverted controls
- Fireball
- Frozen paddle
- Shield that prevents losing a life when the ball is lost
- Sticky paddle (glue)
- Additional balls
- Level skip (if only a few blocks remain on the board)

# Screenshoots
![image](https://github.com/user-attachments/assets/a7916090-6b3e-4806-8e00-627a412bbe31)
![image](https://github.com/user-attachments/assets/3340a82d-78e6-404e-827a-15e811db72ab)
![image](https://github.com/user-attachments/assets/23b5aed3-e731-486f-8003-9c0d373fd25f)

# Compilation 
1. Clone repo
```sh
git clone https://github.com/FileEX/Brick-Breaker.git
```
3. Run the ``generate_project.bat`` file.
4. Open the generated .sln file in the ``build`` directory.
5. Set the **Brick_Breaker** project as the startup project (Right-click → 'Set as Startup Project').
(Yeah, I know it might seem dumb, but for some reason, CMake creates an additional ALL_BUILD project and sets it as the startup one.)
6. Compile the project.
7. Everything is ready to run!

# Resources
**Textures:** All textures, except for the backgrounds, were created by me using [Figma](https://figma.com/). The backgrounds come from various no-copyright image websites.

**Sounds:** Sound effects are sourced from [pixabay](https://pixabay.com).

**Music:** The songs come from various sources (mainly YouTube) and are no-copyright melodies. A ``credits.txt`` file in the ``music`` directory contains a list of songs.

