#include "Plane.h"
#include "Intersection.h"

Plane::Plane(const Material & lightingMaterial, Vector3f planeNormal, Vector3f planePoint):
	Object(lightingMaterial),
	m_Normal(planeNormal.Normalize())
{
	m_DistanceFromOrigin = Vector3f::Dot(-planeNormal, planePoint);
	// rotate 90 degrees by z axis 
	m_UAxis = Vector3f(m_Normal.y,m_Normal.z,-m_Normal.x);
	m_VAxis = (Vector3f::Cross(planeNormal, m_UAxis).Normalize());

	ConstructAABB();
}

Plane::~Plane()
{
}

bool Plane::IsIntersectingRay(Ray ray, float * out_ValueT, Intersection * out_Intersection)
{
	if (!IsEnable())
	{
		return false;
	}

	// transform ray to local space
	ray = GetWorldInvTransform().TransformRay(ray);

	float temp = Vector3f::Dot(ray.direction, m_Normal);
	if (abs(temp) < _EPSILON)
	{
		return false;
	}

	const float tValue = (-Vector3f::Dot(ray.origin, m_Normal) + m_DistanceFromOrigin) / temp;

	if (tValue < _EPSILON)
	{
		return false;
	}

	if (tValue)
	{
		if (out_Intersection && tValue < *out_ValueT)
		{
			*out_ValueT = tValue;
			ConstructIntersection(ray.origin + ray.direction*tValue, *out_Intersection);
		}
		else if (tValue > *out_ValueT)
		{
			return false;
		}
	}
	return true;
}

Material Plane::GetMaterial(Vector3f surfacePoint)
{
	const TextureInfo &diffuseTextureInfo = m_Material.GetDiffuseTexture();

	if (!diffuseTextureInfo.texture)
	{
		return m_Material;
	}

	// transform to local space
	surfacePoint = GetWorldInvTransform().TransformPosition(surfacePoint);

	//calculate U V 
	// we caculate the projection of vector that from original to point
	// the projection can be (Dot(Dot(axis,vec),axis))/squre of n
	// because the n is unit
	const float U = Vector3f::Dot(surfacePoint, m_UAxis) * diffuseTextureInfo.UAxisScale;
	const float V = Vector3f::Dot(surfacePoint, m_VAxis) * diffuseTextureInfo.VAxisScale;

	m_Material.SetDiffuse(diffuseTextureInfo.texture->GetSample(U, V));

	return m_Material;
}

void Plane::ConstructIntersection(const Vector3f  intersectionPoint, Intersection  &intersectionOut)
{
	intersectionOut.normal = m_Normal;
	intersectionOut.object = this;
	intersectionOut.point = GetWorldTransform().TransformPosition(intersectionPoint + m_Normal * _EPSILON);
}

void Plane::ConstructAABB(Vector3f min, Vector3f max)
{
	max.x = std::numeric_limits<float>::max();
	min.x = -std::numeric_limits<float>::max();

	max.y = std::numeric_limits<float>::max();;
	min.y = -std::numeric_limits<float>::max();;

	max.z = std::numeric_limits<float>::max();
	min.z = -std::numeric_limits<float>::max();

	AABB aabb(min,max);
	SetBoundingBox(aabb);
}
