#pragma once

#include <string>
#include <vector>
#include "Color.h"

class Texture
{
public:
	Texture(const std::string filename);
	~Texture();

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	Color GetPixel(uint32_t X,uint32_t Y) const;

	Color GetSample(float U, float V) const;

	friend static bool ReadTGAImage(Texture &texture, const std::string filename);

private:
	std::vector<Color> m_Pixel;
	uint32_t m_Width;
	uint32_t m_Height;
};