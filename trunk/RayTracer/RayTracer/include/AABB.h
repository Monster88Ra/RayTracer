#pragma once

#include <algorithm>
#include "Ray.h"
#include "MathUtiliy.h"

/*****************************
*	Class of the AABB.	     *
******************************/

class  AABB
{
public:
	AABB(Vector3f minBound = Vector3f(), Vector3f maxBound = Vector3f());

	bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr);

	Vector3f GetCenter() const;
	Vector3f GetDeminsions() const;
public:
	Vector3f min, max;
};
inline AABB::AABB(Vector3f minBound , Vector3f maxBound ):
	min(minBound),max(maxBound)
{

}


inline bool AABB::IsIntersectingRay(Ray ray, float *out_ValueT )
{
	// a better way is Fast Ray-Box Intersection by Andrew Woo from "Graphics Gems", Academic Press, 1990
	/**
	* Ray-AABB intersection test from Real-Time Collision Detection by Christer Ericson p. 181
	* Test checks AABB slabs against ray for concurrent intersection with each slab
	**/

	float tMin = 0.0f;
	float tMax = (out_ValueT) ? *out_ValueT : std::numeric_limits<float>::max();

	const Vector3f& RayD = ray.direction;
	const Vector3f& RayO = ray.origin;

	for (int i = 0; i < 3; i++)
	{
		if (abs(RayD[i]) < _EPSILON)
		{
			//Ray is parallel to this slab, no hit unless origin is within slab
			if (RayO[i] < min[i] || RayO[i] > max[i])
				return false;
		}
		else
		{
			// Get intersection of t value with near and far planes of slab
			float ood = 1.0f / RayD[i];
			float t1 = (min[i] - RayO[i]) * ood;
			float t2 = (max[i] - RayO[i]) * ood;

			// Make t1 be the near plane
			if (t1 > t2)
				std::swap(t1, t2);

			// Compute intersection of slab intersection intervals
			tMin = std::max(tMin, t1);
			tMax = std::min(tMax, t2);

			// Exit if no collision when slab intersection becomes empty
			if (tMin > tMax)
				return false;
		}
	}

	// Ray intersects all slabs, return nearest t value if less than tValueOut
	if (out_ValueT)
	{
		if (tMin < *out_ValueT)
		{
			*out_ValueT = tMin;
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

inline Vector3f AABB::GetCenter() const
{
	return Vector3f(min + max) * 0.5;
}

inline Vector3f AABB::GetDeminsions() const
{
	return max - min;
}




