#include "Triangle.h"
#include "Intersection.h"

Triangle::Triangle():
	Object(),
	m_V0(),
	m_V1(),
	m_V2(),
	m_Normal(),
	m_UV0(),
	m_UV1(),
	m_UV2()
{

}

Triangle::Triangle(Vector3f v0, Vector3f v1, Vector3f v2, const Material & lightingMaterial):
	Object(lightingMaterial),
	m_V0(v0),
	m_V1(v1),
	m_V2(v2),
	m_UV0(),
	m_UV1(),
	m_UV2()
{
	Vector3f e1 = v2 - v0;
	Vector3f e2 = v1 - v0;
	m_Normal = Vector3f::Cross(e1, e2);
	m_Normal.Normalize();
	ConstructAABB();
	
}

Triangle::Triangle(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f normal, const Material & lightingMaterial):
	Object(lightingMaterial),
	m_V0(v0),
	m_V1(v1),
	m_V2(v2),
	m_Normal(normal),
	m_UV0(),
	m_UV1(),
	m_UV2()
{
	
}

bool Triangle::IsIntersectingRay(Ray ray, float * out_ValueT, Intersection * out_Intersection)
{
	if (!IsEnable())
	{
		return false;
	}

	//just test the front face
	float gradient = Vector3f::Dot(m_Normal, ray.direction);
	if (!(gradient < 0.0f))
	{
		return false;
	}

	// reference from Fundamental of Computer Graphics
	// E1 E2
	Vector3f E1 = m_V1 - m_V0;
	Vector3f E2 = m_V2 - m_V0;

	Vector3f P = Vector3f::Cross(ray.direction, E2);

	// determinant
	float det = Vector3f::Dot(P, E1);
	// Vector T
	Vector3f T;
	if (det > 0)
	{
		T = ray.origin - m_V0;
	}
	else
	{
		det *= -1;
		T = m_V0 - ray.origin;
	}

	// if det is near 0, ray lies in the same plane of triangle
	if (det < _EPSILON)
	{
		return false;
	}

	float u, v;
	u = Vector3f::Dot(P, T);

	// check u is >=0 and < =1
	if (u < 0.0f || u >det)
	{
		return false;
	}

	// Vector Q
	Vector3f Q;
	Q = Vector3f::Cross(T, E1);
	// caculate v
	v = Vector3f::Dot(Q, ray.direction);
	// check v is >=0 and v + u <= 1
	if (v < 0.0f || (u + v > det))
	{
		return false;
	}

	//caculate the t
	float t = Vector3f::Dot(Q, E2);

	float invDet = 1.0f / det;
	t *= invDet;
	u *= invDet;
	v *= invDet;

	if (out_ValueT)
	{
		if (out_Intersection && t < *out_ValueT)
		{
			*out_ValueT = t;
			ConstructIntersection(ray.origin + t * ray.direction, out_Intersection);
		}
		else if (t > *out_ValueT)
		{
			return false;
		}
	}
	return true;
}

Material Triangle::GetMaterial(Vector3f surfacepoint)
{
	const TextureInfo &diffuseinfo = m_Material.GetDiffuseTexture();

	if (!diffuseinfo.texture)
	{
		return m_Material;
	}

	float B[3];
	// get U V by barycentric
	ComputeBarycentricCoords3d(m_V0, m_V1, m_V2, surfacepoint, B);

	// U V coordinate
	Vector2f UV = B[0] * m_UV0 + B[1] * m_UV1 + B[2] * m_UV2;
	UV.x *= diffuseinfo.UAxisScale;
	UV.y *= diffuseinfo.VAxisScale;

	m_Material.SetDiffuse(diffuseinfo.texture->GetSample(UV.x, UV.y));

	return m_Material;


}

void Triangle::SetUVCoordinate(const Vector2f & UV0, const Vector2f & UV1, const Vector2f & UV2)
{
	m_UV0 = UV0;
	m_UV1 = UV1;
	m_UV2 = UV2;
}

Triangle::~Triangle()
{
}

void Triangle::ConstructIntersection(Vector3f IntersectionPoint, Intersection * IntersectionOut)
{
	IntersectionOut->object = this;
	IntersectionOut->normal = m_Normal;
	IntersectionOut->point = IntersectionPoint;
}

void Triangle::ConstructAABB(Vector3f Min, Vector3f Max)
{
	Max = Vector3f(std::max({ m_V0.x, m_V1.x, m_V2.x }), std::max({ m_V0.y, m_V1.y, m_V2.y }), std::max({ m_V0.z, m_V1.z, m_V2.z }));
	Min = Vector3f(std::min({ m_V0.x, m_V1.x, m_V2.x }), std::min({ m_V0.y, m_V1.y, m_V2.y }), std::min({ m_V0.z, m_V1.z, m_V2.z }));
	SetBoundingBox(AABB(Min, Max));
}
