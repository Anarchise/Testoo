//Vector2.cpp
#include "Vector2.h"

Vector2::Vector2()
{
	this->x = 0;
	this->y = 0;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2 operator+(Vector2 a, Vector2 b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator-(Vector2 a, Vector2 b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}

float operator*(Vector2 a, Vector2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

std::ostream& operator<<(std::ostream& os, Vector2 vector)
{
	return os << "Vector2(" << vector.x << ", " << vector.y << ")";
}
