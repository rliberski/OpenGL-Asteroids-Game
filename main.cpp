#include "Ship.h"
#include "Bullet.h" 
#include "Asteroid.h"

#include <glut.h>
#include <ctime>
#include <list>
#include <string>
#include <cmath>


std::list<Bullet*> bullets;		//active bullets
std::list<Asteroid*> asteroids;	//active asteroids
Obj *asteroidObject = new Obj("asteroidModel.obj");	//one model for all asteroids, so it is loaded here
//Obj *bulletObject = new Obj("bulletModel.obj");		//one model for all bullets, so it is loaded here
Ship* m_ship = new Ship();		//main ship

//steering keys
bool leftPressed = false;
bool rightPressed = false;

//score, difficulty
int score = 0;
float asteroidSpeed = 1.0;
float spawnTime = 1.0;

//time
float toSpawn = 0.0;		//time to spawn asteroid
float startFrame = 0.0;		//start frame time
float endFrame = 0.0;		//end frame time
int framesPerSec = 0;		//frames per sec
int frames = 0;				//actual frames
float oneSec;				//one second counter

//drawing functions (used in display function)
void drawScore()		//drawing score and update window title (FPS)
{
	glColor3f(1.0f,1.0f,1.0f);			//white
    glRasterPos3f(-0.9f,-0.9f,0.0f);	//position

	//draw score
	std::string scoreText = "Score: ";
	std::string scoreStr;
	scoreStr=std::to_string(score);
	for (unsigned int i=0; i<scoreText.length(); i++) 
	{
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreText[i]);
    }
	for (unsigned int i=0; i<scoreStr.length(); i++) 
	{
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreStr[i]);
    }

	//update FPS
	std::string frameStr = "Asteroids - FPS: " + std::to_string(framesPerSec);
	glutSetWindowTitle(frameStr.c_str());
}
void updateObjects()
{
	//update and draw bullets
	for(Bullet* bullet : bullets)
	{
		bullet->moveBullet(endFrame / CLOCKS_PER_SEC);
		bullet->drawBullet();
	}

	//update and draw asteroids
	for(Asteroid* asteroid : asteroids)
	{
		asteroid->moveAsteroid(asteroidSpeed, endFrame / CLOCKS_PER_SEC);
		asteroid->drawAsteroid(asteroidObject);
	}

	//draw ship
	m_ship->drawShip();

	//draw score
	drawScore();
}
//end of drawing functions

//opengl functions
void Display()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	//background (almost black)

	//prepare
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();					

	//drawing
	updateObjects();
	drawScore();

	//finish
	glFlush();
	glutSwapBuffers();
}
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);	
	Display();
}
void Keyboard(unsigned char key, int x, int y)
{
	//shooting
	if (key == ' ')
		bullets.push_front(new Bullet(m_ship->getTranslate()));
}
void SpecialKeys(int key, int x, int y)
{
	//steering (keys down)
	if (key == GLUT_KEY_LEFT)
		leftPressed = true;
	if (key == GLUT_KEY_RIGHT)
		rightPressed = true;
}
void SpecialKeysUp(int key, int x, int y)
{
	//steering (keys up)
	if (key == GLUT_KEY_LEFT)
		leftPressed = false;
	if (key == GLUT_KEY_RIGHT)
		rightPressed = false;
}
//end of opengl functions

//collision functions
void reset()	//reset game if ship collide with asteroid
{
	//delete bullets and asteroids
	std::list<Bullet*>::iterator b = bullets.begin();
	while(b != bullets.end())
	{
		Bullet* deleteThis = *b;
		bullets.erase(b++);
		delete deleteThis;
	}
	std::list<Asteroid*>::iterator a = asteroids.begin();
	while(a != asteroids.end())
	{
		Asteroid* deleteThis = *a;
		asteroids.erase(a++);
		delete deleteThis;
	}
	asteroids.clear();
	bullets.clear();

	//delete ship and create new
	m_ship->reset();

	//reset score and difficulty values
	score = 0;
	asteroidSpeed = 1.0;
	spawnTime = 1.0;
}
bool bulletCollision()	//bullets with asteroids collision
{
	std::list<Asteroid*>::iterator a = asteroids.begin();
	while(a!=asteroids.end())
	{
		float AsteroidSphere = (*a)->getRadius();
		std::list<Bullet*>::iterator b = bullets.begin();
		while(b!=bullets.end())
		{	
			//it's just euclidean distance (2D, 'Z' Axis is 0)
			float distance = sqrt((((*a)->getX()-(*b)->getX())*((*a)->getX() -(*b)->getX()))+(((*a)->getY() -(*b)->getY())*((*a)->getY() -(*b)->getY())));
			//if collide set as unactive (it will be deleted in "deleteOutOfBorders" function)
			if(distance < AsteroidSphere)
			{
				(*a)->setActive(false);
				(*b)->setActive(false);
				return true;
			}
			b++;
		}
		a++;
	}
	return false;
}
bool shipCollision() //ship with asteroids collision
{
	for(Asteroid* asteroid : asteroids)
	{
		float sphere = asteroid->getRadius();
		if(asteroid->getY() - sphere < -0.80 && asteroid->getY() + sphere > -0.85 )
		{
			if(asteroid->getX() < m_ship->getTranslate() + 0.07 && asteroid->getX() > m_ship->getTranslate()  -0.07)
			{
				score = -1;
				Sleep(1000);
				return true;
			}
		}
	}
	return false;
}
//end of collision functions

//Loop functions
void updateShip()	//update ship position and rotation
{
	if(leftPressed)
		m_ship->turnLeft(endFrame / CLOCKS_PER_SEC);
	if(rightPressed)
		m_ship->turnRight(endFrame / CLOCKS_PER_SEC);
	if (!leftPressed && !rightPressed)
		m_ship->rotateBack(endFrame / CLOCKS_PER_SEC);
}
void deleteOutOfBorders()//delete unactive bullets and asteroids
{
	std::list<Bullet*>::iterator it = bullets.begin();
	while(it != bullets.end())
	{
		if((*it)->isActive())
		{
			++it;
		}
		else
		{
			Bullet* deleteThis = *it;
			bullets.erase(it++);
			delete deleteThis;
		}
	}
	std::list<Asteroid*>::iterator itt = asteroids.begin();
	while(itt != asteroids.end())
	{
		if((*itt)->isActive())
		{
			++itt;
		}
		else
		{
			Asteroid* deleteThis = *itt;
			asteroids.erase(itt++);
			delete deleteThis;
		}
	}
}
void spawnAsteroids()	//generate asteroids
{
	toSpawn += endFrame / CLOCKS_PER_SEC;
	if(toSpawn > spawnTime) 
	{
		toSpawn = 0.0f;
		asteroids.push_front(new Asteroid()); //create asteroid
	}
}
void checkCollision() //update game state
{
	if(bulletCollision()) 
	{
		score++;
		asteroidSpeed = 1.0f + (score/200.0f);
		if(spawnTime > 0.01)
			spawnTime = 1.0f /score*3.0f;///100.0f); 
	}

	//if(shipCollision()) reset();
}
//end of loop functions

//main loop: count framerate and update game
void Loop()
{
	startFrame = (float)clock();	//start frame time

	updateShip();   //steering
	deleteOutOfBorders();  //delete bullets and asteroids out of screen
	spawnAsteroids();  //generate asteroids
	checkCollision();  //check collisions and update game state
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); //refresh

	endFrame = clock() - startFrame;	//one frame time

	//count fps
	oneSec += endFrame;
	frames++;
	if (oneSec >= CLOCKS_PER_SEC)
	{
		framesPerSec = frames;
		frames = 0;
		oneSec = 0.0;
	}
}

//initialize
int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Asteroids");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys);
	glutSpecialUpFunc(SpecialKeysUp);

	glutIdleFunc(Loop);
	glutMainLoop();

	return 0;
}