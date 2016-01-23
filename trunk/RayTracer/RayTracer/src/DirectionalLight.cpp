#include <cstdlib>
#include <limits>

#include "DirectionalLight.h"
#include "Matrix4.h"


DirectionalLight::DirectionalLight():
	Light(),
	m_LightDirection(0,-1,0)
{
}

DirectionalLight::DirectionalLight(Color lightColor, Vector3f lightDirection):
	Light(lightColor),
	m_LightDirection(lightDirection)
{
	m_LightDirection.Normalize();
}

Ray DirectionalLight::GetRayToLight(const Vector3f & surfacePoint) const
{
	return  Ray(surfacePoint, -m_LightDirection);
}

std::vector<Ray> DirectionalLight::GetRayToLightSamples(const Vector3f & surfacepoint, int numSamples) const
{
	// make a new fram which points in the direction of light
	const Vector3f N = m_LightDirection;

	// let the up direction be the smallest component of the light direction
	int shortestDirection = 0;
	for (int i = 1; i < 3; i++)
	{
		if (abs(N[i]) < abs(N[shortestDirection]))
		{
			shortestDirection = i;
		}
	}

	// caculate basis vecor for the frame
	Vector3f upVector;
	upVector[shortestDirection] = 1.0f;

	const Vector3f rightVector(Vector3f::Cross(N, upVector).Normalize());
	upVector = Vector3f::Cross(rightVector, N).Normalize();

	const Matrix4 lightFram(rightVector, upVector, N);
	std::vector<Ray> raySamples;

	// random
	for (int i = 0; i < numSamples; i++)
	{
		const float XBias = std::rand() % 100 / 1000.0f;
		const float ZBias = std::rand() % 100 / 1000.0f;

		Vector3f sampleDirectionBias(XBias, 0.0f, ZBias);
		sampleDirectionBias = lightFram.TransformPosition(sampleDirectionBias);

		Ray sampleRay(surfacepoint, (-m_LightDirection + sampleDirectionBias).Normalize());
		sampleRay.origin += sampleRay.direction * _EPSILON;
		raySamples.push_back(sampleRay);
	}

	return raySamples;
}

Color DirectionalLight::GetIntesityAtPosition(Vector3f Position) const
{
	//turn off compiler warning
	Position;
	// directional light
	return m_LightColor;
}

float DirectionalLight::GetDistance(const Vector3f position) const
{
	//turn off compiler warning
	position;
	return std::numeric_limits<float>::max();
}

Vector3f DirectionalLight::GetLightDirection() const
{
	return m_LightDirection;
}

void DirectionalLight::SetLightDirection(const Vector3f &dir)
{
	m_LightDirection = dir;
}
