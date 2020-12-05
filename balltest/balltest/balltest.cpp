#include <iostream>
#include "Math.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <stdlib.h>


using namespace sf;

using namespace std;


const float gravity = 9.8f;
const float FPS = 80.0f;

namespace myVector {
	struct Vector3 {
	public:
		double x;
		double y;
		double z;
		Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) 
		{}

		Vector3& operator +=(Vector3& vec) {
			x += vec.x;
			y += vec.y;
			z += vec.z;		
			return *this;
		}

		Vector3& operator -=(Vector3& vec) {
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

		bool operator ==(Vector3& vec) {
			return (x == vec.x && y == vec.y && z == vec.z);
		}

		Vector3& operator *=(double vec) {
			x *= vec;
			y *= vec;
			z *= vec;			
			return *this;
		}

		Vector3& operator /=(double vec) {
			x /= vec;
			y /= vec;
			z /= vec;
			return *this;
		}

		double Length() {
			return sqrt(x*x + y * y + z * z);
		}

		void Normalize() {
			double l = Length();
			x /= l;
			y /= l;
			z /= l;
		}
		void PrintInfo() {
			cout << "Vector :" << "(" << x << "," << y << "," << z << ")" << endl;
			cout << "Lenght : " << Length() << endl;
		}
	};
}
class Ball {
public:
	myVector::Vector3 velocity;
	myVector::Vector3 position;
	myVector::Vector3 color;
	CircleShape* circle;
	bool move = true;
	Ball(myVector::Vector3 position = myVector::Vector3(0, 0), double raduis = 10, myVector::Vector3 velocity = myVector::Vector3(0, 0), myVector::Vector3 color = myVector::Vector3(255, 255, 255)) : position(position), velocity(velocity),color(color){
		circle = new CircleShape((float)raduis);
		circle->setPosition((float)position.x, (float)position.y);
		circle->setFillColor(Color((Uint8)color.x, (Uint8)color.y, (Uint8)color.z));
		velocity2 = velocity;
	}
	void ChangeMove() {
		move = !move;
	}
	void Move(double frameTime) {

		if (move) {
			if (position.y < 380)
				velocity.y += gravity * frameTime;
			else
				velocity.y *= -0.9f;

			if (position.x < -5 || position.x > 790)
				velocity.x *= -1;

			if (velocity.x > 0)
				velocity.x -= velocity2.x / 10.0f * frameTime;
			else
				velocity.x += velocity2.x / 10.0f * frameTime;

			position += velocity;

			circle->setPosition((float)position.x, (float)position.y);
		}
	}
private:
	myVector::Vector3 velocity2;
};

Ball CreateBall() {
	float power = -1.0f;
	float angle = 100.0f;
	float startPos = -1.0f;

	myVector::Vector3 pos(0, 0);
	myVector::Vector3 velocity(0, 0);
	float radius = 10;
	myVector::Vector3 color(255, 255, 255);


	while (!(startPos >= 0 && startPos <= 380)) {
		cout << "start Y: (>0 & <380)";
		cin >> startPos;
		pos.y = 360 - startPos;
	}

	while (power < 0) {
		cout << "power: (>0)";
		cin >> power;
	}

	while (!(angle >= 0 && angle <= 90)) {
		cout << "angle: (0<x<90)";
		cin >> angle;

		velocity.x = power * sinf(angle);

		if (angle < 45)
			velocity.y = -power * cosf(angle);
		else
			velocity.y = power * cosf(angle);
	}
	return Ball(pos, radius, velocity, color);
}
Ball CreateRandomBall() {
	float power = -1.0f;
	float angle = 100.0f;
	float startPos = -1.0f;

	myVector::Vector3 pos(0, 0);
	myVector::Vector3 velocity(0, 0);
	float radius = 10;
	myVector::Vector3 color(255, 255, 255);

	pos.y = 340 - abs( rand() % 360 + 20);
	power = abs( rand() % 10) + 5;
	color.x = abs(rand() % 255);
	color.y = abs(rand() % 255);
	color.z = abs(rand() % 255);

	angle = abs( rand() % 90) + 15;

	velocity.x = power * sinf(angle);

	if (angle < 45)
		velocity.y = power * cosf(angle);
	else
		velocity.y = -power * cosf(angle);

	return Ball(pos, radius, velocity, color);
}

int main()
{
	RenderWindow window(VideoMode(840, 480, 512), "Balls dance");
	Clock clock;
	Event ev;
	float frameTime = 1.0f / FPS;
	bool draw = true;

	//////////////////////////////////////
	int countofballs = 0;
	cout << "how many balls you want" << endl;
	cin >> countofballs;
	countofballs = abs(countofballs);

	Ball *arrbals = (Ball*)malloc(countofballs * sizeof(Ball));
	cout << "coms : 1 - random | 0 - my ball | 2 - random balls";
	int command = 0;
	cin >> command;
	if (command != 2)
	{
		for (int i = 0; i < countofballs; i++) {
			cin >> command;
			if (command == 0)
				arrbals[i] = CreateBall();
			else
				arrbals[i] = CreateRandomBall();
		}
	}
	else
	{
		for (int i = 0; i < countofballs; i++)
			arrbals[i] = CreateRandomBall();

	}
	//////////////////////////////////////

	while (window.isOpen())
	{
		if (clock.getElapsedTime().asSeconds() >= frameTime) {
			draw = true;
			clock.restart();
			for (int i = 0; i < countofballs; i++)
				arrbals[i].Move(frameTime);
		}
		else {
			Time sleepTime = seconds((frameTime)-clock.getElapsedTime().asSeconds());
			sleep(sleepTime);
		}

		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed) window.close();

			if (ev.type == Event::MouseButtonPressed)
				for (int i = 0; i < countofballs; i++)
					arrbals[i].ChangeMove();
			if (ev.type == Event::MouseWheelScrolled) {
				if (countofballs > 10)
					countofballs -= countofballs / 10;
				realloc(arrbals, countofballs * sizeof(Ball));
			}
		}

		if (draw) {
			draw = false;
			window.clear(Color::Black);
			for (int i = 0; i < countofballs; i++) {
				window.draw(*(arrbals[i].circle));
			}
			window.display();
		}
	}
	free(arrbals);
	return 0;
}

