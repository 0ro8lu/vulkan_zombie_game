# Simple Vulkan Zombie Game
This repo is just a big experiment to hone my skills in Vulkan.

The game is extremely simple and takes HEAVY inspiration from challenge N.1 of [Making Games With Ben](https://www.youtube.com/playlist?list=PLSPw4ASQYyymu3PfG9gxywSPghnSMiOAW)
Ben, i know you're never going to see this, but i miss you. Thank you for inspiring me. 10 long years have passed since i was a little kid watching your videos, dreaming of being a developer.
Well, i'm trying to make it, one small step at a time, and... you know, stuff has changed since 2014: OpenGL's kinda dead, Vulkan seems to be the way to go so, i'm pretty proud of myself for learning this from scratch.
Hopefully some 3D demos are gonna be coming soon ^^. 

Anywho, you're here for the game ain't ya, curious traveler.

## The game
You shoot and evade zombies. Try not to kill your fellow humans along the way ^^ The end.
Currently i only lazily made two levels, its just a """tech demo""" for me.

![Alt text](/screenshots/win.png?raw=true "Optional Title") 
![Alt text](/screenshots/loose.png?raw=true "Optional Title") 
![Alt text](/screenshots/close.png?raw=true "Optional Title")

# TODO:
- [ ] Having to send vertex data every frame is stupid. we can define that in the shader code.
- [X] Make the new textures for walls and zombies
- [ ] Create camera controller for moving around, zooming in and out.
- [X] Remove color data from vertex and shader code.
- [X] Put all Vulkan-related code in a separate file.
- [ ] Implement game logic.
- [X] Try out instancing instead of making a billion draw commands.
- [X] Make the input class an interface and have static (non-dynamic) dispatch, research C++20's concept.
- [X] Implement controller support.
- [ ] Remove GLFW and roll with custom lean windowing solution.
