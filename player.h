#include <cmath>
#include <string>
#include "screen.h"

class Player {
 
	public:
		
		//Variables 
		float x, y, z;
		float vFov, hFov;

		float deAccFactor;
		float xSpeed, ySpeed, zSpeed;

		float maxVel, speed;
		Screen * screen;

		float rotX, rotY;
		float thetaAngle, phiAngle;

		//Functions
		Player(Screen * screen);

		void addForwardSpeed(float speed);
		void addBackwardSpeed(float speed);
		void addLeftSpeed(float speed);
		void addRightSpeed(float speed);
		void addUpSpeed(float speed);
		void addDownSpeed(float speed);

		void addRotationLeft(float amount);
		void addRotationRight(float amount);

		float getVelocity();

		void handleInput(const Uint8* keystates);

		void update();

	private:

		void handleMouse();
}; 
