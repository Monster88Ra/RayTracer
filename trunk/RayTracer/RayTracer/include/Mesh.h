#pragma once 
#include <vector>
#include <memory>
#include <string>

#include "object.h"
#include "Triangle.h"
#include "Texture.h"

class Mesh:public Object
{
public:
	Mesh(const Material &material = Material());
	Mesh(const std::string &modelFilePath, const Material &material = Material());
	~Mesh();

	bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr, Intersection *out_Intersection = nullptr) override;
	Material GetMaterial(Vector3f surfacePoint) override;
private:
	struct BVHNode
	{
		AABB boundingVolume;
		std::unique_ptr<BVHNode> child[2];
		std::vector<std::unique_ptr<Triangle> > objects;
	};

	void ConstructAABB(Vector3f min = Vector3f(), Vector3f max = Vector3f()) override;
	void ReadModel(const std::string &modelFilePath);
	void ConstructBVH(BVHNode &node, const uint8_t depth);
	void ConstructBoundingVolume(BVHNode &node);
	bool TraverseBVHAgainstRay(BVHNode& node, Ray ray, float* out_ValueT = nullptr, Intersection* IntersectionOut = nullptr);
private:
	BVHNode m_BVHRoot;
};

