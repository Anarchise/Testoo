#pragma once
#include <iostream>

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	float distance(Vector3 vector);
	float length();
	bool isZero();
public:
	float x;
	float y;
	float z;
};

Vector3 operator+(Vector3 a, Vector3 b);
Vector3 operator-(Vector3 a, Vector3 b);
float operator*(Vector3 a, Vector3 b);
std::ostream& operator<<(std::ostream& os, Vector3 vector);
