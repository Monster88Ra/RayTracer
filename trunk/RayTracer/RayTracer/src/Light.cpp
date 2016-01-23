#include "..\include\lights\light.h"

Light::Light():
	m_LightColor()
{
}

Light::Light(Color lightColor):
	m_LightColor(lightColor)
{
}

Light::~Light()
{
}

void Light::SetlightColor(const Color & lightColor)
{
	m_LightColor = lightColor;
}
