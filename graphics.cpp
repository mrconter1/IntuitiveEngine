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











