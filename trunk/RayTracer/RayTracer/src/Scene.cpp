#include <iostream>
#include <algorithm>
#include <limits>
#include <unordered_map>

#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Ray.h"
#include "Cube.h"
#include "Mesh.h"
#include "Texture.h"
#include "MathUtiliy.h"


static std::unordered_map<std::string, Texture> TextureHolder;
static std::unordered_map<std::string, Material> MaterialHolder;
static const uint8_t KdDepth = 10;
static const uint8_t KdMinObjects = 3;


void throwSceneConfigError(const std::string& ObjectType)
{
	std::cout << "...Error in scene config file for a " << ObjectType << std::endl;
	throw std::runtime_error("Scene config error.");
}


Scene::Scene(const std::string &outputName, const Vector2i& outputResolution, const uint16_t numShadowSamples, const uint16_t superSamplingLevel):
	m_outputImage(outputName,outputResolution),
	m_backgroundColor(Color::Black),
	m_globalAmbient(0.2f,0.2f,0.2f),
	m_camera(Vector3f(0, 0, 0), Vector3f(0, 0, -1.0f), Vector3f(0, 1, 0), 75, outputResolution),
	m_lights(),
	m_KDTree(),
	m_numberOfShadowSamples(numShadowSamples),
	m_superSamplingLevel(superSamplingLevel),
	m_outputResolution(outputResolution)
{

}

Scene::~Scene()
{

}

void Scene::BuildScene(std::istream& in)
{
	std::vector<PrimitivePtr> Objects;

	std::string string;
	in >> string;

	while (in.good())
	{
		if (string == "Background:")
			in >> m_backgroundColor.m_R >> m_backgroundColor.m_G >> m_backgroundColor.m_B;
		else if (string == "GlobalAmbient:")
			in >> m_globalAmbient.m_R >> m_globalAmbient.m_G >> m_globalAmbient.m_B;
		else if (string == "Camera")
		{
			Vector3f Position, LookAt, Up;
			float FOV;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Camera");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "LookAt:")
				throwSceneConfigError("Camera");
			in >> LookAt.x >> LookAt.y >> LookAt.z;

			in >> string;
			if (string != "UpDirection:")
				throwSceneConfigError("Camera");
			in >> Up.x >> Up.y >> Up.z;

			in >> string;
			if (string != "FOV:")
				throwSceneConfigError("Camera");
			in >> FOV;

			m_camera = Camera(Position, LookAt, Up, FOV, m_outputResolution);
		}
		else if (string == "DirectionalLight")
		{
			Color Color;
			Vector3f Direction;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("DirectionalLight");
			in >> Color.m_R >> Color.m_G >> Color.m_B;

			in >> string;
			if (string != "Direction:")
				throwSceneConfigError("DirectionalLight");
			in >> Direction.x >> Direction.y >> Direction.z;

			m_lights.push_back(LightPtr(new DirectionalLight(Color, Direction)));
		}
		else if (string == "PointLight")
		{
			Color Color;
			Vector3f Position;
			float SurfaceRadius, MinFalloff, MaxFalloff;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("PointLight");
			in >> Color.m_R >> Color.m_G >> Color.m_B;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("PointLight");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "SurfaceRadius:")
				throwSceneConfigError("PointLight");
			in >> SurfaceRadius;

			in >> string;
			if (string != "MinFalloff:")
				throwSceneConfigError("PointLight");
			in >> MinFalloff;

			in >> string;
			if (string != "MaxFalloff:")
				throwSceneConfigError("PointLight");
			in >> MaxFalloff;

			m_lights.push_back(LightPtr(new PointLight(Color, Position, SurfaceRadius, MinFalloff, MaxFalloff)));
		}
		else if (string == "Plane")
		{
			Vector3f Normal;
			Vector3f Point;
			std::string Material;

			in >> string;
			if (string != "Normal:")
				throwSceneConfigError("Plane");
			in >> Normal.x >> Normal.y >> Normal.z;

			in >> string;
			if (string != "PointOnPlane:")
				throwSceneConfigError("Plane");
			in >> Point.x >> Point.y >> Point.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Plane");
			in >> Material;

			Objects.push_back(PrimitivePtr(new Plane(MaterialHolder[Material], Normal, Point)));
		}
		else if (string == "Sphere")
		{
			Vector3f Position, Rotation;
			float Radius;
			std::string Material;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Sphere");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "Radius:")
				throwSceneConfigError("Sphere");
			in >> Radius;

			in >> string;
			if (string != "Rotation:")
				throwSceneConfigError("Cube");
			in >> Rotation.x >> Rotation.y >> Rotation.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Sphere");
			in >> Material;

			Objects.push_back(PrimitivePtr(new Sphere(Position, Radius, MaterialHolder[Material])));
			Matrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			Objects.back()->SetTransform(Transform);
		}
		else if (string == "Triangle")
		{
			Vector3f V0, V1, V2;
			std::string Material;

			in >> string;
			if (string != "V0:")
				throwSceneConfigError("Triangle");
			in >> V0.x >> V0.y >> V0.z;

			in >> string;
			if (string != "V1:")
				throwSceneConfigError("Triangle");
			in >> V1.x >> V1.y >> V1.z;

			in >> string;
			if (string != "V2:")
				throwSceneConfigError("Triangle");
			in >> V2.x >> V2.y >> V2.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Triangle");
			in >> Material;

			Objects.push_back(PrimitivePtr(new Triangle(V0, V1, V2, MaterialHolder[Material])));
		}
		else if (string == "Cube")
		{
			Vector3f Position, Rotation, Scale;
			std::string Material;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Cube");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "Rotation:")
				throwSceneConfigError("Cube");
			in >> Rotation.x >> Rotation.y >> Rotation.z;

			in >> string;
			if (string != "Scale:")
				throwSceneConfigError("Cube");
			in >> Scale.x >> Scale.y >> Scale.z;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Cube");
			in >> Material;

			Objects.push_back(PrimitivePtr(new Cube(Position, MaterialHolder[Material])));
			Matrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			Transform.Scale(Scale);
			Objects.back()->SetTransform(Transform);
		}
		else if (string == "Model")
		{
			std::string Filename, Material;
			Vector3f Position, Rotation, Scale;

			in >> string;
			if (string != "ModelFile:")
				throwSceneConfigError("Model");
			in >> Filename;

			in >> string;
			if (string != "Material:")
				throwSceneConfigError("Model");
			in >> Material;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Model");
			in >> Position.x >> Position.y >> Position.z;

			in >> string;
			if (string != "Rotation:")
				throwSceneConfigError("Model");
			in >> Rotation.x >> Rotation.y >> Rotation.z;

			in >> string;
			if (string != "Scale:")
				throwSceneConfigError("Model");
			in >> Scale.x >> Scale.y >> Scale.z;

			Objects.push_back(PrimitivePtr(new Mesh(Filename, MaterialHolder[Material])));
			Matrix4 Transform;
			Transform.SetOrigin(Position);
			Transform.Rotate(Rotation);
			Transform.Scale(Scale);
			Objects.back()->SetTransform(Transform);
		}
		else if (string == "Texture")
		{
			std::string Name, File;

			in >> string;
			if (string != "Name:")
				throwSceneConfigError("Texture");
			in >> Name;

			in >> string;
			if (string != "File:")
				throwSceneConfigError("Texture");
			in >> File;

			TextureHolder.insert({ Name, Texture(File) });
		}
		else if (string == "Material")
		{
			std::string Name;
			Color Specular, Diffuse, Ambient;
			float Glossiness, Reflectivity, RefractiveIndex;

			std::string DiffuseTextureName;
			Vector2f UVScale;

			in >> string;
			if (string != "Name:")
				throwSceneConfigError("Material");
			in >> Name;

			in >> string;
			if (string != "Specular:")
				throwSceneConfigError("Material");
			in >> Specular.m_R >> Specular.m_G >> Specular.m_B >> Specular.m_A;

			in >> string;
			if (string != "Diffuse:")
				throwSceneConfigError("Material");
			in >> Diffuse.m_R >> Diffuse.m_G >> Diffuse.m_B >> Diffuse.m_A;

			in >> string;
			if (string != "Ambient:")
				throwSceneConfigError("Material");
			in >> Ambient.m_R >> Ambient.m_G >> Ambient.m_B >> Ambient.m_A;

			in >> string;
			if (string != "Glossiness:")
				throwSceneConfigError("Material");
			in >> Glossiness;

			in >> string;
			if (string != "Reflectivity:")
				throwSceneConfigError("Material");
			in >> Reflectivity;

			in >> string;
			if (string != "RefractiveIndex:")
				throwSceneConfigError("Material");
			in >> RefractiveIndex;

			in >> string;
			if (string != "DiffuseTextureName:")
				throwSceneConfigError("Material");
			in >> DiffuseTextureName;

			in >> string;
			if (string != "UVScale:")
				throwSceneConfigError("Material");
			in >> UVScale.x >> UVScale.y;

			Material Material(Specular, Diffuse, Ambient, Glossiness, Reflectivity, RefractiveIndex);

			// if a texture was given, assign it to the material
			if (TextureHolder.find(DiffuseTextureName) != TextureHolder.end())
			{
				Material.SetDiffuseTexture(TextureInfo{ &TextureHolder.at(DiffuseTextureName), UVScale.x, UVScale.y });
			}

			MaterialHolder.insert({ Name, Material });
		}
		in >> string;
	}

	m_KDTree.BuildTree(Objects, KdDepth, KdMinObjects);
}

Color Scene::TraceRay(const Ray& cameraRay, int32_t depth)
{
	if (depth < 1)
	{
		return m_backgroundColor;
	}

	float maxValueT(std::numeric_limits<float>::max());
	Intersection closestIntersection;

	m_KDTree.IsIntersectingRay(cameraRay, &maxValueT, &closestIntersection);

	// if an object was intersected
	if (closestIntersection.object)
	{
		// disable the current object so reflection and refraction rays don't interact with it
		closestIntersection.object->SetEnable(false);
		Color outputColor;

		// get surface material point normal
		const Vector3f &surfacePoint(closestIntersection.point);
		const Vector3f &surfaceNormal(closestIntersection.normal.Normalize());
		const Material &surfaceMaterial(closestIntersection.object->GetMaterial(surfacePoint));

		//assert(abs(SurfaceNormal.Length() - 1.0f) < _EPSILON);

		for (const auto &light : m_lights)
		{
			Color lightColor = light->GetIntesityAtPosition(surfacePoint);

			if (lightColor == Color::Black)
			{
				continue;
			}

			// get the direction of light and computer reflection
			Ray rayToLight(light->GetRayToLight(surfacePoint));
			rayToLight.origin += rayToLight.direction * _EPSILON;

			const Vector3f &lightDirection(rayToLight.direction);
			const Vector3f &H = ComputeBlinnSpecularReflectionHalfVec(rayToLight.direction, -cameraRay.direction);

			//if an object in the way of light,skip current
			if (m_numberOfShadowSamples > 1)
			{
				const float shaderFactor = ComputeShadeFactor(*light, surfacePoint);
				if (shaderFactor <= 0)
				{
					continue;
				}
				lightColor *= shaderFactor;
			}
			else if (IsInShadow(rayToLight, light->GetDistance(surfacePoint)))
			{
				continue;
			}

			// computer specular factor
			const float specularFactor = pow(std::max(Vector3f::Dot(surfaceNormal, H), 0.0f), surfaceMaterial.GetGlossiness());
			// computer diffuse factor
			const float diffuseFactor = std::max(Vector3f::Dot(surfaceNormal, lightDirection), 0.f);
			// combine material color & light color
			const Color specularColor(lightColor * surfaceMaterial.GetSpecular() * specularFactor);
			const Color diffuseColor(lightColor  * surfaceMaterial.GetDiffuse() * diffuseFactor);
			// Add diffuse and specular contributions to total
			outputColor += specularColor + diffuseColor;

			if (surfaceMaterial.GetDiffuse().m_A < 1.0f)
			{
				outputColor *= surfaceMaterial.GetDiffuse().m_A;
				const Vector3f refractionDirection = ComputeRefractionVector(-cameraRay.direction, surfaceNormal, surfaceMaterial.GetRefractiveIndex());
				//assert(abs(RefractionDirection.Length() - 1) < _EPSILON);
				const Ray refraction(surfacePoint, refractionDirection);

				outputColor += (1 - surfaceMaterial.GetDiffuse().m_A) * TraceRay(refraction, depth - 1);
			}
			//Add mirror reflection contribution
			const Vector3f mirrorReflection = -cameraRay.direction.Reflect(surfaceNormal);
			const Ray reflectionRay(surfacePoint, mirrorReflection);
			outputColor += TraceRay(reflectionRay, depth - 1) * outputColor * surfaceMaterial.GetReflectivity();
		}

		closestIntersection.object->SetEnable(true);

		return outputColor + (m_globalAmbient * surfaceMaterial.GetAmbient());
	}
	else
	{
		return m_backgroundColor;
	}
}

void Scene::RenderScene()
{
	const float invTotalPixels = 100.0f / (m_outputResolution.x * m_outputResolution.y);
	const int percentUpdateRate = m_outputResolution.y / 20;
	int updateInterval = 0;

	if (m_superSamplingLevel > 1)
	{
		for (int y = 0; y < m_outputResolution.y; y++)
		{
			for (int x = 0; x < m_outputResolution.x; x++)
			{
				Color pixelColor;
				for (const Ray& PixelRay : m_camera.GenerateSampleRays(x, y, m_superSamplingLevel))
				{
					pixelColor += TraceRay(PixelRay, 4);
				}

				pixelColor /= (float)(m_superSamplingLevel * m_superSamplingLevel);
				m_outputImage.SetPixel(x, y, pixelColor.Clamp());
			}
			updateInterval++;
			if (updateInterval >= percentUpdateRate)
			{
				updateInterval = 0;
				std::cout << (y * m_outputResolution.x) * invTotalPixels << "% Complete" << std::endl;
			}
		}

	}
	else
	{
		for (int y = 0; y < m_outputResolution.y; y++)
		{
			for (int x = 0; x < m_outputResolution.x; x++)
			{
				Color piexelColor;
				const Ray& pixelRay = m_camera.GenerateRay(x, y);
				piexelColor = TraceRay(pixelRay, 4);
				m_outputImage.SetPixel(x, y, piexelColor.Clamp());
			}

			updateInterval++;
			if (updateInterval >= percentUpdateRate)
			{
				updateInterval = 0;
				std::cout << (y * m_outputResolution.x) * invTotalPixels << "% Complete" << std::endl;
			}
		}
	}
	m_outputImage.WriteImage();
}

Vector3f Scene::ComputeBlinnSpecularReflectionHalfVec(const Vector3f& lightDirection, const Vector3f& viewerDirection) const
{
	Vector3f halfVec(lightDirection + viewerDirection);
	halfVec.Normalize();
	return halfVec;
}

Vector3f Scene::ComputeRefractionVector(const Vector3f& lightDirection, const Vector3f& surfaceNormal, const float& refractiveIndex) const
{
	const float& InvRefractive = 1.0f / refractiveIndex;
	const float& NDotL = Vector3f::Dot(surfaceNormal, lightDirection);

	return ((InvRefractive * NDotL - std::sqrtf(1 - InvRefractive * InvRefractive * (1 - (NDotL * NDotL)))) * surfaceNormal - InvRefractive * lightDirection).Normalize();
}

bool Scene::IsInShadow(const Ray &lightRay, float maxDistance)
{
	return m_KDTree.IsIntersectingRay(lightRay, &maxDistance);
}

float Scene::ComputeShadeFactor(const Light& light, const Vector3f& surfacePoint)
{
	const float factorSize = 1.0f / m_numberOfShadowSamples;
	float shaderFactor = 1.0f;
	const float maxTValue = light.GetDistance(surfacePoint);
	for (Ray shadowSample : light.GetRayToLightSamples(surfacePoint, m_numberOfShadowSamples))
	{
		shadowSample.origin += shadowSample.direction * _EPSILON;
		float tValue = maxTValue;
		if (m_KDTree.IsIntersectingRay(shadowSample, &tValue))
		{
			shaderFactor -= factorSize;
		}
	}
	return shaderFactor;
}