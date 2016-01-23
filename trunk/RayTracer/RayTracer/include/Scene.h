#pragma once 

#include "Camera.h"
#include "Image.h"
#include "KDTree.h"
#include "Light.h"
#include "object.h"

#include <vector>
#include <memory>
#include <istream>
#include <string>



class Scene
{
public:
	using PrimitivePtr = std::unique_ptr<Object>;
	using LightPtr = std::unique_ptr<Light>;

	 Scene(const std::string& outputName, const Vector2i& outputResolution, const uint16_t numShadowSamples, const uint16_t superSamplingLevel);
	 // Don't allow copies of a scene
	 Scene& operator=(const Scene& Copy) = delete;
	~ Scene();

	// build scene
	// add the camera lights and geometry
	void BuildScene(std::istream &sceneConfig);
	Color TraceRay(const Ray& cameraRay, int32_t depth);
	void RenderScene();

private:
	Vector3f ComputeBlinnSpecularReflectionHalfVec(const Vector3f& lightDirection, const Vector3f& viewerDirection) const;
	//based on Snell's law.
	Vector3f ComputeRefractionVector(const Vector3f& lightDirection, const Vector3f& surfaceNormal, const float& refractiveIndex) const;
	bool IsInShadow(const Ray &lightRay, float maxDistance);

	float ComputeShadeFactor(const Light& light, const Vector3f& surfacePoint);
private:
	Image m_outputImage; 
	Camera m_camera;
	std::vector<LightPtr> m_lights; 
	Color m_backgroundColor; 
	Color m_globalAmbient; 
	KDTree m_KDTree; 

	uint16_t m_numberOfShadowSamples; 
	uint16_t m_superSamplingLevel; 
	Vector2i m_outputResolution; 
};
