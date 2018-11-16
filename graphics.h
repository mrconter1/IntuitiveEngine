#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <list>

class Point {
	public:
		float x, y, z;
		int screenX, screenY;
	
	//Initilize 3d point
	Point(float inputX, float inputY, float inputZ);
	//Initilize 2d point
	Point(int inputX, int inputY);
}; 

class Triangle {
	public:
		std::list<Point> pointList;
		float dist;
		int color[3];
		int alpha;
		int solid;
	
	void addPoint(int x, int y);
	//Draw line to surface
	void drawLineTriangle(SDL_Renderer* gRenderer);
};

class Vertex {
	public:
		std::list<Point> pointList;

	//Initilize a vertex with three points
	Vertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC);
};


class Object {
	public:
		std::list<Vertex> vertexList;
		int isVisible;
		float x, y, z;
		float phiAngle, thetaAngle;
		int colorR, colorG, colorB, alpha;
		int solid;
		int visible;
		int cX, cY, cZ;

	Object();

	//Retrieves three 3d points in form of array lists
	void addVertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC);
	//Rotate an object
	void rotateObject(float x, float y, float z, float thetaAdd, float phiAdd);
	//Set object color
	void setColor(int inputR, int inputG, int inputB);
	//Set object alpha
	void setAlpha(int inputA);
	//Set object alpha
	void setSolid(int inputVal);
	//Set visibility
	void setVisible(int inputVal);
	
};

#endif




