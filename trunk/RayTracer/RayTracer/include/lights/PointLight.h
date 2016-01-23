#pragma once
#include "light.h"

class PointLight:public Light
{
public:
	PointLight(Color lightColor, Vector3f lightPosition,float radius,float minRange,float maxRange);
	~PointLight();

	Ray GetRayToLight(const Vector3f &surfacePoint) const override;
	std::vector<Ray> GetRayToLightSamples(const Vector3f &surfacepoint, int numSamples) const override;
	Color GetIntesityAtPosition(Vector3f position) const override;
	float GetDistance(const Vector3f position) const override;

	void SetLightPosition(const Vector3f &lightPosition);
	Vector3f GetLightPosition() const;

private:
	Vector3f m_Position;
	float m_Radius;

	/* Falloff for linear light attenuation */
	float m_MinRange;
	float m_MaxRange;
};

