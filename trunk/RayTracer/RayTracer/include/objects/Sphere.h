#pragma once

#include "object.h"

/*****************************
* Class of the spere in 3D space.
******************************/

class Sphere:public Object
{
public:
	Sphere(Vector3f center,float radius,Material lightingMaterial);
	~Sphere();

	bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr, Intersection *out_Intersection = nullptr) override;

	float GetRadius() const;
	void SetRadius(const float radius);

	Material GetMaterial(Vector3f surfacepoint) override;

private:
	// construct intersection info
	void ConstructIntersection(const Vector3f intersectionPoint, Intersection &intersectionOut);
	void ConstructAABB(Vector3f min = Vector3f(), Vector3f max = Vector3f()) override;
private:
	float m_Radius;
};
