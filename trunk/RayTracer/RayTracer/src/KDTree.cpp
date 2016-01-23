#include <algorithm>
#include "KDTree.h"
#include "Intersection.h"

KDTree::KDTree():
	m_Root()
{

}

void KDTree::BuildTree(std::vector<std::unique_ptr<Object> >&objects, uint32_t depth, uint32_t minObjectsPerNode)
{
	m_Root.ObjectList = std::move(objects);
	m_Root.Axis = 0;
	
	BuildTreeHelper(m_Root, depth, minObjectsPerNode);
}

void KDTree::BuildTreeHelper(KDNode& currentNode, uint32_t depth, uint32_t minObjectsPerNode)
{
	if (depth == 0 || currentNode.ObjectList.size() <= minObjectsPerNode)
	{
		return;
	}

	const uint32_t dividingAxis = currentNode.Axis;
	auto &currentObjects = currentNode.ObjectList;

	const std::size_t objectListSize = currentNode.ObjectList.size();
	const std::size_t dividingObjectIndex = objectListSize / 2;

	// sort by AABB center
	std::partial_sort(currentObjects.begin(), currentObjects.begin() + dividingObjectIndex, currentObjects.end(),
		[dividingAxis](const std::unique_ptr<Object>& lhs, const std::unique_ptr<Object>& rhs) -> bool
	{
		return lhs->GetWorldAABB().GetCenter()[dividingAxis] < rhs->GetWorldAABB().GetCenter()[dividingAxis];
	});

	// get diving object
	auto &divingObject = currentObjects[dividingObjectIndex];
	AABB divingAABB = divingObject->GetWorldAABB();

	// offset the dividing axis value to just after the median objects AABB
	const float dividingAxisValue = divingAABB.GetCenter()[dividingAxis] + divingAABB.GetDeminsions()[dividingAxis] + 0.1f;
	currentNode.splitValue = dividingAxisValue;

	std::vector<std::unique_ptr<Object>> straddlingObjects;
	currentNode.Child[0] = std::unique_ptr<KDNode>(new KDNode());
	currentNode.Child[1] = std::unique_ptr<KDNode>(new KDNode());

	for (int i = dividingObjectIndex; i >= 0; i--)
	{
		const AABB &currenAABBBox = currentObjects[i]->GetWorldAABB();
		const float axisRange = currenAABBBox.GetDeminsions()[dividingAxis];
		const float dividedRange = std::abs(currenAABBBox.min[dividingAxis] - dividingAxisValue);
		if (axisRange > dividedRange)
		{
			straddlingObjects.push_back(std::move(currentObjects[i]));
		}
		else
		{
			currentNode.Child[0]->ObjectList.push_back(std::move(currentObjects[i]));
		}
	}

	for (size_t i = dividingObjectIndex + 1; i < objectListSize; i++)
	{
		const AABB &currenAABBBox = currentObjects[i]->GetWorldAABB();
		const float axisRange = currenAABBBox.GetDeminsions()[dividingAxis];
		const float dividedRange = currenAABBBox.max[dividingAxis] - dividingAxisValue;
		if (axisRange > dividedRange)
		{
			straddlingObjects.push_back(std::move(currentObjects[i]));
		}
		else
		{
			currentNode.Child[1]->ObjectList.push_back(std::move(currentObjects[i]));
		}
	}
	currentNode.Child[0]->ObjectList.shrink_to_fit();
	currentNode.Child[1]->ObjectList.shrink_to_fit();

	// increment axis for children
	currentNode.Child[0]->Axis = currentNode.Child[1]->Axis = (dividingAxis + 1) % 3;

	currentNode.ObjectList.clear();
	currentNode.ObjectList = std::move(straddlingObjects);

	BuildTreeHelper(*currentNode.Child[0], depth - 1, minObjectsPerNode);
	BuildTreeHelper(*currentNode.Child[1], depth - 1, minObjectsPerNode);
}

bool KDTree::IsIntersectingRay(Ray ray, float *out_ValueT , Intersection *intersectionOut )
{
	return VisitNodesAgainstRay(&m_Root, ray, out_ValueT, intersectionOut);
}

bool KDTree::VisitNodesAgainstRay(KDNode* currentNode, Ray ray, float *out_ValueT , Intersection *intersectionOut )
{
	if (currentNode == nullptr)
	{
		return false;
	}

	bool IsIntersecting = false;

	for (const auto &primitive : currentNode->ObjectList)
	{
		IsIntersecting |= primitive->IsIntersectingRay(ray, out_ValueT, intersectionOut);
	}

	// if intersectionOut is null,just return true
	if (!intersectionOut && IsIntersecting)
	{
		return true;
	}

	// check which child hit first from axis split
	uint32_t axis = currentNode->Axis;
	uint32_t firstChild = ray.origin[axis] > currentNode->splitValue;

	if (abs(ray.direction[axis]) < _EPSILON)
	{
		// ray is parallel to the plane
		IsIntersecting |= VisitNodesAgainstRay(currentNode->Child[firstChild].get(), ray, out_ValueT, intersectionOut);
	}
	else
	{
		// t is the value of intersection of ray with slit plane
		float t = (currentNode->splitValue - ray.origin[axis]) / ray.direction[axis];

		// Check if ray straddles the splitting plane
		// If no tValueout is given, make one
		float maxTValue = (out_ValueT) ? *out_ValueT : std::numeric_limits<float>::max();

		if (t >= 0.0f && t < maxTValue)
		{
			// first check near next check far
			IsIntersecting |= VisitNodesAgainstRay(currentNode->Child[firstChild].get(), ray, out_ValueT, intersectionOut);
			IsIntersecting |= VisitNodesAgainstRay(currentNode->Child[firstChild ^ 1].get(), ray, out_ValueT, intersectionOut);
		}
		else
		{
			IsIntersecting |= VisitNodesAgainstRay(currentNode->Child[firstChild].get(), ray, out_ValueT, intersectionOut);
		}
	}

	return IsIntersecting;
}