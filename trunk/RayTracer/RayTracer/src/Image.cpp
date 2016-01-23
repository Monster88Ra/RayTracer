#include <Windows.h>
#include <sstream>
#include <cassert>
#include <limits>
#include "Image.h"

Image::Image(const std::string &fileName, const Vector2i &outputResolution):
	m_pixelColors(outputResolution.y),
	m_outputResolution(outputResolution),
	m_fileName(fileName)
{
	// Resize each vector to the amount of vertical pixels
	for (auto& row : m_pixelColors)
		row.resize(outputResolution.x);
}

Image::~Image()
{

}

void Image::SetPixel(const uint32_t &x, const uint32_t &y, const Color &color)
{
	assert(y < m_pixelColors.capacity());
	assert(x < m_pixelColors[y].capacity());
	m_pixelColors[y][x] = color;
}

void Image::WriteImage()
{
	// Write to a .ppm image
	m_fileName += ".ppm";
	std::ostringstream headerStream;
	headerStream << "P6\n";
	headerStream << m_outputResolution.x << ' ' << m_outputResolution.y << '\n';
	headerStream << "255\n";
	std::ofstream fileStream(m_fileName, std::ios::out | std::ios::binary);
	fileStream << headerStream.str();

	for (const auto& row : m_pixelColors)
		for (const auto& pixel : row)
			fileStream << static_cast<unsigned char>(pixel.m_R * 255)
			<< static_cast<unsigned char>(pixel.m_G * 255)
			<< static_cast<unsigned char>(pixel.m_B * 255);

	fileStream.flush();
	fileStream.close();

	// open the new image
	const std::wstring WFilename(m_fileName.begin(), m_fileName.end());
	ShellExecute(0, 0, WFilename.c_str(), 0, 0, SW_SHOW);
}

void Image::SetFilename(const std::string& Filename)
{
	m_fileName = Filename;
}

std::string Image::GetFilename() const
{
	return m_fileName;
}
