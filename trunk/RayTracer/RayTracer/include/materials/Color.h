#pragma once

#include <algorithm>
#include <stdint.h>
#include <stdexcept>

class Color
{
public:
	Color();
	Color(float R, float G, float B);
	Color(float R, float G, float B, float A);
	Color(const Color &rhs);
	~Color();

	Color& operator=(const Color &rhs);

	Color& operator += (const Color &rhs);
	Color& operator *= (const Color &rhs);
	Color& operator *= (const float scalar);
	Color& operator /= (const float scalar);

	bool operator == (const Color &rhs);
	bool operator != (const Color &rhs);

	float& operator [] (std::size_t index);
	const float& operator [] (std::size_t index) const;
	
	Color& Clamp();
public:
	float m_R;
	float m_G;
	float m_B;
	float m_A;
public:
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color White;
	static const Color Black;
};
/*****************************
* Inlined Member Functions
******************************/
inline Color::Color():
	m_A(0.0f),m_B(0.0f),m_G(0.0f),m_R(0.0f)
{
}

inline Color::Color(float R, float G, float B):
	m_R(R),m_G(G),m_B(B)
{
}

inline Color::Color(float R, float G, float B, float A):
	m_R(R), m_G(G), m_B(B),m_A(A)
{
}

inline Color::Color(const Color & rhs):
	m_R(rhs.m_R), m_G(rhs.m_G), m_B(rhs.m_B), m_A(rhs.m_A)
{
}

Color::~Color()
{
}

inline Color & Color::operator=(const Color & rhs)
{
	m_R = rhs.m_R;
	m_G = rhs.m_G;
	m_B = rhs.m_B;
	m_A = rhs.m_A;

	return *this;
}

inline Color & Color::operator+=(const Color & rhs)
{
	m_R += rhs.m_R;
	m_G += rhs.m_G;
	m_B += rhs.m_B;

	return *this;
}

inline Color & Color::operator*=(const Color & rhs)
{
	m_R *= rhs.m_R;
	m_G *= rhs.m_G;
	m_B *= rhs.m_B;

	return *this;
}

inline Color & Color::operator*=(const float scalar)
{
	m_R *= scalar;
	m_G *= scalar;
	m_B *= scalar;

	return *this;
}

inline Color & Color::operator/=(const float scalar)
{
	m_R /= scalar;
	m_G /= scalar;
	m_B /= scalar;

	return *this;
}

inline bool Color::operator==(const Color & rhs)
{
	return (m_R == rhs.m_R && m_G == rhs.m_G && m_B == rhs.m_B && m_A == rhs.m_A);
}

inline bool Color::operator!=(const Color & rhs)
{
	return !(*this == rhs);
}

inline float& Color::operator[](std::size_t index)
{
	const_cast<float&>(static_cast<const Color&>(*this)[index]);
}

inline const float& Color::operator[](std::size_t index) const
{
	switch (index)
	{
	case 0:
		return m_R;
	case 1:
		return m_G;
	case 2:
		return m_B;
	case 3:
		return m_A;
	default:
		std::out_of_range("FColor subscript out of range.");
	}
}

inline Color & Color::Clamp()
{
	m_R = std::min(m_R, 1.0f);
	m_G = std::min(m_G, 1.0f);
	m_B = std::min(m_B, 1.0f);

	return *this;
}
/*****************************
* Non Member Functions
******************************/
inline Color operator * (const Color &lhs, const Color &rhs)
{
	return Color(lhs.m_R*rhs.m_R, lhs.m_G*rhs.m_G, lhs.m_B*rhs.m_B, lhs.m_A*rhs.m_A);
}

inline Color operator * (const Color &lhs, const float scalar)
{
	return Color(lhs.m_R*scalar, lhs.m_G*scalar, lhs.m_B*scalar);
}

inline Color operator * (const float scalar, const Color &rhs)
{
	return Color(rhs.m_R*scalar, rhs.m_G*scalar, rhs.m_B*scalar);
}

inline Color operator + (const Color &lhs, const Color &rhs)
{
	return Color(lhs.m_R + rhs.m_R, lhs.m_G + rhs.m_G, lhs.m_B + rhs.m_B);
}