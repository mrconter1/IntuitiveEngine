# IntuitiveEngine

Imagine yourself being stranded on a deserted island. Your only means of computing is a computer capable of compiling C++ and a single function that is capable of drawing a line between two points on the screen. What do you do if you want to play Minecraft?

## Quick Description 

This primitive 3D engine is made from the ground up using only a 2D drawLineToScreen function and native C++. It can at the moment project around 10 000 3D points to the screen using the CPU at 60 FPS. This engine was created using almost only my own intuition on 3D projection. A simple function like filling a triangle took a couple of days and I later learned that this is a very studied problem in computer graphics. It would have been preferable to use an even more primitive function that just drew a single pixel but that was not possible due to performance issues. But using a setPixel function would only require one to implement the drawLineOnScreen which would be trivial. You could even go as far as replacing setPixel with writing directly to the screen buffer in Linux.

## Work flow

The only "cheating" I did was to use the understanding of conversion between cartesian coordinates to spherical coordinates which I have learned in class. Other than that, I figured out everything myself. My workflow/development went something like this:
1. Create a 3D coordinate system
2. Render a 3D point by converting x, y, z to theta, phi using spherical coordinates
3. Let a portion of theta and phi represent my field of view and therefore my physical 2D screen
4. Yaw was achieved by simply rotating all points around the z-axis
5. I think I achieved pitch by rotating everything around the player using a virtual cylinder. This cylinder offset was then remembered and applied before each rendering. I think this was the most difficult aspect of transformations.

This project took a couple of weeks at least. When I went from wireframe rendering to filling the triangles I think my frame rate dropped by 99.9% if I do not remember incorrectly. I went from being able to render around 400 wireframe cubes to almost one cube. So there has been a lot of optimization done as well.

## Features

- [x] Correct XYZ movement
- [x] Smooth Mouse input
- [x] Multiple Keypresses
- [x] Fix mirror drawing bug
- [x] Pass Objects with Pass by reference
- [x] Add render time measuring/sleep for smoother fps
- [x] Allow 3D point rotation around a point
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

## Demonstration

![](peek.gif)

Command used to run project:
```
g++ main.cpp player.cpp graphics.cpp screen.cpp scene.cpp -I `pkg-config --cflags --libs sdl2` -lstdc++fs && ./a.out
```


