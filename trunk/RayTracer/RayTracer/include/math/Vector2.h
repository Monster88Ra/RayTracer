#ifndef _VECTOR2_H
#define _VECTOR2_H

#include <cstdint>
#include <cmath>
#include <iostream>
#include <stdexcept>

/*****************************
 * Class of the 2D vector.
******************************/
template <typename T>
class Vector2
{
public:
	Vector2();
	Vector2(T X);
	Vector2(T X, T Y);
	
	template<typename U> 
	Vector2<T>& operator *= (const U &scalar);
	Vector2<T>& operator += (const Vector2<T> &rhs);
	Vector2<T>& operator -= (const Vector2<T> &rhs);
	template<typename U> 
	Vector2<T>& operator /= (const U &scalar);
	bool operator == (const Vector2<T> &rhs) const;
	bool operator != (const Vector2<T> &rhs) const;
	T& operator[] (std::size_t index);
	const T& operator[] (std::size_t index) const;

	void Normalize();
	float Length();
	// square of length
	float LengthSquare();
	Vector2<T> Reflect(const Vector2<T> &normal) const;
	static T Dot(const Vector2<T> &lhs, const Vector2<T> &rhs);
	friend std::ostream& operator << (const std::ostream &os, const Vector3<T> &vec) 
	{
		os << "[" << "X: " << x << "Y: " << y << "]";
		return os;
	}
public:
	T x;
	T y;
};
#endif//_VECTOR2_H

// c++ 11
using Vector2i = Vector2<int32_t>;
using Vector2f = Vector2<float>;

/*****************************
 * Inlined Member Functions 
 ******************************/
template <typename T>
Vector2<T>::Vector2():
	x(0), y(0)
{

}

template <typename T>
Vector2<T>::Vector2(T X) :
x(X), y(X)
{

}

template <typename T>
Vector2<T>::Vector2(T X, T Y) :
x(X), y(Y)
{

}

template <typename T>
Vector2<T>::~Vector2()
{

}

template <typename T>
template<typename U> 
inline Vector2<T>& Vector2<T>::operator *= (const U &scalar)
{
	x *= scalar;
	y *= scalar;

	return *this;
}

template <typename T> 
inline Vector2<T>& Vector2<T>::operator += (const Vector2<T> &rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator -= (const Vector2<T> &rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this
}

template <typename T>
template <typename U>
inline Vector2<T>& Vector2<T>::operator /= (const U &scalar)
{
	x /= scalar;
	y /= scalar;

	return *this;
}

template <typename T>
inline bool Vector2<T>::operator== (const Vector2<T> &rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

template <typename T>
inline bool Vector2<T>::operator!=(const Vector2<T> &rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

template <typename T>
T& Vector2<T>::operator[] (std::size_t index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Vector2 subscript out of range.");
	}
}

template <typename T>
const inline T& Vector2<T>::operator[] (std::size_t index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Vector2 subscript out of range.");
	}
}

template <typename T>
inline float Vector2<T>::LengthSquare()
{
	return x*x + y*y;
}

template <typename T>
inline float Vector2<T>::Length()
{
	return std::sqrt(x*x + y*y);
}

template <typename T>
inline void Vector2<T>::Normalize()
{
	T nor2 = LengthSquare();
	if (nor2 > 0)
	{
		T invNor = 1 / sqrt(nor2);
		x *= invNor;
		y *= invNor;
	}
}

template <typename T>
void Vector2<T>::Print()
{
	std::cout << "X:" << x << " " << "Y:" << y;
}

template <typename T>
inline T Vector2<T>::Dot(const Vector2<T> &lhs,const Vector2<T> &rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

/*****************************
* Non-member Functions
******************************/

template <typename T>
inline Vector2<T> operator + (const Vector2<T> &lhs, const Vector2<T> &rhs)
{
	return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <typename T>
inline Vector2<T> operator - (const Vector2<T> &lhs, const Vector2<T> &rhs)
{
	return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <typename T>
inline Vector2<T> operator - (const Vector2<T> &lhs)
{
	return Vector2<T>(-lhs.x, -lhs.y);
}

template <typename T,typename U>
inline Vector2<T> operator * (const Vector2<T> &vec, const U &scalar)
{
	return Vector2<T>(vec.x * scalar, vec.y * scalar);
}

template <typename T, typename U>
inline Vector2<T> operator * (const U &scalar, const Vector2<T> &vec)
{
	return Vector2<T>(vec.x * scalar, vec.y * scalar);
}

template <typename T, typename U>
inline Vector2<T> operator / (const Vector2<T> &vec, const U &scalar)
{
	return Vector2<T>(vec.x / scalar, vec.y / scalar);
}

template <typename T, typename U>
inline Vector2<T> operator / (const U &scalar, const Vector2<T> &vec)
{
	return Vector2<T>(scalar/vec.x , scalar/vec.y );
}
