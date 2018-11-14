#include <cmath>
#include <SDL.h>
#include "player.h"

#define PI 3.14159265358	

Player::Player(Screen * inputScreen) {
	//Pointer to screen
	screen = inputScreen;

	//Player Values
	x = y = z = xSpeed = ySpeed = zSpeed = 0;
	hFov = PI/4;
	vFov = hFov*(((float) screen->SCREEN_HEIGHT)/((float) screen->SCREEN_WIDTH));
	deAccFactor = 0.925;
	speed = 0.01f;
	maxVel = 1;

	thetaAngle = 0;
	phiAngle = 0;

	//Init mouse tracking
	rotX = 0;
	rotY = 0;
}

void Player::addForwardSpeed(float speed) {
	if (getVelocity() < maxVel) {
		xSpeed += speed*cos(phiAngle);
		ySpeed += speed*sin(phiAngle);
	}
}

void Player::addBackwardSpeed(float speed) {
	if (getVelocity() < maxVel) {
		xSpeed -= speed*cos(phiAngle);
		ySpeed -= speed*sin(phiAngle);
	}
}

void Player::addLeftSpeed(float speed) {
	if (getVelocity() < maxVel) {
		xSpeed -= speed*cos(phiAngle + PI/2);
		ySpeed -= speed*sin(phiAngle + PI/2);
	}
}

void Player::Player::addRightSpeed(float speed) {
	if (getVelocity() < maxVel) {
		xSpeed += speed*cos(phiAngle + PI/2);
		ySpeed += speed*sin(phiAngle + PI/2);
	}
}

void Player::addUpSpeed(float speed) {
	if (getVelocity() < maxVel) {
		zSpeed -= speed;
	}
}

void Player::addDownSpeed(float speed) {
	if (getVelocity() < maxVel) {
		zSpeed += speed;
	}
}

void Player::addRotationLeft(float amount) {
	phiAngle += amount;
}
	
void Player::addRotationRight(float amount) {
	phiAngle -= amount;
}

float Player::getVelocity() {
	return sqrt(pow(xSpeed, 2) + pow(ySpeed, 2) + pow(zSpeed, 2));
}

void Player::handleInput(const Uint8* keystates) {

	if(keystates[SDL_SCANCODE_W]) {
		addForwardSpeed(speed);
	} 

	if(keystates[SDL_SCANCODE_LSHIFT]) {
		addForwardSpeed(speed);
	}

	if(keystates[SDL_SCANCODE_S]) {
		addBackwardSpeed(speed);
	}

	if(keystates[SDL_SCANCODE_A]) {
		addLeftSpeed(speed);
	}

	if(keystates[SDL_SCANCODE_D]) {
		addRightSpeed(speed);
	}

	if(keystates[SDL_SCANCODE_SPACE]) {
		addUpSpeed(speed);
	}

	if(keystates[SDL_SCANCODE_LCTRL]) {
		addDownSpeed(speed);
	}

	handleMouse();

}

void Player::handleMouse() {

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	rotX = (mouseX - screen->SCREEN_WIDTH/2)/1000.0f;
	rotY = (mouseY - screen->SCREEN_HEIGHT/2)/1000.0f;

	phiAngle += rotX;
	thetaAngle += rotY;
	
	//Restrict mouse angle
	if (thetaAngle > PI/2) {
		thetaAngle = PI/2;
	} else if (thetaAngle < -PI/2) {
		thetaAngle = -PI/2;
	}

	//Reset mouse position
	SDL_WarpMouseInWindow(screen->gWindow, screen->SCREEN_WIDTH/2, screen->SCREEN_HEIGHT/2);

}

void Player::update() {

	handleInput(SDL_GetKeyboardState(NULL));

	x += xSpeed;
	y += ySpeed;
	z += zSpeed;

	xSpeed *= deAccFactor;
	ySpeed *= deAccFactor;
	zSpeed *= deAccFactor;

}

