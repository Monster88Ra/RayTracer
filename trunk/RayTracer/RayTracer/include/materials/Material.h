#pragma once

#include "Color.h"
#include "Texture.h"
#include <cassert>

struct TextureInfo
{
	Texture *texture;
	float UAxisScale;
	float VAxisScale;

	TextureInfo():
		texture(NULL),
		UAxisScale(0.0f),
		VAxisScale(0.0f)
	{

	}
};

class Material
{
public:
	Material(	Color specular = Color(0.0f, 0.0f, 0.0f),
				Color diffuse = Color(0.7f, 0.7f, 0.7f),
				Color ambient = Color(0.1f, 0.1f, 0.1f),
				float glossiness = 0.0f,
				float reflectivity = 0.0f,
				float refractiveIndex = 1.0f);
		

	~Material();

	void SetAmbient(const Color &ambient);
	Color GetAmbient() const;
	void SetSpecular(const Color &specular);
	Color GetSpecular() const;
	void SetDiffuse(const Color &diffuse);
	Color GetDiffuse() const;

	void SetGlossiness(const float glossiness);
	float GetGlossiness() const;
	void SetReflectivity(const float reflectivity);
	float GetReflectivity() const;
	void SetRefractiveIndex(const float refraction);
	float GetRefractiveIndex() const;

	void SetDiffuseTexture(const TextureInfo &diffuseInfo);
	TextureInfo GetDiffuseTexture() const;
private:
	Color m_SpecularColor;
	Color m_DiffuseColor;
	Color m_AmbientColor;
	float m_Glossiness;
	float m_Reflectivity;
	float m_RefractiveIndex;
	TextureInfo m_DiffuseTextureInfo;
};


inline Material::Material(Color specular, Color diffuse, Color ambient, float glossiness, float reflectivity, float refractiveIndex):
	m_SpecularColor(specular),
	m_DiffuseColor(diffuse),
	m_AmbientColor(ambient),
	m_Glossiness(glossiness),
	m_Reflectivity(reflectivity),
	m_RefractiveIndex(refractiveIndex),
	m_DiffuseTextureInfo()
{
	// Reflectivity must be between 0 and 1
	assert(reflectivity <= 1 && reflectivity >= 0);
}

Material::~Material()
{
}

inline void Material::SetAmbient(const Color & ambient)
{
	m_AmbientColor = ambient;
}

inline Color Material::GetAmbient() const
{
	return m_AmbientColor;
}

inline void Material::SetSpecular(const Color &specular)
{
	m_SpecularColor = specular;
}

inline Color Material::GetSpecular() const
{
	return m_SpecularColor;
}

inline void Material::SetDiffuse(const Color & diffuse)
{
	m_DiffuseColor = diffuse;
}

inline Color Material::GetDiffuse() const
{
	return m_DiffuseColor;
}

inline void Material::SetGlossiness(const float glossiness)
{
	m_Glossiness = glossiness;
}

inline float Material::GetGlossiness() const
{
	return m_Glossiness;
}

inline void Material::SetReflectivity(const float reflectivity)
{
	m_Reflectivity = reflectivity;
}

inline float Material::GetReflectivity() const
{
	return m_Reflectivity;
}

inline void Material::SetRefractiveIndex(const float refraction)
{
	m_RefractiveIndex = refraction;
}

inline float Material::GetRefractiveIndex() const
{
	return m_RefractiveIndex;
}

inline void Material::SetDiffuseTexture(const TextureInfo &diffuseInfo)
{
	m_DiffuseTextureInfo = diffuseInfo;
}

inline TextureInfo Material::GetDiffuseTexture() const
{
	return m_DiffuseTextureInfo;
}
