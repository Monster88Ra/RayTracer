#include "Camera.h"

Camera::Camera(const Vector3f &eyePosition, const Vector3f &lookAt, const Vector3f &upDirection, float FOV, const Vector2i &outputResolution):
	m_fieldOfView(FOV),
	m_distanceFromScreenPlane((1 / std::tan(((FOV / 180.0f) * 3.14159265f) / 2.0f))),
	m_aspectRatio((float)outputResolution.y/outputResolution.x),
	m_outputResolution(outputResolution),
	m_viewTransform(eyePosition,lookAt,upDirection)
{
	// since we transform ray to world space
	// get the inverse
	m_viewTransform = m_viewTransform.InverseAffine();
}

Camera::~Camera()
{

}

Ray Camera::GenerateRay(int32_t x, int32_t y)const
{
	// caculate U V D
	const float U = -1 + (2 * (x + 0.5f)) / m_outputResolution.x;
	const float V = m_aspectRatio - (2 * m_aspectRatio * (y + 0.5f)) / m_outputResolution.y;

	// direction
	Vector3f rayDirection = Vector3f(-U, V, -m_distanceFromScreenPlane);
	rayDirection.Normalize();

	const Ray pixelRay(Vector3f(), rayDirection);
	
	// transform to world space
	Ray testRay = m_viewTransform.TransformRay(pixelRay);
	return testRay;
	
}

std::vector<Ray> Camera::GenerateSampleRays(int32_t x, int32_t y, uint16_t samplingLevel) const
{
	const float invOutputResX = 1.0f / (m_outputResolution.x * samplingLevel);
	const float invOutputResY = 1.0f / (m_outputResolution.y * samplingLevel);
	const float invHundred = 1.0f / 100.0f; // store for random UV offsets

	// simulating more pixel with UV
	x *= samplingLevel;
	y *= samplingLevel;

	std::vector<Ray> sampleRays;
	for (int i = 0; i < samplingLevel; i++)
	{
		for (int j = 0; j < samplingLevel; j++)
		{
			// get random offset
			const float UOffset = (std::rand() % 100 + 1) * invHundred;
			const float VOffset = (std::rand() % 100 + 1) * invHundred;

			const float U = -1 + (2 * (x + UOffset)) * invOutputResX;
			const float V = m_aspectRatio - (2 * m_aspectRatio * (y + VOffset)) * invOutputResY;

			//compute direction
			Vector3f rayDirection = Vector3f(-U, V, -m_distanceFromScreenPlane);

			Ray pixelRay(Vector3f(), rayDirection.Normalize());
			pixelRay = m_viewTransform.TransformRay(pixelRay);

			sampleRays.push_back(pixelRay);
			x++;
		}
		y++;
		// reset x for new row
		x -= samplingLevel;
	}

	return sampleRays;
}

float Camera::GetFOV() const
{
	return m_fieldOfView;
}

void Camera::SetFOV(float fov)
{
	m_fieldOfView = fov;
	m_distanceFromScreenPlane = (float)(1 / std::tan((fov / 180 * 3.14159265) / 2));
}