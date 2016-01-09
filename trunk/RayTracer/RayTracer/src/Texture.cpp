#include "Texture.h"

#include <fstream>
#include <iostream>
#include <cmath>

Texture::Texture(const std::string filename)
{
	if (filename.substr(filename.length() - 4) == ".tga")
	{
		ReadTGAImage(*this, filename);
	}
	else
	{
		std::cout << filename << "file format not supported" << std::endl;
	}
}

Texture::~Texture()
{

}

uint32_t Texture::GetWidth() const
{
	return m_Width;
}

uint32_t Texture::GetHeight() const
{
	return m_Height;
}

Color Texture::GetPixel(uint32_t X, uint32_t Y) const
{
	return m_Pixel[Y*m_Width + X];
}

Color Texture::GetSample(float U, float V) const
{
	// use bilinear filtering 
	// reference Wikipedia

	float If, Jf;

	const float fracI = modf(uint32_t(U * m_Width) % m_Width - 0.5f, &I);
	const float fracJ = modf(uint32_t(U * m_Height) % m_Height - 0.5f, &J);

	const uint32_t I = (uint32_t)(If);
	const uint32_t J = (uint32_t)(Jf);

	return (1 - fracI)*(1 - fracJ)*GetPixel(I, J) +
		fracI*(1 - fracJ)*GetPixel(I + 1, J) +
		(1 - fracI)*fracJ*GetPixel(I, J + 1) +
		fracI*fracJ*GetPixel(I + 1, J + 1); 
}

static bool ReadTGAImage(Texture &Texture, const std::string Filename)
{
	std::fstream InputFile(Filename, std::ios::in | std::ios::binary);
	if (!InputFile.is_open())
	{
		std::cout << "Could not open image file: " << Filename << std::endl;
		return false;
	}

	static const uint8_t UnCompressed[10] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	static const uint8_t Compressed[10] = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

	uint8_t HeaderData[18] = { 0 };
	std::vector<uint8_t> ImageData;
	uint8_t BitsPerPixel;
	InputFile.read(reinterpret_cast<char*>(&HeaderData), sizeof(HeaderData));

	// read compressed file
	if (!std::memcmp(UnCompressed, &HeaderData, sizeof(UnCompressed)))
	{
		BitsPerPixel = HeaderData[16];
		Texture.m_Width = (HeaderData[13] << 8) + HeaderData[12];
		Texture.m_Height = (HeaderData[15] << 8) + HeaderData[14];
		const uint32_t& ImageSize = ((Texture.m_Width * BitsPerPixel + 31) / 32) * 4 * Texture.m_Height;

		if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
		{
			std::cout << "Invalid .tga format. Bits per pixel must be 24 or 32: " << Filename << std::endl;
			return false;
		}

		ImageData.resize(ImageSize);
		InputFile.read(reinterpret_cast<char*>(ImageData.data()), ImageSize);
	}
	// read uncompressed file
	else if (!std::memcmp(Compressed, &HeaderData, sizeof(Compressed)))
	{
		BitsPerPixel = HeaderData[16];
		// .tga is little edian, so bitshift
		Texture.m_Width = (HeaderData[13] << 8) + HeaderData[12];
		Texture.m_Height = (HeaderData[15] << 8) + HeaderData[14];

		if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
		{
			std::cout << "Invalid .tga format. Bits per pixel must be 24 or 32: " << Filename << std::endl;
			return false;
		}

		struct FPixel { uint8_t R, G, B, A; };

		FPixel Pixel;
		uint32_t CurrentByte = 0;
		size_t CurrentPixel = 0;
		uint8_t Header = { 0 };
		uint8_t BytesPerPixel = (BitsPerPixel / 8);
		ImageData.resize(Texture.m_Width * Texture.m_Height * sizeof(FPixel));

		do
		{
			InputFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));

			if (Header < 128)
			{
				++Header;
				for (int i = 0; i < Header; i++, CurrentPixel++)
				{
					InputFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

					ImageData[CurrentByte++] = Pixel.B;
					ImageData[CurrentByte++] = Pixel.G;
					ImageData[CurrentByte++] = Pixel.R;

					if (BitsPerPixel > 24)
						ImageData[CurrentByte++] = Pixel.A;
				}
			}
			else
			{
				Header -= 127;
				InputFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

				for (int i = 0; i < Header; i++, CurrentPixel++)
				{
					ImageData[CurrentByte++] = Pixel.B;
					ImageData[CurrentByte++] = Pixel.G;
					ImageData[CurrentByte++] = Pixel.R;

					if (BitsPerPixel > 24)
						ImageData[CurrentByte++] = Pixel.A;
				}
			}
		} while (CurrentPixel < (Texture.m_Width * Texture.m_Height));
	}
	// error in file structure
	else
	{
		InputFile.close();
		std::cout << "Error in .tga header file: " << Filename << std::endl;
		return false;
	}

	InputFile.close();

	Texture.m_Pixel.clear();
	const float InvColorMax(1.0f / 255.0f);
	for (size_t i = 0; i < Texture.m_Width * Texture.m_Height; i++)
	{
		const uint32_t& BaseIndex = i * ((BitsPerPixel > 24) ? 4 : 3);
		const float& R = (float)ImageData[BaseIndex] * InvColorMax;
		const float& G = (float)ImageData[BaseIndex + 1] * InvColorMax;
		const float& B = (float)ImageData[BaseIndex + 2] * InvColorMax;
		Texture.m_Pixel.push_back(Color(R, G, B));
	}

	return true;
}