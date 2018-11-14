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

	//Retrieves three 3d points in form of array lists
	void addVertex(std::initializer_list<float> pointA, std::initializer_list<float> pointB, std::initializer_list<float> pointC);
	//Rotate an object
	void rotateObject(float x, float y, float z, float thetaAdd, float phiAdd);
};

class Scene {
	public:
		Player * player;
		Screen * screen;
		std::list<Object*> objectList;
		float renderTime;

	//Constructor that takes a pointer to the player instance as paramter
	Scene(Player * inputPlayer, Screen * inputScreen);
	void renderScene();
	//Returns a list with 2d vertex's to render
	std::list<Triangle> renderQueue();
	//Add a object that consists of vertices to the scene
	void addObject(Object * object);
	//Rotate an object
	void rotateScene(float thetaAdd, float phiAdd);
	//Returns render time
	float getRenderTime();
};




