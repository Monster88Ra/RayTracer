#include "object.h"

Object::Object(const Material & lightingMaterial):
	m_BoundingBox(),
	m_Transform(),
	m_InvTransform(),
	m_IsEnable(true),
	m_Material(lightingMaterial),
	m_ParentObject(nullptr)
{

}

Object::~Object()
{

}

void Object::SetMaterial(const Material & newMaterial)
{
	m_Material = newMaterial;
}

Material Object::GetMaterial() const
{
	return m_Material;
}

AABB Object::GetBoundingBox() const
{
	return m_BoundingBox;
}

AABB Object::GetWorldAABB() const
{
	// AABB transformation from 3D Math Primer for Graphics
	// and Game Development
	// Minimize each element of the transform matrix to get
	// the transformed AABB

	const Matrix4& WorldTransform(GetWorldTransform());
	const AABB& Box(m_BoundingBox);

	// Start with the object origin
	Vector3f Min = WorldTransform.GetOrigin();
	Vector3f Max(Min);

	// First column
	if (WorldTransform.m[0][0] > 0.0f)
	{
		Min.x += WorldTransform.m[0][0] * Box.min.x; Max.x += WorldTransform.m[0][0] * Box.max.x;
	}
	else
	{
		Min.x += WorldTransform.m[0][0] * Box.max.x; Max.x += WorldTransform.m[0][0] * Box.min.x;
	}

	if (WorldTransform.m[1][0] > 0.0f)
	{
		Min.y += WorldTransform.m[1][0] * Box.min.x; Max.y += WorldTransform.m[1][0] * Box.max.x;
	}
	else
	{
		Min.y += WorldTransform.m[1][0] * Box.max.x; Max.y += WorldTransform.m[1][0] * Box.min.x;
	}

	if (WorldTransform.m[2][0] > 0.0f)
	{
		Min.z += WorldTransform.m[2][0] * Box.min.x; Max.z += WorldTransform.m[2][0] * Box.max.x;
	}
	else
	{
		Min.z += WorldTransform.m[2][0] * Box.max.x; Max.z += WorldTransform.m[2][0] * Box.min.x;
	}

	// Second column
	if (WorldTransform.m[0][1] > 0.0f)
	{
		Min.x += WorldTransform.m[0][1] * Box.min.y; Max.x += WorldTransform.m[0][1] * Box.max.y;
	}
	else
	{
		Min.x += WorldTransform.m[0][1] * Box.max.y; Max.x += WorldTransform.m[0][1] * Box.min.y;
	}

	if (WorldTransform.m[1][1] > 0.0f)
	{
		Min.y += WorldTransform.m[1][1] * Box.min.y; Max.y += WorldTransform.m[1][1] * Box.max.y;
	}
	else
	{
		Min.y += WorldTransform.m[1][1] * Box.max.y; Max.y += WorldTransform.m[1][1] * Box.min.y;
	}

	if (WorldTransform.m[2][1] > 0.0f)
	{
		Min.z += WorldTransform.m[2][1] * Box.min.y; Max.z += WorldTransform.m[2][1] * Box.max.y;
	}
	else
	{
		Min.z += WorldTransform.m[2][1] * Box.max.y; Max.z += WorldTransform.m[2][1] * Box.min.y;
	}

	// Third column
	if (WorldTransform.m[0][2] > 0.0f)
	{
		Min.x += WorldTransform.m[0][2] * Box.min.z; Max.x += WorldTransform.m[0][2] * Box.max.z;
	}
	else
	{
		Min.x += WorldTransform.m[0][2] * Box.max.z; Max.x += WorldTransform.m[0][2] * Box.min.z;
	}

	if (WorldTransform.m[1][2] > 0.0f)
	{
		Min.y += WorldTransform.m[1][2] * Box.min.z; Max.y += WorldTransform.m[1][2] * Box.max.z;
	}
	else
	{
		Min.y += WorldTransform.m[1][2] * Box.max.z; Max.y += WorldTransform.m[1][2] * Box.min.z;
	}

	if (WorldTransform.m[2][2] > 0.0f)
	{
		Min.z += WorldTransform.m[2][2] * Box.min.z; Max.z += WorldTransform.m[2][2] * Box.max.z;
	}
	else
	{
		Min.z += WorldTransform.m[2][2] * Box.max.z; Max.z += WorldTransform.m[2][2] * Box.min.z;
	}

	return AABB(Min, Max);
}

void Object::SetParent(Object & parent)
{
	m_ParentObject = &parent;
}

Matrix4 Object::GetInvTransform() const
{
	return m_InvTransform;
}

Matrix4 Object::GetWorldInvTransform() const
{
	if (m_ParentObject)
	{
		return m_ParentObject->GetWorldInvTransform() * m_InvTransform;
	}
	return m_InvTransform;
}

Matrix4 Object::GetTransform() const
{
	return m_Transform;
}

Matrix4 Object::GetWorldTransform() const
{
	if (m_ParentObject)
	{
		return m_ParentObject->GetTransform() * m_Transform;
	}
	return m_Transform;
}

void Object::SetTransform(const Matrix4 & trans)
{
	m_Transform = trans;
	m_InvTransform = m_Transform.Inverse();
}

void Object::Rotate(EAxis axis, float degree)
{
	m_Transform.Rotate(axis, degree);
	m_InvTransform = m_Transform.Inverse();
}

void Object::Scale(EAxis Axis, float Scale)
{
	m_Transform.Scale(Axis, Scale);
	m_InvTransform = m_Transform.Inverse();
}

void Object::SetLocalOrigin(const Vector3f & origin)
{
	m_Transform.SetOrigin(origin);
	m_InvTransform = m_Transform.Inverse();
}

Vector3f Object::GetLocalOrigin() const
{
	return m_Transform.GetOrigin();
}

Vector3f Object::GetWorldOrigin() const
{
	return m_Transform.GetOrigin();
}

bool Object::IsEnable() const
{
	return m_IsEnable;
}

void Object::SetEnable(bool flag)
{
	m_IsEnable = flag;
}

void Object::SetBoundingBox(AABB &boundingBox)
{
	m_BoundingBox = boundingBox;
}
