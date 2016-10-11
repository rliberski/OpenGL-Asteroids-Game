#include <glut.h>
#include <ctime>
#include <list>
#include <string>
#include <random>
#include <cmath>

// klawisze
GLboolean leftPressed = false;
GLboolean rightPressed = false;
// przesuniêcie i obrót statku
GLfloat rotateShip = 0.0;
GLfloat translateShip = 0.0;

//punkty
int score = 0;
float asteroidSpeed = 0.01;

//czas
float spawnTime = 1.0;
float frameTime = 0.0;
float startFrame = 0.0;
float endFrame = 0.0;

//losowoœæ
 std::random_device rd;
 std::mt19937 eng(rd());
 std::uniform_real_distribution<> dist(-0.9, 0.9); //pozycja startowa asteroidu i si³a obrotu
 std::uniform_real_distribution<> dist2(0.5, 1.5); //skala asteroidu
 std::uniform_real_distribution<> dist3(-0.003,0.003); //przesuniêcie podczas spadania

//pociski
class Bullet
{
public:
	Bullet(GLfloat pos) : posx(pos), posy(-0.7), active(true) {}
	void moveBullet() 
	{ 
		posy+=0.05;
		if(posy>0.99) active = false;
	}

	GLfloat posx;
	GLfloat posy;
	bool active;
};
std::list<Bullet*> bullets;
//asteroidy
class Asteroid
{
public:
	Asteroid()
	{
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
		posy = 1.3;
		rotateAngle=0;
		active = true;
	}
	void moveAsteroid()
	{
		posy-=asteroidSpeed;
		posx+=translate;
		rotateAngle+=5;
		if(posy<-1.3) active = false;
	}
	GLfloat startXPoint;
	GLfloat posx;
	GLfloat posy;
	bool active;
	GLfloat scale;
	GLfloat translate;
	GLfloat rotateAngle;
	GLfloat rotateX;
	GLfloat rotateY;
	GLfloat rotateZ;
};
std::list<Asteroid*> asteroids;

void drawShip()
{
	glPushMatrix();

	glTranslatef(translateShip, -0.90, 0.0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(rotateShip, 0, 1.0, 0);

	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINES);
	glVertex3f(0.0, 0.2, 0.02); glVertex3f(-0.05, 0.125, 0.02);
	glVertex3f(-0.05, 0.125, 0.02); glVertex3f(0.0, 0.1, 0.02);
	glVertex3f(0.0, 0.1, 0.02); glVertex3f(0.05, 0.125, 0.02);
	glVertex3f(0.0, 0.2, 0.02); glVertex3f(0.05, 0.125, 0.02);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.2, -0.02); glVertex3f(-0.05, 0.125, -0.02);
	glVertex3f(-0.05, 0.125, -0.02); glVertex3f(0.0, 0.1, -0.02);
	glVertex3f(0.0, 0.1, -0.02); glVertex3f(0.05, 0.125, -0.02);
	glVertex3f(0.0, 0.2, -0.02); glVertex3f(0.05, 0.125, -0.02);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.2, 0.02); glVertex3f(0.0, 0.2, 0.02);
	glVertex3f(-0.05, 0.125, 0.02); glVertex3f(-0.05, 0.125, 0.02);
	glVertex3f(0.0, 0.1, 0.02); glVertex3f(0.0, 0.1, -0.02); 
	glVertex3f(0.05, 0.125, -0.02); glVertex3f(0.05, 0.125, -0.02);
	glEnd();
	
	glPopMatrix();
}
void drawBullets()
{
	// zolty
	glColor3f(1.0, 1.0, 0.0);

	for(Bullet* bullet : bullets)
	{
		glPushMatrix();
		glTranslatef(bullet->posx, bullet->posy, 0.0);
		glutSolidCube(0.01);
		glPopMatrix();
	}
}
void drawAsteroids()
{
	// brazowy
	glColor3f(0.6, 0.6, 0.2);

	for(Asteroid* asteroid : asteroids)
	{
		glPushMatrix();
		glTranslatef(asteroid->posx, asteroid->posy, 0.0);
		glScalef(asteroid->scale, asteroid->scale, asteroid->scale);
		glRotatef(asteroid->rotateAngle,asteroid->rotateX,asteroid->rotateY,asteroid->rotateZ);
		glutWireCube(0.1);
		glPopMatrix();
	}
}
void drawScore()
{
	glColor3f(1.0,1.0,1.0);
    glRasterPos3f(-0.9,-0.9,0.0);

	std::string scoreText = "Score: ";
	std::string scoreStr;
	if(score == -1)
	{
		glColor3f(1.0,0.2,0.2);
		scoreText = "";
		scoreStr = "GAME OVER";
	}
	else
		scoreStr=std::to_string(score);

	for (int i=0; i<scoreText.length(); i++) 
	{
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreText[i]);
    }
	for (int i=0; i<scoreStr.length(); i++) 
	{
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreStr[i]);
    }
}

void turnLeft()
{
	if (rotateShip > -55)
		rotateShip -= 5;
	if (translateShip > -0.95)
		translateShip -= 0.02;

}
void turnRight()
{
	if (rotateShip < 55)
		rotateShip += 5;
	if(translateShip < 0.95)
		translateShip +=0.02;
}
void shoot()
{
	bullets.push_front(new Bullet(translateShip));
}
void createAsteroid()
{
	asteroids.push_front(new Asteroid());
}

void Display()
{
	Sleep(1);
	startFrame = clock();

	// kolor t³a - zawartoœæ bufora koloru
	glClearColor(0.1, 0.1, 0.1, 1.0);
	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);
	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);
	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// rysowanie
	drawShip();
	drawBullets();
	drawAsteroids();
	drawScore();

	// skierowanie poleceñ do wykonania
	glFlush();
	// zamiana buforów koloru
	glutSwapBuffers();
	endFrame = clock() - startFrame;
}
void Reshape(int width, int height)
{
	// obszar renderingu - ca³e okno
	glViewport(0, 0, width, height);	

	// generowanie sceny 3D
	Display();
}

void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == ' ')
		shoot();
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
		leftPressed = true;
	if (key == GLUT_KEY_RIGHT)
		rightPressed = true;
}
void SpecialKeysUp(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
		leftPressed = false;
	if (key == GLUT_KEY_RIGHT)
		rightPressed = false;
}

void reset()
{
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

	rotateShip = 0.0;
	translateShip = 0.0;
	score = 0;
	asteroidSpeed = 0.01;
	spawnTime = 1.0;
}
bool bulletCollision()
{
	std::list<Asteroid*>::iterator a = asteroids.begin();
	while(a!=asteroids.end())
	{
		float AsteroidSphere = (((*a)->scale * 0.1) * sqrt(3))/2;
		std::list<Bullet*>::iterator b = bullets.begin();
		while(b!=bullets.end())
		{
			float distance = sqrt((((*a)->posx-(*b)->posx)*((*a)->posx-(*b)->posx))+(((*a)->posy-(*b)->posy)*((*a)->posy-(*b)->posy)));
			//jeœli koliduje to ustaw jako nieaktywne
			if(distance < AsteroidSphere)
			{
				(*a)->active = false;
				(*b)->active = false;
				return true;
			}
			b++;
		}
		a++;
	}
	return false;
}
bool shipCollision()
{
	for(Asteroid* asteroid : asteroids)
	{
		float sphere = asteroid->scale * 0.1 * (sqrt(3)/2);
		if(asteroid->posy - sphere < -0.85 && asteroid->posy + sphere > -0.90 )
		{
			if(asteroid->posx < translateShip + 0.03 && asteroid->posx > translateShip -0.03)
			{
				score = -1;
				Display();
				Sleep(2000);
				return true;
			}
		}
	}
	return false;
}

void Loop()
{
	//sterowanie statkiem
	if(leftPressed)
		turnLeft();
	if(rightPressed)
		turnRight();
	//powrót do pozycji wyjœciowej (obrót statku)
	if (!leftPressed && !rightPressed)
	{
		if (rotateShip < -5)
			rotateShip += 5;
		else if (rotateShip > 5)
			rotateShip -= 5;
		else
			rotateShip = 0.0;
	}
	//przesuñ pociski i usuñ te, które wysz³y poza ekran
	std::list<Bullet*>::iterator it = bullets.begin();
	while(it != bullets.end())
	{
		if((*it)->active)
		{
			(*it)->moveBullet();
			++it;
		}
		else
		{
			Bullet* deleteThis = *it;
			bullets.erase(it++);
			delete deleteThis;
		}
	}
	//przesuñ asteroidy i usuñ te, które wysz³y poza ekran
	std::list<Asteroid*>::iterator itt = asteroids.begin();
	while(itt != asteroids.end())
	{
		if((*itt)->active)
		{
			(*itt)->moveAsteroid();
			++itt;
		}
		else
		{
			Asteroid* deleteThis = *itt;
			asteroids.erase(itt++);
			delete deleteThis;
		}
	}
	//odmierz czas klatki i stwórz asteroid jeœli min¹³ okreœlony czas
	frameTime += endFrame / CLOCKS_PER_SEC;
	if(frameTime > spawnTime) 
	{
		frameTime = 0.0;
		createAsteroid();
	}
	//dostosuj prêdkoœæ asteroidów oraz czêstotliwoœæ tworzenia ich
	asteroidSpeed = 0.01 + (score/1000);
	if(spawnTime > 0.01)
		spawnTime = 1.0 - (score/100.0); 
	//usuñ zestrzelone asteroidy
	if(bulletCollision()) score++;
	if(shipCollision()) reset();

	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

int main(int argc, char * argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// rozmiary g³ównego okna programu
	glutInitWindowSize(800, 600);

	// utworzenie g³ównego okna programu
	glutCreateWindow("Asteroidy");

	// do³¹czenie funkcji generuj¹cej scenê 3D
	glutDisplayFunc(Display);

	// do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// do³¹czenie funkcji obs³ugi klawiatury
	glutKeyboardFunc(Keyboard);

	// do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);
	glutSpecialUpFunc(SpecialKeysUp);

	// pêtla
	glutIdleFunc(Loop);

	// wprowadzenie programu do obs³ugi pêtli komunikatów
	glutMainLoop();
	return 0;
}