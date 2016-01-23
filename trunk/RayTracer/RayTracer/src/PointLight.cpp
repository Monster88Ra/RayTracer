#include <cstdlib>
#include "Matrix4.h"
#include "PointLight.h"

PointLight::PointLight(Color lightColor, Vector3f lightPosition, float radius, float minRange, float maxRange):
	Light(lightColor),
	m_Position(lightPosition),
	m_Radius(radius),
	m_MaxRange(maxRange),
	m_MinRange(minRange)
{
}

PointLight::~PointLight()
{
}

Ray PointLight::GetRayToLight(const Vector3f & surfacePoint) const
{
	Ray ret (surfacePoint, (m_Position - surfacePoint).Normalize());
	ret.origin += ret.direction * _EPSILON;
	return ret;
}

std::vector<Ray> PointLight::GetRayToLightSamples(const Vector3f & surfacepoint, int numSamples) const
{
	const Vector3f surfaceDirection(-(m_Position-surfacepoint).Normalize());

	int shortestDirection = 0;
	for (int i = 1; i < 3; i++)
	{
		if (abs(surfaceDirection[i]) < abs(surfaceDirection[shortestDirection]))
		{
			shortestDirection = i;
		}
	}

	// caculate basis vecor for the frame
	Vector3f upVector;
	upVector[shortestDirection] = 1.0f;

	const Vector3f rightVector(Vector3f::Cross(surfaceDirection, upVector).Normalize());
	upVector = Vector3f::Cross(rightVector, surfaceDirection).Normalize();

	// create a new frame from these vector
	Matrix4 lightFrame(rightVector, upVector, surfaceDirection);
	lightFrame.SetOrigin(m_Position);

	const float samplePlaneRadius = m_Radius;
	const int numRows = (int)sqrt(numSamples);
	const float gridSize = samplePlaneRadius * 2.0f / numRows;

	// generate rays 
	std::vector<Ray> raySamples;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			const float jitterOffsetZ = ((std::rand() % 9 + 1) / 10.0f) * gridSize;
			const float Z = -i*gridSize + samplePlaneRadius - jitterOffsetZ;

			const float jitterOffsetX = ((std::rand() % 9 + 1) / 10.0f) * gridSize;
			const float X = j * gridSize - samplePlaneRadius + jitterOffsetX;

			Vector3f gridPosition(X, 0, Z);
			gridPosition = lightFrame.TransformPosition(gridPosition);

			Ray sampleRay(surfacepoint, (gridPosition - surfacepoint).Normalize());
			sampleRay.origin += sampleRay.direction * _EPSILON;

			raySamples.push_back(sampleRay);
		}
	}

	return raySamples;
}

Color PointLight::GetIntesityAtPosition(Vector3f position) const
{
	float distance = (position - m_Position).Length();

	if (distance < m_MinRange)
	{
		return m_LightColor;
	}
	else if (distance < m_MaxRange)
	{
		return m_LightColor * ((m_MaxRange - distance) / (m_MaxRange - m_MinRange));
	}
	else
	{
		return Color::Black;
	}
}

float PointLight::GetDistance(const Vector3f position) const
{
	return (m_Position - position).Length();
}

void PointLight::SetLightPosition(const Vector3f & lightPosition)
{
	m_Position = lightPosition;
}

Vector3f PointLight::GetLightPosition() const
{
	return m_Position;
}
