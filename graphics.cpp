#include <SDL.h>
#include <list>
#include <cmath>
#include <chrono>
#include "player.h"
#include "graphics.h"

//-----Point Class-----
Point::Point(float inputX, float inputY, float inputZ) {
	x = inputX;
	y = inputY;
	z = inputZ;
}

Point::Point(int inputX, int inputY) {
	screenX = inputX;
	screenY = inputY;
}

//-----Triangle Class-----
void Triangle::addPoint(int x, int y) {
	pointList.push_back(Point(x, y));
}

void Triangle::drawLineTriangle(SDL_Renderer* gRenderer) {
	int x[3];
	int y[3];
	int i = 0;
	for (std::list<Point>::iterator pointIt = pointList.begin(); pointIt != pointList.end(); ++pointIt){
		x[i] = pointIt->screenX;
		y[i] = pointIt->screenY;
		i++;
	}
		
	SDL_RenderDrawLine(gRenderer, x[0], y[0], x[1], y[1]);
	SDL_RenderDrawLine(gRenderer, x[0], y[0], x[2], y[2]);
	SDL_RenderDrawLine(gRenderer, x[1], y[1], x[2], y[2]);

}

//-----Vertex Class-----
Vertex::Vertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC) {
	pointList.push_back(Point(pointA.begin()[0], pointA.begin()[1], pointA.begin()[2]));
	pointList.push_back(Point(pointB.begin()[0], pointB.begin()[1], pointB.begin()[2]));
	pointList.push_back(Point(pointC.begin()[0], pointC.begin()[1], pointC.begin()[2]));
}

//-----Object Class-----
//Retrieves three 3d points in form of array lists
void Object::addVertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC) {
	vertexList.push_back(Vertex(pointA, pointB, pointC));
}

//Rotate an object
void Object::rotateObject(float x, float y, float z, float thetaAdd, float phiAdd) {
		//Iterate through each vertex in object
		for (auto &vertex : vertexList) {
			//Three points for each vertex
			std::list<Point> &pointList = vertex.pointList;
			for (auto &point : pointList) {
				
				//Calculate phi angle
				//Calculate diff values for xy plane
				float diffX = point.x - x;
				float diffY = point.y - y;

				//Radius
				float r = sqrt(pow(diffX, 2) + pow(diffY, 2));

				//Degrees
				float phi = atan2(diffY, diffX);

				//Add rotation
				phi += phiAdd;

				//Rotate 
				point.x = x + r * cos(phi);
				point.y = y + r * sin(phi);

				//Calculate theta angle
				//Calculate diff values for xy plane
				diffX = point.x - x;
				float diffZ = point.z - z;

				//New radius
				r = sqrt(pow(diffX, 2) + pow(diffZ, 2));

				//Degrees
				float theta = atan2(diffX, diffZ);

				//Add rotation
				theta += thetaAdd;

				//Rotate 
				point.x = x + r * sin(theta);
				point.z = z + r * cos(theta);

		}				
	}
}

//-----Scene Class-----
//Constructor that takes a pointer to the player instance as paramter
Scene::Scene(Player * inputPlayer, Screen * inputScreen) {
	player = inputPlayer;
	screen = inputScreen;
	renderTime = 0.0f;
}

//Renders scene
void Scene::renderScene() {

	//Measure time before
	auto start_time = std::chrono::high_resolution_clock::now();

	//Clear screen
	screen->clearScreen();

	SDL_SetRenderDrawColor(screen->gRenderer, 255, 0, 255, 255); 

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

//Returns a list with 2d vertex's to render
std::list<Triangle> Scene::renderQueue() {
	float halfHFov = player->hFov/2;
	float halfVFov = player->vFov/2;
	float hFovTimesScreen_WIDTH = screen->SCREEN_WIDTH/player->hFov;
	float vFovTimesScreen_HEIGHT = screen->SCREEN_HEIGHT/player->vFov;
	//List to hold render vertices
	std::list<Triangle> triangles;
	//Iterate through all objects in scene
	for (Object * object : objectList) {
		//Retrieve vertexList for object
		std::list<Vertex> &vertexList = object->vertexList;
		//Iterate through each vertex in object
		for (Vertex &vertex : vertexList) {
			//Three points for each vertex
			std::list<Point> &pointList = vertex.pointList;
			//Variable to hold 2d triangle
			Triangle triangle;
			for (Point &point : pointList) {

				//3D Point coordinates
				float pointX = point.x;
				float pointY = point.y;
				float pointZ = point.z;	

				//-----ROTATION--------

	
				//Calculate phi angle
				//Calculate diff values for xy plane
				float diffX = point.x - player->x;
				float diffY = point.y - player->y;

				//Radius
				float r = sqrt(pow(diffX, 2) + pow(diffY, 2));

				//Degrees
				float phi = atan2(diffY, diffX);

				//Add rotation
				phi -= player->phiAngle;

				//Rotate 
				pointX = player->x + r * cos(phi);
				pointY = player->y + r * sin(phi);
				
				//Calculate theta angle
				//Calculate diff values for xy plane
				//diffX = point.x - x;
				diffX = pointX - player->x;
				float diffZ = point.z - player->z;

				//New radius
				r = sqrt(pow(diffX, 2) + pow(diffZ, 2));

				//Degrees
				float theta = atan2(diffX, diffZ);

				//Add rotation
				theta += player->thetaAngle;

				//Rotate 
				pointX = player->x + r * sin(theta);
				pointZ = player->z + r * cos(theta);


				//-----END ROTATION----
				
				//Calculate distances
				float xDist = pointX - player->x;
				float yDist = pointY - player->y;
				float zDist = pointZ - player->z;

				float xFovPos = 0.0f;
				float yFovPos = 0.0f;
				//Project point
				if (xDist != 0) {
					xFovPos = atan2(yDist, xDist);
					yFovPos = atan2(zDist, xDist);
				}
				int screenPosX = (xFovPos + halfHFov)*hFovTimesScreen_WIDTH;
				int screenPosY = (yFovPos + halfVFov)*vFovTimesScreen_HEIGHT;
				triangle.addPoint(screenPosX, screenPosY);					

			}
			triangles.push_back(triangle);
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











