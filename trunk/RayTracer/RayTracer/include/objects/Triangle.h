#pragma once
#include "object.h"
#include "Vector2.h"

class  Triangle:public Object
{
public:
	 Triangle();
	 // v0 v1 v2 are the three vertice of triangles
	 Triangle(Vector3f v0, Vector3f v1, Vector3f v2,const Material &lightingMaterial = Material());
	 Triangle(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f normal, const Material &lightingMaterial = Material());

	 bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr, Intersection *out_Intersection = nullptr) override;

	 Material GetMaterial(Vector3f surfacepoint) override;

	 // UV0 UV1 UV2 are the UV coordinate of three vertice
	 void SetUVCoordinate(const Vector2f &UV0, const Vector2f &UV1, const Vector2f &UV2);
	~ Triangle();
private:
	void ConstructIntersection(Vector3f IntersectionPoint, Intersection* IntersectionOut);

	void ConstructAABB(Vector3f Min = Vector3f(), Vector3f Max = Vector3f()) override;
private:
	Vector3f m_V0;
	Vector3f m_V1;
	Vector3f m_V2;
	Vector3f m_Normal;

	Vector2f m_UV0;
	Vector2f m_UV1;
	Vector2f m_UV2;
};

