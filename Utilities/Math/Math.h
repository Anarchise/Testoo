#pragma once

#include "stdafx.h"

class Math
{
public:
	static Vector2 worldToScreen(Vector3 worldLocation, Vector3 cameraPosition, int screenWidth, int screenHeight, Vector2 fieldOfView, Vector3* matrices);
};
