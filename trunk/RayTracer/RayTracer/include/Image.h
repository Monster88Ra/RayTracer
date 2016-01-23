#pragma once
#include <vector>
#include <fstream>
#include <cstdint>

#include "Color.h"
#include "Vector2.h"

class Image
{
public:
	Image(const std::string &fileName,const Vector2i &outputResolution);
	~Image();

	void SetPixel(const uint32_t &x, const uint32_t &y, const Color &color);
	void WriteImage();
	void SetFilename(const std::string& fileName);
	std::string GetFilename() const;
private:
	std::vector<std::vector<Color> > m_pixelColors;
	std::string m_fileName;
	Vector2i m_outputResolution;
};
