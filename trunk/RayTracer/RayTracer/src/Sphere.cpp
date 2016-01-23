#include "Sphere.h"
#include "Intersection.h"

Sphere::Sphere(Vector3f center, float radius, Material lightingMaterial):
	Object(lightingMaterial),
	m_Radius(radius)
{
	SetLocalOrigin(center);
	ConstructAABB();
}

Sphere::~Sphere()
{
}

bool Sphere::IsIntersectingRay(Ray ray, float * out_ValueT, Intersection * out_Intersection)
{
	if (!IsEnable())
	{
		return false;
	}

	// transform ray to local space
	ray = GetWorldInvTransform().TransformRay(ray);
	const Vector3f rayDirection = ray.direction;
	const Vector3f rayOrigin = ray.origin;

	// reference: Fundamentals of Computer Graphics chapter 4.4
	// because in local space the center of sphere is (0.0f,0.0f,0.0f)
	const float B = Vector3f::Dot(rayDirection, rayOrigin);
	const float C = Vector3f::Dot(rayOrigin, rayOrigin) - m_Radius * m_Radius;

	// If the ray origin is outside the sphere and direction is pointing away from sphere
	if (C > 0 && B > 0)
	{
		return false;
	}

	const float delta = B * B - C;
	if (delta < 0)
	{
		return false;
	}
	float smallestValueT = -B - sqrt(delta);

	if (smallestValueT < _EPSILON)
	{
		return false;
	}

	if (out_ValueT)
	{
		if (out_Intersection && smallestValueT < *out_ValueT)
		{
			*out_ValueT = smallestValueT;

			//transform intersect point to wolrd space
			Vector3f intersection = rayOrigin + smallestValueT*rayDirection;
			intersection = GetWorldTransform().TransformPosition(intersection);

			ConstructIntersection(intersection,*out_Intersection);
		}
		else if (smallestValueT > *out_ValueT)
		{
			return false;
		}
	}

	return true;
}

float Sphere::GetRadius() const
{
	return m_Radius;
}

void Sphere::SetRadius(const float radius)
{
	m_Radius = radius;
}

Material Sphere::GetMaterial(Vector3f surfacepoint)
{
	TextureInfo diffuseInfo = m_Material.GetDiffuseTexture();
	if (!diffuseInfo.texture)
	{
		return m_Material;
	}

	// transform to local space and convert to vector
	surfacepoint = GetWorldInvTransform().TransformPosition(surfacepoint).Normalize();
	const Vector3f Vn(0.0f, 1.0f, 0.0f); // points to north pole
	const Vector3f Ve(0.0f, 0.0f, 1.0f); // points to equator 

	// in fact the spheroidal coordinates about three parameter theta phi and radius
	// map theta and phi to U and V
	// theta/2*pi = U   phi/pi = V
	const float& Phi = acosf(-Vector3f::Dot(Vn, surfacepoint)); // get latitude
	const float& V = diffuseInfo.VAxisScale * Phi * m_Radius / _PI;

	const float& Theta = (acosf(Vector3f::Dot(surfacepoint, Ve) / sinf(Phi))) / (2 * _PI);
	float U;

	if (Vector3f::Dot(Vector3f::Cross(Vn, Ve), surfacepoint) > 0)
	{
		U = Theta * m_Radius * diffuseInfo.UAxisScale;
	}
	else
	{
		U = (1 - Theta) * m_Radius * diffuseInfo.UAxisScale;
	}

	m_Material.SetDiffuse(diffuseInfo.texture->GetSample(U, V));

	return m_Material;
}

void Sphere::ConstructIntersection(const Vector3f  intersectionPoint, Intersection &intersectionOut)
{
	intersectionOut.normal = (intersectionPoint - GetWorldOrigin()).Normalize();
	intersectionOut.point = intersectionPoint;
	intersectionOut.object = this;
}

void Sphere::ConstructAABB(Vector3f min, Vector3f max)
{
	//turn off compiler warnings
	min; max;
	SetBoundingBox(AABB(Vector3f(-m_Radius, -m_Radius, -m_Radius), Vector3f(m_Radius, m_Radius, m_Radius)));
}
