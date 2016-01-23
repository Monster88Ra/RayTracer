#ifndef _VECTOR4_H
#define _VECTOR4_H


#include <cstdint>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "Vector3.h"

/*****************************
* Class of the 3D vector.
******************************/

template<typename T>
class Vector4
{
public:
	Vector4();
	Vector4(T X);
	Vector4(T X, T Y, T Z, T W=1);
	Vector4(const Vector3<T> &vec3);

	Vector4<T>& operator += (const Vector4<T> &rhs);
	Vector4<T>& operator -= (const Vector4<T> &rhs);
	template<typename U>
	Vector4<T>& operator *= (const U &scalar);
	template<typename U>
	Vector4<T>& operator /= (const U &scalar);
	bool operator == (const Vector4<T> &rhs) const;
	bool operator != (const Vector4<T> &rhs) const;
	T& operator[] (std::size_t index);
	const T& operator[] (std::size_t index) const;

	// normalize x,y,z
	Vector4<T>& Normalize3();
	// length of vector with x,y,z,w
	float Length4();
	// length of vector with x,y,z
	float Length3();
	// square length of vector with x,y,z
	float LengthSquare3();
	// Reflects the vector against a normal.
	// return a Vector4
	Vector4<T>& Reflect3(const Vector4<T>& normal) const;

	// Calculates the dot product of two vectors with x, y, and z components.
	static T Dot3(const Vector4<T> &lhs, const Vector4<T> &rhs);
	// Calculates the dot product of two vectors with x, y,z and w components.
	static T Dot4(const Vector4<T> &lhs, const Vector4<T> &rhs);
	static Vector4<T> Cross(const Vector4<T> &lhs, const Vector4<T> &rhs);

	friend std::ostream& operator << (std::ostream &os, const Vector4<T> &vec)
	{
		os << "[" << "X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z << " W: "<<vec.w << "]";
		return os;
	}
public:
	T x;
	T y;
	T z;
	T w;
};

using Vector4i = Vector4<int32_t>;	/* Vector type for integers */
using Vector4f = Vector4<float>; /* Vector type for floats */

/*****************************
* Inlined Member Functions
******************************/
//#endif//_VECTOR4_H

template<typename T>
inline Vector4<T>::Vector4():
	x(0),y(0),z(0),w(1)
{
}

template<typename T>
inline Vector4<T>::Vector4(T X):
	x(X),y(X),z(X),w(1)
{
}

template<typename T>
inline Vector4<T>::Vector4(T X, T Y, T Z, T W):
	x(X),y(Y),z(Z),w(W)
{
}

template<typename T>
inline Vector4<T>::Vector4(const Vector3<T>& vec3):
	Vector4(vec3.x,vec3.y,vec3.z)
{
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
}

template<typename T>
inline bool Vector4<T>::operator==(const Vector4<T>& rhs) const
{
	return (x==rhs.x && y==rhs.y && z==rhs.z && w==rhs.w);
}

template<typename T>
inline bool Vector4<T>::operator!=(const Vector4<T>& rhs) const
{
	return !(x== rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
}

template<typename T>
inline T & Vector4<T>::operator[](std::size_t index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::out_of_range("Vector4 subscript out of range.");
	}
}

template<typename T>
inline const T & Vector4<T>::operator[](std::size_t index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::out_of_range("Vector4 subscript out of range.");
	}
}

template<typename T>
inline Vector4<T>& Vector4<T>::Normalize3()
{
	/*
	T nor2 = LengthSquare3();
	if (nor2 > 0)
	{
		T invNor = 1 / sqrt(nor2);
		x *= invNor;
		y *= invNor;
		z *= invNor;
	}
	*/
	float invLength = 1.0f / Length3();
	return *this *= invLength;
}

template<typename T>
inline float Vector4<T>::Length4()
{
	return std::sqrtf(x*x + y*y + z*z + w*w);
}

template<typename T>
inline float Vector4<T>::Length3()
{
	return std::sqrtf(x*x + y*y + z*z);
}

template<typename T>
inline float Vector4<T>::LengthSquare3()
{
	return (x*x + y*y + z*z);
}

template<typename T>
inline Vector4<T>& Vector4<T>::Reflect3(const Vector4<T>& normal) const
{
	return 2.0f * Dot3(*this, normal) * normal - *this;
}

template<typename T>
inline T Vector4<T>::Dot3(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename T>
inline T Vector4<T>::Dot4(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return Dot3(lhs, rhs) + lhs.w * rhs.w;
}

template<typename T>
inline Vector4<T> Vector4<T>::Cross(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return Vector4<T>(lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x,
		0.0f);
}

template<typename T>
template<typename U>
inline Vector4<T>& Vector4<T>::operator*=(const U & scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	
	return *this;
}

template<typename T>
template<typename U>
inline Vector4<T>& Vector4<T>::operator/=(const U & scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;

	return *this;
}

/*****************************
* Non-member Functions
******************************/

template<typename T>
inline Vector4<T> operator + (const Vector4<T> &lhs, const Vector4<T> &rhs)
{
	return Vector4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template<typename T>
inline Vector4<T> operator - (const Vector4<T> &lhs, const Vector4<T>& rhs)
{
	return Vector4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template<typename T>
inline Vector4<T> operator - (const Vector4<T> &lhs)
{
	return Vector4<T>(-lhs.x, -lhs.y, -lhs.z, -lhs.w);
}

template<typename T,typename U>
inline Vector4<T> operator*(const Vector4<T>& vec, const U& scalar)
{
	return Vector4<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
}

template<typename T,typename U>
inline Vector4<T> operator*(const U& scalar, const Vector4<T>& vector)
{
	return vector*scalar;
}

template<typename T,typename U>
inline Vector4<T> operator/(const Vector4<T>& vec, const U& scalar)
{
	return Vector4<T>(vec.x / scalar, vec.y / scalar, vec.z / scalar, vec.w / scalar);
}

#endif //_VECTOR4_H