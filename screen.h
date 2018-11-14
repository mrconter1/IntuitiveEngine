#ifndef SCREEN_H
#define SCREEN_H

class Screen {
	public:
		SDL_Window* gWindow;
		SDL_Renderer* gRenderer;
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
		int fps;

	Screen(int input_SCREEN_WIDTH, int input_SCREEN_HEIGHT, int input_fps);

	void clearScreen();
	bool init();
};

#endif
