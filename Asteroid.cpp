#include "Asteroid.h"

Asteroid::Asteroid()
{
	//losowoœæ
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<float> dist(-0.9f, 0.9f); //pozycja startowa asteroidu i si³a obrotu
	std::uniform_real_distribution<float> dist2(0.5f, 1.5f); //skala asteroidu
	std::uniform_real_distribution<float> dist3(-0.003f,0.003f); //przesuniêcie podczas spadania
	//startuj z:
	startXPoint = dist(eng);
	//si³a obrotu:
	rotateX = dist(eng);
	rotateY = dist(eng);
	rotateZ = dist(eng);
	//skalowanie
	scale = dist2(eng);
	//przesuniecie
	translate = dist3(eng);
	//ustal pozycjê startow¹
	posx = startXPoint;
	posy = 1.3f;
	rotateAngle=0;
	active = true;
}

void Asteroid::moveAsteroid(float speed, float time)
{
	posy-=speed * time;
	posx+= translate * 100 * time;
	rotateAngle+= 50.0f * time;
	if(posy<-1.3) active = false;
}

void Asteroid::drawAsteroid(Obj *object)
{
	glEnable(GL_CULL_FACE);

	glPushMatrix();

	glColor3f(0.3f, 0.2f, 0.2f);
	glTranslatef(posx, posy, 0.0f);
	glScalef(scale, scale, scale);
	glRotatef(rotateAngle, rotateX, rotateY, rotateZ);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &(object->vertices[0]));
	glNormalPointer(GL_FLOAT, 0, &(object->normals[0]));

	glDrawElements(GL_TRIANGLES, object->faces.size(), GL_UNSIGNED_SHORT, &(object->faces[0]));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}

float Asteroid::getRadius()
{
	return scale * 0.125f * sqrt(3.0f) / 2.0f;
}

bool Asteroid::isActive()
{
	return active;
}

void Asteroid::setActive(bool act)
{
	active = act;
}

float Asteroid::getX()
{
	return posx;
}

float Asteroid::getY()
{
	return posy;
}