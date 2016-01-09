#pragma
#include "object.h"
// Construct a renderable 3d cube
class Cube:public Object
{
public:
	Cube(Vector3f center,Material &lightingMaterial);
	~Cube();

	bool IsIntersectingRay(Ray ray, float *out_ValueT = nullptr, Intersection *out_Intersection = nullptr) override;

	Material GetMaterial(Vector3f surfacePoint) override;
private:
	// construct intersection info
	void ConstructIntersection(const Vector3f& intersectionPoint, Intersection* ntersectionOut);
	void ConstructAABB(Vector3f min = Vector3f(), Vector3f max = Vector3f()) override;
};

