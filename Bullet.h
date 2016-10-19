#pragma once

#ifndef _BULLET_H
#define _BULLET_H

#include <freeglut.h>

class Bullet
{
public:
	Bullet(float pos) : posx(pos), posy(-0.7f), active(true) {}
	void moveBullet(float time);
	void drawBullet();
	bool isActive();
	void setActive(bool act);
	float getX();
	float getY();

private:
	float posx;
	float posy;
	bool active;
};

#endif