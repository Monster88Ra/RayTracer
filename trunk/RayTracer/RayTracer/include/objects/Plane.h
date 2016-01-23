#pragma once
#include "object.h"

/*****************************
* Class of the plane in 3D space.
* plane are define point p and normal n
* p dot n equal distanceFromOrigin
******************************/

class  Plane:public Object
{
public:
	 Plane(const Material &lightingMaterial, Vector3f planeNormal,Vector3f planePoint);
	~ Plane();

	bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr, Intersection *out_Intersection = nullptr) override;

	Material GetMaterial(Vector3f surfacePoint) override;

private:
	// construct intersection info
	void ConstructIntersection(const Vector3f intersectionPoint, Intersection &intersectionOut);
	void ConstructAABB(Vector3f min = Vector3f(), Vector3f max = Vector3f()) override;

private:
	Vector3f m_Normal;
	float m_DistanceFromOrigin;

	Vector3f m_UAxis;
	Vector3f m_VAxis;
};
