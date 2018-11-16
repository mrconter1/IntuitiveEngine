#include <SDL.h>
#include <stdio.h>
#include <string>

#include "player.h"
#include "graphics.h"
#include "scene.h"
#include "screen.h"

//Custom object
Object CreateBox(float x, float y, float z, float size) {

	Object box;

	//Front and back
	box.addVertex(	{x, y, z},
				{x, y + size, z},
				{x, y, z - size});
	
	box.addVertex(	{x, y, z - size},
				{x, y + size, z - size},
				{x, y + size, z});

	box.addVertex(	{x + size, y, z},
				{x + size, y + size, z},
				{x + size, y, z - size});
	
	box.addVertex(	{x + size, y, z - size},
				{x + size, y + size, z - size},
				{x + size, y + size, z});

	//Left and right
	box.addVertex(	{x, y, z},
				{x + size, y, z},
				{x, y, z - size});
	
	box.addVertex(	{x, y, z - size},
				{x + size, y, z - size},
				{x + size, y, z});

	box.addVertex(	{x, y + size, z},
				{x + size, y + size, z},
				{x, y + size, z - size});
	
	box.addVertex(	{x, y + size, z - size},
				{x + size, y + size, z - size},
				{x + size, y+ size, z});

	//Bottom and top
	box.addVertex(	{x, y, z},
				{x + size, y, z},
				{x, y + size, z});
	
	box.addVertex(	{x + size, y + size, z},
				{x + size, y, z},
				{x, y + size, z});

	box.addVertex(	{x, y, z - size},
				{x + size, y, z - size},
				{x, y + size, z - size});
	
	box.addVertex(	{x + size, y + size, z - size},
				{x + size, y, z - size},
				{x, y + size, z - size});

	return box;
}

int main( int argc, char* args[] )
{

	//Screen instance
	Screen screen(1280, 720, 60);

	//Creates player instance
	Player player(&screen);
	
	//Create scene instance with pointer to player instance as parameter
	Scene scene(&player, &screen);

	//Draw random scene
	srand (time(NULL));

	int boxes = 2;
	Object box[boxes][boxes][boxes];

	for (int x = 0; x < boxes; x++) {
		for (int y = 0; y < boxes; y++) {
			for (int z = 0; z < boxes; z++) {
				box[x][y][z] = CreateBox(x + rand() % 10, y + rand() % 10, z + rand() % 10, 1);
				box[x][y][z].setColor(rand() % 256, rand() % 256, rand() % 256);
				scene.addObject(&box[x][y][z]);
			}
		}
	}
	
	//Start up SDL and create window
	if(!screen.init())
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//While application is running
		while( !quit )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT ) quit = true;

			}

			//Update player
			player.update();

			//Render scene
			scene.renderScene();

			//Prints render time
			printf("Average render time: %f\n", scene.getAverageRenderTime(120));

		}
	}

	return 0;
}







