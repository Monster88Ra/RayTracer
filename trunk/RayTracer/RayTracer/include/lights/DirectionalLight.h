#pragma once
#include "light.h"
#include "Vector3.h"

class DirectionalLight :public Light
{
public:
	DirectionalLight();
	DirectionalLight(Color lightColor,Vector3f lightDirection);
	Ray GetRayToLight(const Vector3f &surfacePoint) const override;
	std::vector<Ray> GetRayToLightSamples(const Vector3f &surfacepoint, int numSamples) const override;
	Color GetIntesityAtPosition(Vector3f Position) const override;
	float GetDistance(const Vector3f position) const override;

	Vector3f GetLightDirection() const;
	void SetLightDirection(const Vector3f &dir);
private:
	Vector3f m_LightDirection;
};