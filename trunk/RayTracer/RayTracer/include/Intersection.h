#pragma once

#include "Vector3.h"
class Object;

/*****************************
*	Struct for describing the properties of an intersection  *
******************************/
struct Intersection
{

public:
	inline Intersection() :
		object(nullptr),
		point(),
		normal()
	{

	}

	inline Intersection(Object &intersectedObject, Vector3f intersectionPoint, Vector3f surfaceNormal) :
		object(&intersectedObject),
		point(intersectionPoint),
		normal(surfaceNormal)
	{

	}

	~Intersection()
	{

	}

public:
	Object *object;		// the object of intersection
	Vector3f point;
	Vector3f normal;
};