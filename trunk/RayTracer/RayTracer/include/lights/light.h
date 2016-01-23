#pragma once
#include <vector>
#include"Color.h"
#include "Vector2.h"
#include "Ray.h"

class Light
{
public:
	Light();
	Light(Color lightColor);
	virtual ~Light();

	// chech in shadow
	virtual Ray GetRayToLight(const Vector3f &surfacePoint) const = 0;

	// softshadow sample points
	virtual std::vector<Ray> GetRayToLightSamples(const Vector3f &surfacepoint, int numSamples) const = 0;

	virtual Color GetIntesityAtPosition(Vector3f Position) const = 0;

	virtual float GetDistance(const Vector3f position) const = 0;

	void SetlightColor(const Color &lightColor);

protected:
	Color m_LightColor;
};

