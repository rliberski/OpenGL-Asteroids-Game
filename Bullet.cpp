#include "Bullet.h"

void Bullet::moveBullet(float time)
{ 
	posy+=2.0f * time;
	if(posy>0.99) 
		active = false;
}

void Bullet::drawBullet()
{
	glColor3f(1.0, 1.0, 0.0);	//yellow
	glPushMatrix();
	glTranslatef(posx, posy, 0.0);
	glutSolidCube(0.01);
	glPopMatrix();
}

bool Bullet::isActive()
{
	return active;
}

float Bullet::getX()
{
	return posx;
}

float Bullet::getY()
{
	return posy;
}

void Bullet::setActive(bool act)
{
	active = act;
}
