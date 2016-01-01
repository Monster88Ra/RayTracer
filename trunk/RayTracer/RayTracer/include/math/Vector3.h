#ifndef _VECTOR3_H
#define _VECTOR3_H

#include <cstdint>
#include <cmath>
#include <iostream>
#include <stdexcept>

/*****************************
* Class of the 3D vector.
******************************/

template <typename T>
class Vector3
{
public:
	Vector3();
	Vector3(T X);
	Vector3(T X, T Y, T Z);
	
	template<typename U>
	Vector3<T>& operator *= (const U &scalar);
	Vector3<T>& operator += (const Vector3<T> &rhs);
	Vector3<T>& operator -= (const Vector3<T> &rhs);
	template<typename U>
	Vector3<T>& operator /= (const U &scalar);
	bool operator == (const Vector3<T> &rhs) const;
	bool operator != (const Vector3<T> &rhs) const;
	T& operator[] (std::size_t index);
	const T& operator[] (std::size_t index) const;

	void Normalize();
	float Length();
	float LengthSquare();

	Vector3<T> Reflect(const Vector3<T>& Normal) const;

	static T Dot(const Vector3<T>& lhs, const Vector3<T>& rhs);
	static Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs);

	friend std::ostream& operator << (std::ostream &os, const Vector3<T> &vec)
	{
		os << "[" << "X: " << vec.x << " Y: " << vec.y << " Z: "<< vec.z<<"]";
		return os;
	}
public:
	T x;
	T y;
	T z;
};


using Vector3i = Vector3<int32_t>;	/* Vector type for integers */
using Vector3f = Vector3<float>; /* Vector type for floats */

/*****************************
* Inlined Member Functions
******************************/
template<typename T>
Vector3<T>::Vector3():
	x(0), y(0), z(0)
{

}

template<typename T>
Vector3<T>::Vector3(T X):
	x(X), y(X), z(X)
{

}

template<typename T>
Vector3<T>::Vector3(T X,T Y,T Z):
	x(X), y(Y), z(Z)
{

}

template<typename T>
template<typename U>
inline Vector3<T>& Vector3<T>::operator *= (const U &scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator+= (const Vector3<T> &rhs)
{
	x += rhs;
	y += rhs;
	z += rhs;

	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator-= (const Vector3<T> &rhs)
{
	x -= rhs;
	y -= rhs;
	z -= rhs;

	return *this;
}

template<typename T>
template<typename U>
inline Vector3<T>& Vector3<T>::operator /= (const U &rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;

	return *this;
}

template<typename T>
inline bool Vector3<T>::operator==(const Vector3<T>& rhs) const
{
	return (x==rhs.x && y==rhs.y && z==rhs.z);
}

template<typename T>
inline bool Vector3<T>::operator!=(const Vector3<T>& rhs) const
{
	return !(x == rhs.x && y == rhs.y && z == rhs.z);;
}

template<typename T>
inline T & Vector3<T>::operator[](std::size_t index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("TVector3 subscript out of range.");
	}
}

template<typename T>
inline const T & Vector3<T>::operator[](std::size_t index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("TVector3 subscript out of range.");
	}
}

template<typename T>
inline void Vector3<T>::Normalize()
{
	T nor2 = LengthSquare();
	if (nor2 > 0)
	{
		T invNor = 1 / sqrt(nor2);
		x *= invNor;
		y *= invNor;
		z *= invNor;
	}
}

template<typename T>
inline float Vector3<T>::Length()
{
	return std::sqrtf(x*x + y*y + z*z);
}

template<typename T>
inline float Vector3<T>::LengthSquare()
{
	return (x*x + y*y + z*z);
}

template<typename T>
inline Vector3<T> Vector3<T>::Reflect(const Vector3<T>& normal) const
{
	return 2 * Dot(*this, normal) * normal - *this;
}

template<typename T>
inline T Vector3<T>::Dot(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename T>
inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(	(lhs.y * rhs.z) - (lhs.z * rhs.y),
						(lhs.z * rhs.x) - (lhs.x * rhs.z),
						(lhs.x * rhs.y) - (lhs.y * rhs.x));
}

/*****************************
* Non-member Functions
******************************/

template <typename T>
inline Vector3<T> operator + (const Vector3<T> &lhs, const Vector3<T> &rhs)
{
	return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename T>
inline Vector3<T> operator - (const Vector3<T> &lhs, const Vector3<T> &rhs)
{
	return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template <typename T>
inline Vector3<T> operator - (const Vector3<T> &lhs)
{
	return Vector3<T>(-lhs.x, -lhs.y, -lhs.z);
}

template <typename T, typename U>
inline Vector3<T> operator * (const Vector3<T> &vec, const U &scalar)
{
	return Vector3<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

template <typename T, typename U>
inline Vector3<T> operator * (const U &scalar, const Vector3<T> &vec)
{
	return Vector3<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

template <typename T, typename U>
inline Vector3<T> operator / (const Vector3<T> &vec, const U &scalar)
{
	return Vector3<T>(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

#endif//_VECTOR3_H