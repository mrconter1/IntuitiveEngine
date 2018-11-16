#include <SDL.h>
#include <list>
#include <cmath>
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

	//Outline triangle	
	SDL_Point points[4];
	int i = 0;
	for (std::list<Point>::iterator pointIt = pointList.begin(); pointIt != pointList.end(); ++pointIt){
		if (i == 0) {
			points[0] = {pointIt->screenX, pointIt->screenY};
			points[3] = {pointIt->screenX, pointIt->screenY};
		} else {
			points[i] = {pointIt->screenX, pointIt->screenY};
		}
		x[i] = pointIt->screenX;
		y[i] = pointIt->screenY;
		i++;
	}

	SDL_RenderDrawLines(gRenderer, points, 4);

	//Fill triangle
	int numOfPoints = 0;
	std::list<Point> linePointList;

	float angleA = atan2(y[0] - y[1], x[0] - x[1]);
	float angleB = atan2(y[0] - y[2], x[0] - x[2]);

	float cAngleA = cos(angleA);
	float sAngleA = sin(angleA);

	float cAngleB = cos(angleB);
	float sAngleB = sin(angleB);

	int doneA = 0;
	int doneB = 0;

	float xA, yA, xB, yB;
	
	float radius = 0.0f;	
	while (!doneA || !doneB) {

		if (!doneA) {
			xA = radius * cAngleA + x[1];
			yA = radius * sAngleA + y[1];
		}

		if (!doneB) {
			xB = radius * cAngleB + x[2];
			yB = radius * sAngleB + y[2];
		}

		if (doneA == 0 && sqrt(pow(xA  - x[0], 2)) < 2.0f && sqrt(pow(yA  - y[0], 2)) < 2.0f) {
			doneA = 1;
		} else {
			if (!doneA) {
				linePointList.push_back(Point(xA, yA));
				numOfPoints++;
			}
		}

		if (doneB == 0 && sqrt(pow(xB  - x[0], 2)) < 2.0f && sqrt(pow(yB  - y[0], 2)) < 2.0f) {
			doneB = 1;
		} else {
			if (!doneB) {
				linePointList.push_back(Point(xB, yB));
				numOfPoints++;
			}
		}

		radius += 0.5f;

	}

	SDL_Point linePoints[numOfPoints];
	int j = 0;
	for (std::list<Point>::iterator pointIt = linePointList.begin(); pointIt != linePointList.end(); ++pointIt){
		linePoints[j] = {pointIt->screenX, pointIt->screenY};
		j++;
	}

	SDL_RenderDrawLines(gRenderer, linePoints, numOfPoints);
	
}

//-----Vertex Class-----
Vertex::Vertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC) {
	pointList.push_back(Point(pointA.begin()[0], pointA.begin()[1], pointA.begin()[2]));
	pointList.push_back(Point(pointB.begin()[0], pointB.begin()[1], pointB.begin()[2]));
	pointList.push_back(Point(pointC.begin()[0], pointC.begin()[1], pointC.begin()[2]));
}

//-----Object Class-----
Object::Object() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

	phiAngle = 0.0f;
	thetaAngle = 0.0f;
}

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











