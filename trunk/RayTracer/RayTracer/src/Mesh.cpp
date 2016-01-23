#include <iostream>
#include <fstream>
#include <limits>

#include "Mesh.h"
#include "Intersection.h"

#define BVH_MIN_OBJECTS 5

Mesh::Mesh(const Material &material ):
	Object(material),
	m_BVHRoot()
{

}

Mesh::Mesh(const std::string &modelFilePath, const Material &material ):
	Object(material),
	m_BVHRoot()
{
	ReadModel(modelFilePath);
	ConstructBVH(m_BVHRoot, 8);

}

Mesh::~Mesh()
{

}

bool Mesh::IsIntersectingRay(Ray ray, float *out_ValueT , Intersection *out_Intersection )
{
	if (!IsEnable())
		return false;

	// bring ray into object space for intersection tests
	ray = GetWorldInvTransform().TransformRay(ray);

	const bool Flag = TraverseBVHAgainstRay(m_BVHRoot, ray, out_ValueT, out_Intersection);

	if (Flag && out_ValueT && out_Intersection)
	{
		const Matrix4& WorldTransform = GetWorldTransform();
		out_Intersection->point = WorldTransform.TransformPosition(out_Intersection->point);
		out_Intersection->normal = WorldTransform.TransformDirection(out_Intersection->normal);
	}

	return Flag;
}

Material Mesh::GetMaterial(Vector3f surfacePoint)
{
	// turn off compiler warning
	surfacePoint;
	return m_Material;
}

void Mesh::ConstructAABB(Vector3f min , Vector3f max )
{
	SetBoundingBox(AABB(min, max));
}

void Mesh::ReadModel(const std::string &ModelFilepath)
{
	// unsolved
	// bounds for bounding box
	Vector3f MinBounds;
	Vector3f MaxBounds;

	std::ifstream ModelFile(ModelFilepath.c_str());

	if (ModelFile.fail())
	{
		std::cout << "Failed to open FMesh model filename: "
			<< ModelFilepath;
		return;
	}

	std::string FileLine;
	std::vector<Vector3f> Vertices;
	std::vector<Vector2f> UVs;
	std::vector<std::unique_ptr<Triangle>> Triangles;

	while (getline(ModelFile, FileLine))
	{
		// line contains a vertex
		if (FileLine[0] == 'v' && FileLine[1] == ' ')
		{
			FileLine = FileLine.substr(2);

			const std::string XString = FileLine.substr(0, FileLine.find(' '));
			const float X = (float)atof(XString.c_str());

			FileLine = FileLine.substr(XString.length() + 1);
			const std::string YString = FileLine.substr(0, FileLine.find(' '));
			const float Y = (float)atof(YString.c_str());

			FileLine = FileLine.substr(YString.length() + 1);
			const std::string ZString = FileLine.substr(0, FileLine.find(' '));
			const float Z = (float)atof(ZString.c_str());

			const Vector3f Vertex(X, Y, Z);
			UpdateBounds(MinBounds, MaxBounds, Vertex);

			Vertices.push_back(Vertex);

		}
		// line contains a face
		else if (FileLine[0] == 'f' && FileLine[1] == ' ')
		{
			FileLine = FileLine.substr(2);

			Vector3f FaceVerts[3];

			bool HasUVs = false;
			Vector2f FaceUVs[3];

			// get attributes for each vertex
			for (int i = 0; i < 3; i++)
			{
				std::string DataString = FileLine.substr(0, FileLine.find(' '));
				std::string VertexString = DataString.substr(0, DataString.find('/'));
				FaceVerts[i] = Vertices[atoi(VertexString.c_str()) - 1];

				if (HasUVs || VertexString.length() != DataString.length())
				{
					HasUVs = true;
					std::string UVString = DataString.substr(VertexString.length() + 1);
					UVString = UVString.substr(0, UVString.find('/'));
					FaceUVs[i] = UVs[atoi(UVString.c_str()) - 1];
				}

				if (i < 2)
					FileLine = FileLine.substr(DataString.length() + 1);
			}

			// .obj vertex order is clockwise, we use counterclockwise
			std::unique_ptr<Triangle> Triangle(new Triangle(FaceVerts[2], FaceVerts[1], FaceVerts[0], m_Material));
			Triangle->SetParent(*this);

			if (HasUVs)
				Triangle->SetUVCoordinate(FaceUVs[2], FaceUVs[1], FaceUVs[0]);

			Triangles.push_back(std::move(Triangle));
		}
		// line contains a UV
		else if (FileLine[0] == 'v' && FileLine[1] == 't' && FileLine[2] == ' ')
		{
			FileLine = FileLine.substr(3);

			const std::string UString = FileLine.substr(0, FileLine.find(' '));
			const std::string VString = FileLine.substr(UString.length() + 1, FileLine.find(' '));
			UVs.push_back(Vector2f((float)atof(UString.c_str()), (float)atof(VString.c_str())));
		}
	}

	ModelFile.close();

	ConstructAABB(MinBounds, MaxBounds);

	// setup root of BVH
	m_BVHRoot.boundingVolume = GetBoundingBox();
	m_BVHRoot.objects = std::move(Triangles);
}

void Mesh::ConstructBVH(BVHNode &node, const uint8_t depth)
{
	if (depth <= 0 || node.objects.size() <= BVH_MIN_OBJECTS)
	{
		return;
	}

	// find the longest axis
	uint8_t splitAxis = 0;
	const Vector3f &currentDemisions(node.boundingVolume.GetDeminsions());
	for (uint8_t i = 1; i < 3; i++)
	{
		if (currentDemisions[i] > currentDemisions[splitAxis])
		{
			splitAxis = i;
		}
	}
	
	// sort the objects
	std::sort(node.objects.begin(), node.objects.end(), [&splitAxis](const std::unique_ptr<Triangle> &lhs, const std::unique_ptr<Triangle> &rhs)
	{
		return lhs->GetBoundingBox().GetCenter()[splitAxis] < rhs->GetBoundingBox().GetCenter()[splitAxis];
	});

	// split at median
	const uint32_t splitIndex = node.objects.size() / 2;

	// construct the child node
	node.child[0] = std::unique_ptr<BVHNode>(new BVHNode());
	node.child[1] = std::unique_ptr<BVHNode>(new BVHNode());

	for (size_t i = 0; i < splitIndex; i++)
	{
		node.child[0]->objects.push_back(std::move(node.objects[i]));
	}

	for (size_t i = splitIndex; i < node.objects.size(); i++)
	{
		node.child[1]->objects.push_back(std::move(node.objects[i]));
	}

	node.objects.clear();
	node.objects.resize(0);

	ConstructBoundingVolume(*node.child[0]);
	ConstructBoundingVolume(*node.child[1]);

	ConstructBVH(*node.child[0], depth - 1);
	ConstructBVH(*node.child[1], depth - 1);
}

void Mesh::ConstructBoundingVolume(BVHNode &node)
{
	Vector3f max(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
	Vector3f min(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

	// set new AABB for every triangles
	for (const auto &triangle : node.objects)
	{
		const AABB &currentAABB = triangle->GetBoundingBox();
		UpdateBounds(min, max, currentAABB.min);
		UpdateBounds(min, max, currentAABB.max);
	}

	node.boundingVolume.min = min;
	node.boundingVolume.max = max;

}

bool Mesh::TraverseBVHAgainstRay(BVHNode& node, Ray ray, float* out_ValueT , Intersection* IntersectionOut )
{
	float tempValueT = (out_ValueT) ? *out_ValueT : std::numeric_limits<float>::max();
	if (!node.boundingVolume.IsIntersectingRay(ray, &tempValueT))
	{
		return false;
	}
	// leaves
	if (!node.objects.empty())
	{
		bool isIntersecting = false;
		for (const auto &object : node.objects)
		{
			isIntersecting |= object->IsIntersectingRay(ray, out_ValueT, IntersectionOut);
		}
		return isIntersecting;
	}

	bool isIntersecting = false;
	// check two children
	isIntersecting |= TraverseBVHAgainstRay(*node.child[0], ray, out_ValueT, IntersectionOut);
	isIntersecting |= TraverseBVHAgainstRay(*node.child[1], ray, out_ValueT, IntersectionOut);
	return isIntersecting;
}