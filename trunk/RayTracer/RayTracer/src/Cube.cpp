#include "Cube.h"
#include "Intersection.h"

#include <limits>

Cube::Cube(Vector3f center, Material &lightingMaterial):
	Object(lightingMaterial)
{
	SetLocalOrigin(center);
	ConstructAABB();
}

Cube::~Cube()
{
}

bool Cube::IsIntersectingRay(Ray ray, float * out_ValueT, Intersection * out_Intersection)
{
	if (!IsEnable)
	{
		return false;
	}

	// use bounding box test
	const float originalT = (out_ValueT) ? *out_ValueT : 0;

	// transform ray to world space
	ray = GetWorldTransform().TransformRay(ray);

	const bool isIntersecting = GetBoundingBox().IsIntersectingRay(ray, out_ValueT);
	if (isIntersecting && out_Intersection && originalT > *out_ValueT)
	{
		// get intersection point in world space
		Vector3f intersectingpoint = GetWorldTransform().TransformPosition(ray.origin + *out_ValueT * ray.direction);
		ConstructIntersection(intersectingpoint, out_Intersection);
	}
	else if (out_ValueT && originalT < *out_ValueT)
	{
		return false;
	}
	return isIntersecting;
}

Material Cube::GetMaterial(Vector3f surfacePoint)
{
	// ???
	const TextureInfo& DiffuseInfo = m_Material.GetDiffuseTexture();
	if (!DiffuseInfo.texture)
		return m_Material;

	surfacePoint = GetWorldInvTransform().TransformPosition(surfacePoint);
	const Vector3f& Scale = GetWorldTransform().GetScale();

	float U = 0.0f, V = 0.0f;
	if (1.0f - abs(surfacePoint.x) < _EPSILON)
	{
		U = Scale.z * (surfacePoint.z + 1.0f) / 2.0f;
		V = Scale.y * (surfacePoint.y + 1.0f) / 2.0f;
	}
	else if (1.0f - abs(surfacePoint.y)< _EPSILON)
	{
		U = Scale.x * (surfacePoint.x + 1.0f) / 2.0f;
		V = Scale.z * (surfacePoint.z + 1.0f) / 2.0f;
	}
	else
	{
		U = Scale.x * (surfacePoint.x + 1.0f) / 2.0f;
		V = Scale.y * (surfacePoint.y + 1.0f) / 2.0f;
	}

	U *= DiffuseInfo.UAxisScale;
	V *= DiffuseInfo.VAxisScale;

	m_Material.SetDiffuse(DiffuseInfo.texture->GetSample(U, V));

	return m_Material;
}

void Cube::ConstructIntersection(const Vector3f & intersectionPoint, Intersection * intersectionOut)
{
	float largestSide = 0;
	int intersectionAxis = 0;
	int intersectionSide = 0;

	// transform point to local space
	const Matrix4 modelInverse = GetWorldInvTransform();
	const Vector3f intersectionInObjectSpace = modelInverse.TransformPosition(intersectionPoint);
	// get largest side
	for (int i = 0; i < 3; i++)
	{
		if (abs(intersectionInObjectSpace[i]) > largestSide)
		{
			largestSide = abs(intersectionInObjectSpace[i]);
			// judge inside or outside
			intersectionSide = (intersectionInObjectSpace[i] < 0.0f) ? -1:1;
			intersectionAxis = i;
		}
	}

	Vector3f normal;
	normal[intersectionAxis] = (float)intersectionSide;
	// transform to world spcae
	normal = modelInverse.TransformDirection(normal).Normalize();

	intersectionOut->normal = normal;
	intersectionOut->object = this;
	intersectionOut->point = intersectionPoint + (normal * _EPSILON);
}

void Cube::ConstructAABB(Vector3f min, Vector3f max)
{
	min = Vector3f(-1, -1, -1);
	max = Vector3f(1, 1, 1);

	SetBoundingBox(AABB{ min,max });
}
