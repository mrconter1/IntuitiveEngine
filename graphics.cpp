#include <SDL.h>
#include <list>
#include <cmath>
#include "player.h"
#include "graphics.h"

#define PI 3.14159265358	

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

	SDL_SetRenderDrawColor(gRenderer, color[0], color[1], color[2], alpha); 
	
	int x[3];
	int y[3];

	//Outline triangle	
	int i = 0;
	for (auto &point : pointList){
		x[i] = point.screenX;
		y[i] = point.screenY;
		i++;
	}

	if (!solid) {

		SDL_Point points[4];
		int i = 0;
		for (auto &point : pointList){
			if (i == 0) {
				points[0] = {point.screenX, point.screenY};
				points[3] = {point.screenX, point.screenY};
			} else {
				points[i] = {point.screenX, point.screenY};
			}
			i++;
		}

		SDL_RenderDrawLines(gRenderer, points, 4);

	} else {

		//Fill triangle
		int numOfPoints = 0;

		//Calculate angle between point A and B and between A and C
		float angleA = atan2(y[0] - y[1], x[0] - x[1]);
		float angleB = atan2(y[0] - y[2], x[0] - x[2]);

		//Calculate angles
		float cAngleA = cos(angleA);
		float sAngleA = sin(angleA);

		float cAngleB = cos(angleB);
		float sAngleB = sin(angleB);

		//Flags to toggle when reaching target coordinates
		int doneA = 0;
		int doneB = 0;

		float xA, yA, xB, yB;

		int buffSize = 5000;
		SDL_Point linePoints[buffSize];
		
		float radius = 0.0f;	
		while (!doneA || !doneB) {

			//Calculate coordinates if job not done
			if (!doneA) {
				xA = radius * cAngleA + x[1];
				yA = radius * sAngleA + y[1];

				//If target is not reached, calculate coordinate and add to list
				if (abs(xA  - x[0]) < 1 && abs(yA  - y[0]) < 1) {
					doneA = 1;
				} else {
					linePoints[numOfPoints] = {(int) xA, (int) yA};
					if (numOfPoints < buffSize) {
						numOfPoints++;
					}
				}
				
			}

			if (!doneB) {
				xB = radius * cAngleB + x[2];
				yB = radius * sAngleB + y[2];

				//If target is not reached, calculate coordinate and add to list
				if (abs(xB  - x[0]) < 1 && abs(yB  - y[0]) < 1) {
					doneB = 1;
				} else {
					linePoints[numOfPoints] = {(int) xB, (int) yB};
					if (numOfPoints < buffSize) {
						numOfPoints++;
					}
				}
			}

			radius += 0.5f;

		}

		SDL_RenderDrawLines(gRenderer, linePoints, numOfPoints);

	}
	
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

	colorR = 255;
	colorG = 255;
	colorB = 255;
	alpha = 255;

	solid = 1;
	visible = 1;

	objectPointCount = 0;
	cX = 0;
	cY = 0;
	cZ = 0;
}

//Retrieves three 3d points in form of array lists
void Object::addVertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC) {
	vertexList.push_back(Vertex(pointA, pointB, pointC));

	int pointCount = 0;
	//Find center of object
	for (auto &vertex : vertexList) {

			//To hold phi and angle relative to origo for each point
			float x[3], y[3], z[3];
			int index = 0;

			//Three points for each vertex
			std::list<Point> &pointList = vertex.pointList;
			for (auto &point : pointList) {
				
				x[index] = point.x;
				y[index] = point.y;
				z[index] = point.z;

				cX += point.x;
				cY += point.y;
				cZ += point.z;
				objectPointCount++;
			
				pointCount++;
				index++;
			}	

			//Calculate phi and theta between A and B and between A and C relative to A
			float phiAB = atan2(y[1] - y[0], x[1] - x[0]);
			float thetaAB = acos((z[1] - z[0])/sqrt(pow(x[1] - x[0], 2) + pow(y[1] - y[0], 2) + pow(z[1] - z[0], 2)));
			float phiAC = atan2(y[2] - y[0], x[2] - x[0]);
			float thetaAC = acos((z[2] - z[0])/sqrt(pow(x[2] - x[0], 2) + pow(y[2] - y[0], 2) + pow(z[2] - z[0], 2)));

			//Treat these as 2D coordinates on the spherical plane. Determine angle and rotate 90 degrees to find parallel angle
			float angle = atan2(thetaAB - thetaAC, phiAB - phiAC);
			float radius = sqrt(pow(thetaAB - thetaAC, 2) + pow(phiAB - phiAC, 2));

			//Rotate angle 90 degrees
			angle += PI/4;

			//Calculate parallel phi and theta
			float phiParallel = phiAB + radius * cos(angle);
			float thetaParallel = thetaAB + radius * sin(angle);

			//Assign values to vertex
			vertex.phi = phiParallel;
			vertex.theta = thetaParallel;

	}

	centerX =  cX / objectPointCount;
	centerY =  cY / objectPointCount;
	centerZ =  cZ / objectPointCount;
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

//Update object color
void Object::setColor(int inputR, int inputG, int inputB) {
	colorR = inputR;
	colorG = inputG;
	colorB = inputB;
}

//Update object color
void Object::setAlpha(int inputA) {
	alpha = inputA;
}


//Update object color
void Object::setSolid(int inputVal) {
	solid = inputVal;
}

//Update object color
void Object::setVisible(int inputVal) {
	visible = inputVal;
}











