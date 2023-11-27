<h1 align="center"> 2D Game Engine in C++</h1>
This is a barebone generic 2d engine that I am working on. Much of the architecture is based
on the examples provided in Game Programming in C++ by Sanjay Madhav. 

It uses SDL2 for the handling of sprites, textures, and UI components.

The audio system uses FMOD for sound events and music.

<h3 align="left">What I am currently developing:</h3>
<p>Currently I am working on implementing a more robust collision system. The requirements for this are going to be building components for differnt actors and game objects. For example, I am working on a top down RPG so there will be tiles which are not traverable. I will need to implement collision detection for players, NPCs, and projectiles on these tiles.<br><br> Then once there is collision detection, I can program the AI to avoid these tiles in their pathing algorithm. The exact implementation will be gam-by-game an won't be included in this general package. To see how I implement these features in a specific scenario please follow my <a href="https://github.com/Zivarius">Github page</a> for when I upload proof of concepts.</p>
