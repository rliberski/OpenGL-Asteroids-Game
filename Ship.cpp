#include "Ship.h"

Ship::Ship()
{
	shipObject = new Obj("shipModel.obj");
	rotateShip = 0.0f;
	translateShip = 0.0f;
}

/*
void Ship::drawShip()
{
	glPushMatrix();

	glTranslatef(translateShip, -0.90f, 0.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	glRotatef(rotateShip, 0.0f, 1.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.2f, 0.02f); glVertex3f(-0.05f, 0.125f, 0.02f);
	glVertex3f(-0.05f, 0.125f, 0.02f); glVertex3f(0.0f, 0.1f, 0.02f);
	glVertex3f(0.0f, 0.1f, 0.02f); glVertex3f(0.05f, 0.125f, 0.02f);
	glVertex3f(0.0f, 0.2f, 0.02f); glVertex3f(0.05f, 0.125f, 0.02f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.2f, -0.02f); glVertex3f(-0.05f, 0.125f, -0.02f);
	glVertex3f(-0.05f, 0.125f, -0.02f); glVertex3f(0.0f, 0.1f, -0.02f);
	glVertex3f(0.0f, 0.1f, -0.02f); glVertex3f(0.05f, 0.125f, -0.02f);
	glVertex3f(0.0f, 0.2f, -0.02f); glVertex3f(0.05f, 0.125f, -0.02f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.2f, 0.02f); glVertex3f(0.0f, 0.2f, -0.02f);
	glVertex3f(-0.05f, 0.125f, 0.02f); glVertex3f(-0.05f, 0.125f, -0.02f);
	glVertex3f(0.0f, 0.1f, 0.02f); glVertex3f(0.0f, 0.1f, -0.02f); 
	glVertex3f(0.05f, 0.125f, 0.02f); glVertex3f(0.05f, 0.125f, -0.02f);
	glEnd();
	
	glPopMatrix();
}
*/

void Ship::drawShip()
{
	glEnable(GL_CULL_FACE);

	glPushMatrix();

	glTranslatef(translateShip, -0.90f, 0.0f);
	glRotatef(rotateShip, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &(shipObject->vertices[0]));
	glNormalPointer(GL_FLOAT, 0, &(shipObject->normals[0]));

	glDrawElements(GL_TRIANGLES, shipObject->faces.size(), GL_UNSIGNED_SHORT, &(shipObject->faces[0]));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}

void Ship::rotateBack(float time)
{
	if (rotateShip < -5)
		rotateShip += 100.0f * time;
	else if (rotateShip > 5)
		rotateShip -= 100.0f * time;
	else
		rotateShip = 0.0;
}

void Ship::turnLeft(float time)
{
	if (rotateShip > -55)
		rotateShip -= 100.0f * time;
	if (translateShip > -0.95)
		translateShip -= 1.0f * time;

}

void Ship::turnRight(float time)
{
	if (rotateShip < 55)
		rotateShip += 100.0f * time;
	if(translateShip < 0.95)
		translateShip +=1.0f * time;
}

GLfloat Ship::getTranslate()
{
	return translateShip;
}

void Ship::reset()
{
	translateShip = 0.0f;
}