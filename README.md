# IntuitiveEngine
A primitive 3D engine made from the ground up using only a 2D drawLineToScreen function. It can at the moment project around 10 000 points to the screen using the CPU at 60 FPS.

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
- [ ] Primitive Object Shadowing
- [ ] Optimize Rendering
- [ ] Object Rotational Properties
- [ ] Add more objects to graphics library

Command used to run project:
```
g++ main.cpp player.cpp graphics.cpp screen.cpp scene.cpp -I `pkg-config --cflags --libs sdl2` -lstdc++fs && ./a.out
```
Gameplay:
![](game.gif)
