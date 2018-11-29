#include <SDL.h>
#include <list>
#include <cmath>
#include <chrono>
#include <stdio.h>
#include "scene.h"
#include "player.h"
#include "graphics.h"

#define PI 3.14159265358	

//-----Scene Class-----
//Constructor that takes a pointer to the player instance as paramter
Scene::Scene(Player * inputPlayer, Screen * inputScreen) {
	player = inputPlayer;
	screen = inputScreen;

	//Rendering Time Variables
	renderTime = 0.0f;
	collectAverageRenderTime = 0;
	frameIndex = 0;

	//Only render objects inside drawing distance
	drawDistance = 1000.0f;
	
}

//Renders scene
void Scene::renderScene() {

	//Measure time before
	auto start_time = std::chrono::high_resolution_clock::now();

	//Clear screen
	screen->clearScreen();

	//Retrieve a list of objects to render
	std::list<Triangle> objectsToRender = renderQueue();

	//Iterate through each render object
	for (auto &triangle : objectsToRender) {
		triangle.drawLineTriangle(screen->gRenderer);
	}

	//Update screen
	SDL_RenderPresent(screen->gRenderer);

	//Calculate total render time
	auto tempRenderTime = std::chrono::high_resolution_clock::now() - start_time;
	float milliSecs = (std::chrono::duration_cast<std::chrono::microseconds>(tempRenderTime).count())/1000.0f;

	//Sleep the amount of time to make the program run at the correct 
	if (milliSecs <= (1000/screen->fps)) SDL_Delay(1000/screen->fps - milliSecs);

	//Update render time
	renderTime = milliSecs;
} 

float Scene::getDistToPlayer(Object * inputObject) {
	float dist = 5;
	float x = player->x + dist * sin(player->thetaAngle) * cos(player->phiAngle);
	float y = player->y + dist * sin(player->thetaAngle) * sin(player->phiAngle);
	float z = player->z + dist * cos(player->thetaAngle - PI/4);

	return sqrt(pow(x - inputObject->centerX, 2) + pow(y - inputObject->centerY, 2) + pow(z - inputObject->centerZ, 2));
}

int Scene::objectVisible(Object * inputObject) {

	int renderMargin = 600;
	int objectRenderMargin = 0;
	int pointsInside = 0;

	//-----Calculate where center point of object is projected-----
	float halfHFov = player->hFov/2;
	float halfVFov = player->vFov/2;
	float hFovTimesScreen_WIDTH = screen->SCREEN_WIDTH/player->hFov;
	float vFovTimesScreen_HEIGHT = screen->SCREEN_HEIGHT/player->vFov;

	//3D Point coordinates with object coordinates offset

	float pointX = inputObject->centerX;
	float pointY = inputObject->centerY;
	float pointZ = inputObject->centerZ;		

	//-----Camera ROTATION--------

	//Calculate phi angle
	//Calculate diff values for xy plane
	float diffX = pointX - player->x;
	float diffY = pointY - player->y;

	//Radius
	float r = sqrt(pow(diffX, 2) + pow(diffY, 2));

	//Degrees
	float phi = atan2(diffY, diffX);

	//Add rotation
	phi -= player->phiAngle;

	//Rotate 
	pointX = player->x + r * cos(phi);
	pointY = r * sin(phi);
	
	//Calculate theta angle
	//Calculate diff values for xy plane
	diffX = pointX - player->x;
	float diffZ = pointZ - player->z;

	//New radius
	r = sqrt(pow(diffX, 2) + pow(diffZ, 2));

	//Degrees
	float theta = atan2(diffX, diffZ);

	//Add rotation
	theta += player->thetaAngle;

	//Rotate 
	pointX = r * sin(theta);
	pointZ = r * cos(theta);

	//-----END Camera ROTATION----

	float xFovPos = 0.0f;
	float yFovPos = 0.0f;
	//Project point
	if (pointX != 0) {
		xFovPos = atan2(pointY, pointX);
		yFovPos = atan2(pointZ, pointX);
	}
	int screenPosX = (xFovPos + halfHFov)*hFovTimesScreen_WIDTH;
	int screenPosY = (yFovPos + halfVFov)*vFovTimesScreen_HEIGHT;

	//Calculate maximum width of object on screen using objects maximum radius
	float distToObject = sqrt(pow(player->x - inputObject->centerX, 2) + pow(player->y - inputObject->centerY, 2) + pow(player->z - inputObject->centerZ, 2));
	float objectRadius = inputObject->maxRadius + objectRenderMargin;

	float angleWidth = atan(objectRadius/distToObject);
	float screenRadius = (angleWidth/std::max(player->hFov, player->vFov))*std::max(screen->SCREEN_WIDTH, screen->SCREEN_HEIGHT);

	//Check circle enveloping object is inside view
	for (float angle = 0.0f; angle < 2*PI; angle += PI/2) {
		int screenX = screenPosX + (screenRadius * cos(angle));
		int screenY = screenPosY + (screenRadius * sin(angle));
		//Checks if point is inside view
		if ((screenX >= (-renderMargin) && 
			screenX < (screen->SCREEN_WIDTH + renderMargin)) && 
			(screenY >= (-renderMargin) && 
			screenY < (screen->SCREEN_HEIGHT + renderMargin))) {

			return 1;
		}	
	}

	return 0;

}

//Returns a list with 2d vertex's to render
std::list<Triangle> Scene::renderQueue() {

	//Variable pre calculation
	float halfHFov = player->hFov/2;
	float halfVFov = player->vFov/2;
	float hFovTimesScreen_WIDTH = screen->SCREEN_WIDTH/player->hFov;
	float vFovTimesScreen_HEIGHT = screen->SCREEN_HEIGHT/player->vFov;
	//List to hold render vertices
	std::list<Triangle> triangles;
	//Iterate through all objects in scene
	for (auto * object : objectList) {
		//Skip object if not visible
		if (!objectVisible(object)) {
			continue;
		}
		//Set object invisible if outside drawing distance
		if (getDistToPlayer(object) > drawDistance) {
			continue;
		}
		//Skip object if not visible
		if (!object->visible) {
			continue;
		}
		//Retrieve vertexList for object
		std::list<Vertex> &vertexList = object->vertexList;
		//Iterate through each vertex in object
		for (auto &vertex : vertexList) {

			//Three points for each vertex
			std::list<Point> &pointList = vertex.pointList;
			//Used to create zBuffer			
			float distAvg = 0;
			//Variable to hold 2d triangle
			Triangle triangle;
			//Is inside view
			int pointsInside = 0;

			for (auto &point : pointList) {

				//3D Point coordinates with object coordinates offset

				float pointX = point.x + object->x;
				float pointY = point.y + object->y;
				float pointZ = point.z + object->z;		

				//-----Camera ROTATION--------

				//Calculate phi angle
				//Calculate diff values for xy plane
				float diffX = pointX - player->x;
				float diffY = pointY - player->y;

				//Radius
				float r = sqrt(pow(diffX, 2) + pow(diffY, 2));

				//Degrees
				float phi = atan2(diffY, diffX);

				//Add rotation
				phi -= player->phiAngle;

				//Rotate 
				pointY = r * sin(phi);
				
				//Calculate theta angle
				//Calculate diff values for xy plane
				diffX = r * cos(phi);
				float diffZ = pointZ - player->z;

				//New radius
				r = sqrt(pow(diffX, 2) + pow(diffZ, 2));

				//Degrees
				float theta = atan2(diffX, diffZ);

				//Add rotation
				theta += player->thetaAngle;

				//Rotate 
				pointX = r * sin(theta);
				pointZ = r * cos(theta);

				//-----END Camera ROTATION----

				distAvg += pow(pointX, 2) + pow(pointY, 2) + pow(pointZ, 2);

				float xFovPos = 0.0f;
				float yFovPos = 0.0f;
				//Project point
				if (pointX != 0) {
					xFovPos = atan2(pointY, pointX);
					yFovPos = atan2(pointZ, pointX);
				}
				int screenPosX = (xFovPos + halfHFov)*hFovTimesScreen_WIDTH;
				int screenPosY = (yFovPos + halfVFov)*vFovTimesScreen_HEIGHT;

				triangle.addPoint(screenPosX, screenPosY);				

				//Checks if point is inside view
				if (	!pointsInside &&
					screenPosX >= 0 && 
					screenPosX < screen->SCREEN_WIDTH && 
					screenPosY >= 0 && 
					screenPosY < screen->SCREEN_HEIGHT) {

					pointsInside = 1;

				}	

			}

			//If whole triangle is visible
			if (pointsInside > 0) {

				float modVal = PI/4;
				//Alternative 2 as a function of triangle angle
				float sphereDist = sqrt(pow(fmod(player->phiAngle, modVal) - fmod(vertex.phi, modVal), 2) + pow(fmod(player->thetaAngle, modVal) - fmod(vertex.theta, modVal), 2))/(2*PI);

				//Test
				if (sphereDist < 0) {
					sphereDist = 0;
				} else if (sphereDist > 1) {
					sphereDist = 1;
				} 

				float factor = 0.6 + 0.4 * sphereDist;

				//Set triangle color and alpha
				triangle.color[0] = (int) (object->colorR*factor);
				triangle.color[1] = (int) (object->colorG*factor);
				triangle.color[2] = (int) (object->colorB*factor);
				triangle.alpha = object->alpha;
				triangle.solid = object->solid;
				triangle.dist = distAvg;			

				//Iterate through triangleList to find correct render index
				std::list<Triangle>::iterator triangleIt;
				for (triangleIt = triangles.begin(); triangleIt != triangles.end(); ++triangleIt){
					if (distAvg > triangleIt->dist) break;
				}

				//Add 2d triangle to render queue
				triangles.insert(triangleIt, triangle);

			}

		}
	}

	return triangles;
}

//Add a object that consists of vertices to the scene
void Scene::addObject(Object * object) {
	objectList.push_back(object);
}

//Rotate an object
void Scene::rotateScene(float thetaAdd, float phiAdd) {
	for (auto * object : objectList) {
		object->rotateObject(player->x, player->y, player->z, thetaAdd, phiAdd);
	}
}

//Returns render time
float Scene::getRenderTime() {
	return renderTime;
}

//Returns averages render time for given number of frames
float Scene::getAverageRenderTime(int numOfFrames) {

	//Start collecting average
	if (!collectAverageRenderTime) {
		collectAverageRenderTime = 1;
		frameTimes = (float*) malloc(numOfFrames*sizeof(float));
	} else {

		//Add new render time
		*(frameTimes + frameIndex) = renderTime;

		//Frame position
		frameIndex = (frameIndex + 1) % numOfFrames;

		//Calculate and return average
		int i = 0;
		float sum = 0.0f;
		while (*(frameTimes + i)) {
			sum += *(frameTimes + i);
			i++;
		}
		
		//Return average 
		return sum/i;
	}

	return renderTime;
}







