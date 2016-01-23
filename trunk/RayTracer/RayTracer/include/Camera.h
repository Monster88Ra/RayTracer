#pragma once

#include <cstdint>
#include <vector>

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Ray.h"

class Camera
{
public:
	Camera(const Vector3f &eyePosition,const Vector3f &lookAt,const Vector3f &upDirection,float FOV,const Vector2i &outputResolution);
	~Camera();

	Ray GenerateRay(int32_t x, int32_t y) const;
	// super AA
	std::vector<Ray> GenerateSampleRays(int32_t x, int32_t y, uint16_t samplingLevel) const;
	// get the horizontal FOV
	float GetFOV() const;
	// set the horizontal FOV
	void SetFOV(float fov);

private:
	float m_fieldOfView;//horizontal fov
	float m_aspectRatio;//height/width
	float m_distanceFromScreenPlane;//distance between viewpoint and screen
	Vector2i m_outputResolution;//in pixel
	LookAtMatrix m_viewTransform;
};

