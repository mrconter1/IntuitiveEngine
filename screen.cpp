#include <SDL.h>
#include "screen.h"

//-----Screen Class-----
Screen::Screen(int input_SCREEN_WIDTH, int input_SCREEN_HEIGHT, int input_fps) {
	gWindow = NULL;
	gRenderer = NULL;
	SCREEN_WIDTH = input_SCREEN_WIDTH;
	SCREEN_HEIGHT = input_SCREEN_HEIGHT;
	fps = input_fps;
	Screen::init();
}

void Screen::clearScreen() {
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00 );
	SDL_RenderClear(gRenderer);
}

bool Screen::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Intuitive Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//SDL_SetWindowFullscreen(gWindow, 1);
			SDL_ShowCursor(SDL_DISABLE);

			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			
				SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			}
		}
	}

	return success;
}


