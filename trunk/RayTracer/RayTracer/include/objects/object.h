#pragma once

#include "Material.h"
#include "AABB.h"
#include "Matrix4.h"
#include "Ray.h"

class Texture;
struct Intersection;

/*****************************
* Abstract class for all renderable objects.
* Each derived class must construct it's own
* AABB for use with the Kd-tree.
******************************/
class Object
{
public:
	explicit Object(const Material &lightingMaterial = Material());
	virtual ~Object();

	virtual bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr, Intersection *out_Intersection = nullptr) = 0;

	virtual void SetMaterial(const Material &newMaterial);
	// return the default material
	Material GetMaterial() const;
	// Retrieves the material for the Primitive at a point in world space.
	virtual Material GetMaterial(Vector3f surfacePoint) = 0;

	//loacl space
	AABB GetBoundingBox() const;
	// wolrd space
	AABB GetWorldAABB() const;

	void SetParent(Object &parent);

	Matrix4 GetInvTransform() const;
	// Gets the world inverse transform of the object.include parent
	Matrix4 GetWorldInvTransform() const;

	Matrix4 GetTransform() const;
	// Gets transform of the object.include parent
	Matrix4 GetWorldTransform() const;

	void SetTransform(const Matrix4 &trans);
	void Rotate(EAxis axis, float degree);
	void Scale(EAxis Axis, float Scale);
	void SetLocalOrigin(const Vector3f &origin);
	// Get the origin of the objectin respect to it's parent
	Vector3f GetLocalOrigin() const;
	Vector3f GetWorldOrigin() const;

	bool IsEnable() const;
	void SetEnable(bool flag);

protected:
	void SetBoundingBox(AABB &boundingBox);
private:
	virtual void ConstructAABB(Vector3f min = Vector3f(), Vector3f max = Vector3f()) = 0;
protected:
	Material m_Material;	// lighting material
	Object *m_ParentObject;
private:
	AABB m_BoundingBox;
	Matrix4 m_Transform;	//object space transform
	Matrix4 m_InvTransform; 
	bool m_IsEnable;		// if object is disable,no intersections  
};
