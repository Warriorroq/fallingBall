#include <iostream>
#include "Math.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace sf;

using namespace std;


const float gravity = 9.8f;
const float FPS = 75.0f;

namespace myVector {
	struct Vector3 {
	public:
		double x;
		double y;
		double z;
		Vector3(double x = 0, double y = 0, double z = 0, string name = "") : x(x), y(y), z(z), name(name) {
			if (name.length() == 0)
				SetRandomName();
		}

		virtual void operator +=(Vector3 vec) {
			x += vec.x;
			y += vec.y;
			z += vec.z;
		}

		virtual void operator -=(Vector3 vec) {
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
		}

		virtual bool operator ==(Vector3 vec) {
			return (x == vec.x && y == vec.y && z == vec.z);
		}

		virtual void operator *=(double vec) {
			x *= vec;
			y *= vec;
			z *= vec;
		}

		virtual void operator /=(double vec) {
			x /= vec;
			y /= vec;
			z /= vec;
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

		string GetName() {
			return name;
		}

		void SetName(string name) {
			this->name = name;
		}

		void PrintInfo() {
			cout << "Name : " << name << endl;
			cout << "Vector :" << "(" << x << "," << y << "," << z << ")" << endl;
			cout << "Lenght : " << Length() << endl;

		}

	private:
		string name;
		void SetRandomName() {
			this->name = "vector " + to_string(rand() % 15);
		}
	};
}
struct Ball {
public:
	myVector::Vector3 velocity;
	myVector::Vector3 position;
	myVector::Vector3 color;
	CircleShape circle;
	Ball(myVector::Vector3 position = myVector::Vector3(0, 0), double raduis = 10, myVector::Vector3 velocity = myVector::Vector3(0, 0), myVector::Vector3 color = myVector::Vector3(255, 255, 255)) : position(position), velocity(velocity),color(color){
		circle = CircleShape(raduis);
		circle.setPosition(this->position.x, this->position.y);
		circle.setFillColor(Color(this->color.x, this->color.y, this->color.z));
		velocity2 = this->velocity;
	}
	void Move(float frameTime) {

		if (position.y < 380)
			velocity.y += gravity * frameTime;

		if (position.y > 380)
			velocity.y *= -0.9f;

		if (position.x < -5 || position.x > 790)
			velocity.x *= -1;

		if (velocity.x > 0)
			velocity.x -= velocity2.x / 10.0f * frameTime;
		else
			velocity.x += velocity2.x / 10.0f * frameTime;

		position += velocity;

		circle.setPosition(this->position.x, this->position.y);
	}
private:
	myVector::Vector3 velocity2;
};

Ball CreateBall() {
	float power = -1;
	float angle = 100;
	float startPos = -1;

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
	float power = -1;
	float angle = 100;
	float startPos = -1;

	myVector::Vector3 pos(0, 0);
	myVector::Vector3 velocity(0, 0);
	float radius = 10;
	myVector::Vector3 color(255, 255, 255);

	pos.y = 340 -abs( rand() % 360 + 20);

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
	RenderWindow window(VideoMode(800, 400, 32), "Hello");
	Clock clock;
	Event ev;
	float frameTime = 1.0f / FPS;
	bool draw = true;

	//////////////////////////////////////
	int balls = 0;
	cout << "how many balls you want (1<x<101)" << endl;
	cin >> balls;
	balls = abs(balls);
	//Ball *bal = (Ball *)malloc(balls * sizeof(Ball));
	Ball bal[100];
	cout << "coms : 1 - random | 0 - my ball | 2 - random balls";
	int a = 0;
	cin >> a;
	if(a != 2)
		for(int i =0; i< balls;i++){
			int a = 0;
			cin >> a;
			Ball baL;
			if (a == 0)
				baL = CreateBall();
			else
				baL = CreateRandomBall();
			bal[i] = baL;}
	else {
		for (int i = 0; i < balls; i++) {
			Ball baL;
			baL = CreateRandomBall();
			bal[i] = baL;
		}
	}
	//////////////////////////////////////

	while (window.isOpen())
	{
		if (clock.getElapsedTime().asSeconds() >= frameTime) {
			draw = true;
			clock.restart();
			for (int i = 0; i < balls; i++) 
				bal[i].Move(frameTime);
		}
		else {
			Time sleepTime = seconds((frameTime)-clock.getElapsedTime().asSeconds());
			sleep(sleepTime);
		}

		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed) window.close();
		}

		if (draw) {
			draw = false;
			window.clear(Color::Black);
			for (int i = 0; i < balls; i++) {
				window.draw(bal[i].circle);
			}

			window.display();
		}
	}
	//free(bal);
	return 0;
}

