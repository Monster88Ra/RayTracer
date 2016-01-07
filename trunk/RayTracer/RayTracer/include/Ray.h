#pragma once

#include <limits>
#include "Vector3.h"

struct Ray
{
	inline Ray(Vector3f originPoint = Vector3f(), Vector3f directionVec = Vector3f()):
		origin(originPoint),direction(directionVec)
	{
	}

	inline Vector3f GetPointAtParamter(float t)
	{
		return origin + direction * t;
	}

	Vector3f origin;
	Vector3f direction;
};
