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
- [x] Object Spatial Properties
- [x] One draw call for each triangle
- [ ] Draw full triangle function
- [ ] Z-Buffer implementation
- [ ] Object Color Property
- [ ] Object Rotational Properties
- [ ] Add more objects to graphics library

Command used to run project:
```
g++ main.cpp player.cpp graphics.cpp screen.cpp scene.cpp -I `pkg-config --cflags --libs sdl2` -lstdc++fs && ./a.out
```
