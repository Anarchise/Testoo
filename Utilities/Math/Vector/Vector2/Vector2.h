#pragma once
#include <iostream>

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
public:
	float x;
	float y;
};

Vector2 operator+(Vector2 a, Vector2 b);
Vector2 operator-(Vector2 a, Vector2 b);
float operator*(Vector2 a, Vector2 b);
std::ostream& operator<<(std::ostream& os, Vector2 vector);
