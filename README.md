# IntuitiveEngine

Imagine yourself being stranded on an deserted island. Your only means of computing is a computer capabale of compile C++ and a single function that is capable of drawing a line between two points on the screen. What do you do if you want to play Minecraft?

## Quick Description 

This primitive 3D engine made from the ground up using only a 2D drawLineToScreen function and native C++. It can at the moment project around 10 000 points to the screen using the CPU at 60 FPS. This engine was created using almost only my own intuation on 3D projection. A simple function filling a triangle took a couple of days and I later learned that this is a very studied problem in computer graphics.

## Features

- [x] Correct XYZ movement
- [x] Smooth Mouse input
- [x] Multiple Keypresses
- [x] Fix mirror drawing bug
- [x] Pass Objects with Pass by reference
- [x] Add render time measuring/sleep for smoother fps
- [x] Allow 3D point rotation around point
- [x] Camera rotation
- [x] Mouse Support
- [x] Split project into modules
- [x] Start using Github
- [x] One draw call for each triangle
- [x] Draw full triangle function
- [x] Object Spatial, Solid, Visibility and Color properties
- [x] Z-Buffer implementation
- [x] Only render things approximately inside view
- [x] Optimize Rendering

![](peek.gif)

Command used to run project:
```
g++ main.cpp player.cpp graphics.cpp screen.cpp scene.cpp -I `pkg-config --cflags --libs sdl2` -lstdc++fs && ./a.out
```
