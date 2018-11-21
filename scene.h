#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>
#include <list>
#include "player.h"
#include "graphics.h"

class Scene {
	public:
		Player * player;
		Screen * screen;
		std::list<Object*> objectList;
		
		//Rendering Time Variables
		float renderTime;
		int collectAverageRenderTime;
		float * frameTimes;
		int frameIndex;

		//Render margin error used to only render things within view
		int renderMargin;
		float drawDistance;

	//Constructor that takes a pointer to the player instance as paramter
	Scene(Player * inputPlayer, Screen * inputScreen);
	//Function that renders the scene
	void renderScene();
	//Returns a list with 2d vertex's to render
	std::list<Triangle> renderQueue();
	//Add a object that consists of vertices to the scene
	void addObject(Object * object);
	//Rotate an object
	void rotateScene(float thetaAdd, float phiAdd);
	//Returns render time
	float getRenderTime();
	//Returns average render time for x latest frames
	float getAverageRenderTime(int numOfFrames);
	//Returns distance between object and player
	float getDistToPlayer(Object * inputObject);
};

#endif
