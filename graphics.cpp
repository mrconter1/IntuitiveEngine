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

	SDL_SetRenderDrawColor(gRenderer, color[0], color[1], color[2], alpha); 
	
	int x[3];
	int y[3];

	//Outline triangle	
	SDL_Point points[4];
	int i = 0;
	for (auto &point : pointList){
		if (i == 0) {
			points[0] = {point.screenX, point.screenY};
			points[3] = {point.screenX, point.screenY};
		} else {
			points[i] = {point.screenX, point.screenY};
		}
		x[i] = point.screenX;
		y[i] = point.screenY;
		i++;
	}

	SDL_RenderDrawLines(gRenderer, points, 4);

	if (solid) {

		//Fill triangle
		int numOfPoints = 0;
		std::list<Point> linePointList;

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
		
		float radius = 0.0f;	
		while (!doneA || !doneB) {

			//Calculate coordinates if job not done
			if (!doneA) {
				xA = radius * cAngleA + x[1];
				yA = radius * sAngleA + y[1];

				//If target is not reached, calculate coordinate and add to list
				if (abs(xA  - x[0]) < 2 && abs(yA  - y[0]) < 2) {
					doneA = 1;
				} else {
					linePointList.push_back(Point(xA, yA));
					numOfPoints++;
				}
				
			}

			if (!doneB) {
				xB = radius * cAngleB + x[2];
				yB = radius * sAngleB + y[2];

				//If target is not reached, calculate coordinate and add to list
				if (abs(xB  - x[0]) < 2 && abs(yB  - y[0]) < 2) {
					doneB = 1;
				} else {
					linePointList.push_back(Point(xB, yB));
					numOfPoints++;
				}
			}

			radius += 1.0f;

		}

		SDL_Point linePoints[numOfPoints];
		int j = 0;
		for (auto &point : linePointList){
			linePoints[j] = {point.screenX, point.screenY};
			j++;
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
}

//Retrieves three 3d points in form of array lists
void Object::addVertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC) {
	vertexList.push_back(Vertex(pointA, pointB, pointC));

	int pointCount = 0;
	//Find center of object
	for (auto &vertex : vertexList) {

			//To hold phi and angle relative to origo for each point
			float phi[3], theta[3];
			int index = 0;

			//Three points for each vertex
			std::list<Point> &pointList = vertex.pointList;
			for (auto &point : pointList) {
				
				cX += point.x;
				cY += point.y;
				cZ += point.z;
				
				//Calculate angle
				float r = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
				phi[index] = atan2(point.y, point.x);
				printf("index: %d\n", index);
				if (r > 0) {
					theta[index] = acos(point.z/r);
				} else {
					theta[index] = 0.0f;
				}
			
				pointCount++;
				index++;
			}	

			float angleAB = atan2(theta[1] - theta[0], phi[1] - phi[0]); 
			float angleAC = atan2(theta[2] - theta[0], phi[2] - phi[0]); 

			float angleBA = atan2(theta[0] - theta[1], phi[0] - phi[1]); 
			float angleBC = atan2(theta[2] - theta[1], phi[2] - phi[1]);	

			float middleA = (angleAB + angleAC)/2;
			float middleB = (angleBA + angleBC)/2;
			
									
			
	}

	//Add average point
	if (pointCount > 0) {
		cX /= pointCount;
		cY /= pointCount;
		cZ /= pointCount;
	}

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











