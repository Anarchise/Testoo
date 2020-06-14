//Vector3.cpp
#include "Vector3.h"

Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::distance(Vector3 vector)
{
	float distance = 0.0f;
	distance += (float)pow(this->x - vector.x, 2);
	distance += (float)pow(this->y - vector.y, 2);
	distance += (float)pow(this->z - vector.z, 2);
	return (float)sqrt(distance);
}

float Vector3::length()
{
	float length = 0.0f;
	length += this->x * this->x;
	length += this->y * this->y;
	length += this->z * this->z;
	return (float)sqrt(length);
}

bool Vector3::isZero()
{
	return this->x == 0 && this->y == 0 && this->z == 0;
}

Vector3 operator+(Vector3 a, Vector3 b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator-(Vector3 a, Vector3 b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

float operator*(Vector3 a, Vector3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

std::ostream& operator<<(std::ostream& os, Vector3 vector)
{
	return os << "Vector3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
}
