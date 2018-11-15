# IntuitiveEngine
A primitive 3D engine made from the ground up using only a 2D drawLineToScreen function.

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
- [ ] One list for all 2d lines instead of separate 2d triangle instances
- [ ] Object Rotational Properties
- [ ] Draw full triangle function
- [ ] Z-Buffer implementation
- [ ] Add more objects to graphics library

Command used to run project:
```
g++ main.cpp player.cpp graphics.cpp screen.cpp scene.cpp -I `pkg-config --cflags --libs sdl2` -lstdc++fs && ./a.out
```
