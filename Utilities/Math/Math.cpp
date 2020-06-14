//Math.cpp
#include "Math.h"

Vector2 Math::worldToScreen(Vector3 worldLocation, Vector3 cameraPosition, int screenWidth, int screenHeight, Vector2 fieldOfView, Vector3* matrices)
{
    Vector3 local = worldLocation - cameraPosition;
    Vector3 trans = Vector3(local * matrices[1], local * matrices[2], local * matrices[0]);

    if (trans.z < 0.01f)
        return Vector2();

    float x = (((float)screenWidth / 2) * (1 - (trans.x / fieldOfView.x / trans.z)));
    float y = (((float)screenHeight / 2) * (1 - (trans.y / fieldOfView.y / trans.z)));
    return Vector2(x, y);
}
