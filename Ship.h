#pragma once

#ifndef _SHIP_H
#define _SHIP_H

#include "Obj.h"
#include <freeglut.h>

class Ship 
{
public:
	Ship();
	void drawShip();
	void turnLeft(float time);
	void turnRight(float time);
	void rotateBack(float time);
	GLfloat getTranslate();
	void reset();

private:
	Obj *shipObject;
	GLfloat rotateShip;
	GLfloat translateShip;
};

#endif