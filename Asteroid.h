#pragma once

#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <freeglut.h>
#include <random>
#include "Obj.h"

class Asteroid
{
public:
	Asteroid();
	void moveAsteroid(float speed,float time);
	void drawAsteroid(Obj *object);
	float getRadius();
	bool isActive();
	void setActive(bool act);
	float getX();
	float getY();

private:
	GLfloat startXPoint;
	GLfloat posx;
	GLfloat posy;
	bool active;
	GLfloat scale;
	GLfloat translate;
	float rotateAngle;
	GLfloat rotateX;
	GLfloat rotateY;
	GLfloat rotateZ;
};

#endif