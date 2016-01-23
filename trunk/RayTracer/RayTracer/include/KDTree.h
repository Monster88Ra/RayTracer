#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "object.h"

struct KDNode
{
	// 0 means near, 1 means far
	std::unique_ptr<KDNode> Child[2];
	// value on the splitting axis
	float splitValue = 0.0f;
	// list of primitives
	std::vector<std::unique_ptr<Object>>ObjectList;
	// x, y, or z splitting axis
	uint8_t Axis = 0; 
};

class KDTree
{
public:
	KDTree();
	void BuildTree(std::vector<std::unique_ptr<Object> >&objects, uint32_t depth, uint32_t minObjectsPerNode);
	bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr, Intersection *intersectionOut = nullptr);
private:
	void BuildTreeHelper(KDNode& currentNode, uint32_t depth, uint32_t minObjectsPerNode);
	bool VisitNodesAgainstRay(KDNode* currentNode, Ray ray, float *out_ValueT = nullptr, Intersection *intersectionOut = nullptr);
private:
	KDNode m_Root;
};