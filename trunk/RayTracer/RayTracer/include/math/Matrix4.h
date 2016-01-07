#pragma once

#include <initializer_list>
#include <cassert>
#include <cmath>
#include <mmintrin.h>


#include "Ray.h"
#include "MathUtiliy.h"
#include "Vector3.h"
#include "Vector4.h"

/*****************************
* Class of the 4x4 column-matrix.
******************************/

class Matrix4
{
public:
	Matrix4();
	Matrix4(const std::initializer_list<float> &mat);
	Matrix4(const Vector3f &x, const Vector3f &y, const Vector3f &z);

	Matrix4(const Matrix4 &rhs);

	Matrix4& operator = (const Matrix4 &rhs);

	Matrix4& operator += (const Matrix4 &rhs);
	Matrix4& operator *= (const Matrix4 &rhs);
	Matrix4& operator *= (const float scalar);

	bool operator == (const Matrix4 &rhs) const;
	bool operator != (const Matrix4 &rhs) const;

	Matrix4 Transpose() ;

	Vector4f TransformVector(const Vector4f &vec) const;
	Vector3f TransformDirection(const Vector3f &dir) const;
	Vector3f TransformPosition(const Vector3f &pos) const;
	Ray TransformRay(Ray ray) const;

	void Rotate(Vector3f vec, float degree);
	void Rotate(EAxis axis, float degree);
	void Rotate(Vector3f degrees);
	void Scale(float scale);
	void Scale(EAxis axis, float scale);
	// x, y ,z anxis scale
	void Scale(const Vector3f &scale);
	Matrix4 Inverse() const;
	Matrix4 InverseAffine() const;

	Vector3f GetAxis(EAxis axis) const;
	void SetAxis(EAxis axis,const Vector3f &vec);
	// get world origin
	Vector3f GetOrigin() const;
	void SetOrigin(const Vector3f &origin);

	Vector3f GetScale() const;
	Vector4f GetColum(int col) const;
	Vector4f GetRow(int row) const;

	friend std::ostream& operator << (std::ostream &os, const Matrix4 &mat)
	{
		os << mat.m[0][0] << "\t" << mat.m[0][1] << "\t" << mat.m[0][2] << "\t" << mat.m[0][3] << std::endl;
		os << mat.m[1][0] << "\t" << mat.m[1][1] << "\t" << mat.m[1][2] << "\t" << mat.m[1][3] << std::endl;
		os << mat.m[2][0] << "\t" << mat.m[2][1] << "\t" << mat.m[2][2] << "\t" << mat.m[2][3] << std::endl;
		os << mat.m[3][0] << "\t" << mat.m[3][1] << "\t" << mat.m[3][2] << "\t" << mat.m[3][3] << std::endl;

		return os;
	}
public:
	float m[4][4];
};



/*****************************
* Non-member Functions
******************************/
inline Matrix4 operator * (const  Matrix4 &lhs, const Matrix4 &rhs)
{
	Matrix4 result;
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			result.m[row][col] = Vector4f::Dot4(lhs.GetRow(row), rhs.GetColum(col));
		}
	}
	return result;
}

inline Matrix4 operator * (const Matrix4 &lhs, const float scalar)
{
	Matrix4 result;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			result.m[row][col] = lhs.m[row][col] * scalar;
		}
	}
	return result;
}

inline Matrix4 operator * (const float scalar, const Matrix4 &rhs)
{
	return rhs * scalar;
}

inline Matrix4 operator + (const Matrix4 &lhs, const Matrix4 &rhs)
{
	Matrix4 result;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			result.m[row][col] = lhs.m[row][col] + rhs.m[row][col];
		}
	}
	return result;
}


/*****************************
* Inlined Member Functions
******************************/
inline Matrix4::Matrix4():
	Matrix4({
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f})
{

}

inline Matrix4::Matrix4(const std::initializer_list<float> &mat)
{
	assert(mat.size() == 16);

	const float *itr = mat.begin();
	m[0][0] = *(itr++); m[0][1] = *(itr++); m[0][2] = *(itr++); m[0][3] = *(itr++);
	m[1][0] = *(itr++); m[1][1] = *(itr++); m[1][2] = *(itr++); m[1][3] = *(itr++);
	m[2][0] = *(itr++); m[2][1] = *(itr++); m[2][2] = *(itr++); m[2][3] = *(itr++);
	m[3][0] = *(itr++); m[3][1] = *(itr++); m[3][2] = *(itr++); m[3][3] = *(itr++);
}

inline Matrix4::Matrix4(const Vector3f & x, const Vector3f & y, const Vector3f & z)
{
	for (int i = 0; i < 3; i++)
	{
		m[0][i] = x[i];
		m[1][i] = y[i];
		m[2][i] = z[i];
	}
	m[3][0] = m[3][1] = m[3][2] = m[0][3] = m[1][3] = m[2][3] = 0.0f;
	m[3][3] = 1.0f;

}

inline Matrix4::Matrix4(const Matrix4 & rhs)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = rhs.m[i][j];
		}
	}
}

inline Matrix4 & Matrix4::operator=(const Matrix4 & rhs)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = rhs.m[i][j];
		}
	}
	return *this;
}

inline Matrix4 & Matrix4::operator+=(const Matrix4 & rhs)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] += rhs.m[i][j];
		}
	}
	return *this;
}

inline Matrix4 & Matrix4::operator*=(const Matrix4 & rhs)
{
	float temp[4][4];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i][j] = Vector4f::Dot4(GetRow(i), rhs.GetColum(j));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = temp[i][j];
		}
	}

	return *this;
}

inline Matrix4 & Matrix4::operator*=(const float scalar)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] *= scalar;
		}
	}
	return *this;
}

inline bool Matrix4::operator==(const Matrix4 & rhs) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m[i][j] != rhs.m[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

inline bool Matrix4::operator!=(const Matrix4 & rhs) const
{
	return !(*this == rhs);
}

inline Matrix4 Matrix4::Transpose()
{
	return Matrix4{
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]
	};
}

inline Vector4f Matrix4::TransformVector(const Vector4f & vec) const
{
	Vector4f ret;
	ret.x = Vector4f::Dot4(GetRow(0), vec);
	ret.y = Vector4f::Dot4(GetRow(1), vec);
	ret.z = Vector4f::Dot4(GetRow(2), vec);
	ret.w = Vector4f::Dot4(GetRow(3), vec);

	return ret;
}

inline Vector3f Matrix4::TransformDirection(const Vector3f & dir) const
{
	Vector4f ret(dir);
	ret.w = 0.0;
	ret = TransformVector(ret);
	return Vector3f(ret.x, ret.y, ret.z);
}

inline Vector3f Matrix4::TransformPosition(const Vector3f & pos) const
{
	Vector4f ret(pos);
	ret = TransformVector(ret);
	return Vector3f(ret.x, ret.y, ret.z);
}

inline Ray Matrix4::TransformRay(Ray ray) const
{
	ray.origin = TransformPosition(ray.origin);
	ray.direction = TransformPosition(ray.direction);
	return ray;
}

inline void Matrix4::Rotate(Vector3f vec, float degree)
{
	// because the original method are right hand coordinate
	degree *= -1;
	float radians = (float)(_PI * degree / 180.0f);
	vec.Normalize();
	float nx = vec.x, ny = vec.y, nz = vec.z;

	float cosRadians = cosf(radians);
	float sinRadians = sinf(radians);

	Matrix4 rotateMat = Matrix4{
						nx*nx*(1 - cosRadians) + cosRadians,	nx*ny*(1 - cosRadians) + nz*sinRadians, nx*nz*(1 - cosRadians) - ny*sinRadians, 0,
						nx*ny*(1 - cosRadians) - nz*sinRadians,	ny*ny*(1 - cosRadians) + cosRadians,	ny*nz*(1 - cosRadians) + nx*sinRadians, 0,
						nx*nz*(1 - cosRadians) + ny*sinRadians,	ny*nz*(1 - cosRadians) - nx*sinRadians,	nz*nz*(1 - cosRadians) + cosRadians,	0,
						0,										0,										0,										1
	};

	Vector3f translation = GetOrigin();
	m[0][3] = m[1][3] = m[2][3] = 0.0f;

	*this = rotateMat * (*this);
	SetOrigin(translation);
}

inline void Matrix4::Rotate(EAxis axis, float degree)
{
	float radians = (float)(_PI * degree / 180.0f);
	Matrix4 rotationMat;

	switch (axis)
	{
	case EAxis::X:
		rotationMat = Matrix4{
			1.0f,			0.0f,					0.0f,					0.0f,
			0.0f,			cosf(radians),			-sinf(radians),	0.0f,
			0.0f,			sinf(radians),			cosf(radians),	0.0f,
			0.0f,			 0.0f,					0.0f,					1.0f };
		break;
	case EAxis::Y:
		rotationMat = Matrix4{
			cosf(radians),	0.0f,	sinf(radians),	0.0f,
			0.0f,					1.0f,	0.0f,					0.0f,
			-sinf(radians),	0.0f,	cosf(radians),	0.0f,
			0.0f,					0.0f,	0.0f,					1.0f };
		break;
	case EAxis::Z:
		rotationMat = Matrix4{
			cosf(radians),	-sinf(radians),	0.0f,	0.0f,
			sinf(radians),	cosf(radians),	0.0f,	0.0f,
			0.0f,					0.0f,					1.0f,	0.0f,
			0.0f,					0.0f,					0.0f,	1.0f };
		break;
	default:
		return;
	}
	// save and remove translation
	Vector3f translation = GetOrigin();
	m[0][3] = m[1][3] = m[2][3] = 0.0f;

	*this = rotationMat * (*this);
	SetOrigin(translation);
}

inline void Matrix4::Rotate(Vector3f degrees)
{
	Rotate(EAxis::X, degrees.x);
	Rotate(EAxis::Y, degrees.y);
	Rotate(EAxis::Z, degrees.z);
}

inline void Matrix4::Scale(float scale)
{
	Scale(Vector3f(scale, scale, scale));
}

inline void Matrix4::Scale(EAxis axis, float scale)
{
	int intAxis = -1;
	switch (axis)
	{
	case EAxis::X:
		intAxis = 0;
		break;
	case EAxis::Y:
		intAxis = 1;
		break;
	case EAxis::Z:
		intAxis = 2;
		break;
	default:
		return ;
	}

	Vector3f newScale = GetScale();
	newScale[intAxis] = scale;
	Scale(newScale);
}

inline void Matrix4::Scale(const Vector3f & scale)
{
	Matrix4 scaleMat;
	for (int i = 0; i < 3; i++)
	{
		scaleMat.m[i][i] = scale[i];
	}

	// save transform and back to origin
	const Vector3f& oldTrans(GetOrigin());
	
	*this = (*this) * scaleMat;
	SetOrigin(oldTrans);
}

inline Matrix4 Matrix4::Inverse() const
{
	Matrix4 inv;
	float* invM = &(inv.m[0][0]);
	const float	m0 = m[0][0], m1 = m[0][1], m2 = m[0][2], m3 = m[0][3],
		m4 = m[1][0], m5 = m[1][1], m6 = m[1][2], m7 = m[1][3],
		m8 = m[2][0], m9 = m[2][1], m10 = m[2][2], m11 = m[2][3],
		m12 = m[3][0], m13 = m[3][1], m14 = m[3][2], m15 = m[3][3];

	invM[0] = m5 * m10 * m15 - m5 * m11 * m14 - m9 * m6 * m15 + m9 * m7 * m14 + m13 * m6 * m11 - m13 * m7 * m10;
	invM[1] = -m1 * m10 * m15 + m1 * m11 * m14 + m9 * m2 * m15 - m9 * m3 * m14 - m13 * m2 * m11 + m13 * m3 * m10;
	invM[2] = m1 * m6  * m15 - m1 * m7  * m14 - m5 * m2 * m15 + m5 * m3 * m14 + m13 * m2 * m7 - m13 * m3 * m6;
	invM[3] = -m1 * m6  * m11 + m1 * m7  * m10 + m5 * m2 * m11 - m5 * m3 * m10 - m9  * m2 * m7 + m9  * m3 * m6;
	invM[4] = -m4 * m10 * m15 + m4 * m11 * m14 + m8 * m6 * m15 - m8 * m7 * m14 - m12 * m6 * m11 + m12 * m7 * m10;
	invM[5] = m0 * m10 * m15 - m0 * m11 * m14 - m8 * m2 * m15 + m8 * m3 * m14 + m12 * m2 * m11 - m12 * m3 * m10;
	invM[6] = -m0 * m6  * m15 + m0 * m7  * m14 + m4 * m2 * m15 - m4 * m3 * m14 - m12 * m2 * m7 + m12 * m3 * m6;
	invM[7] = m0 * m6  * m11 - m0 * m7  * m10 - m4 * m2 * m11 + m4 * m3 * m10 + m8  * m2 * m7 - m8  * m3 * m6;
	invM[8] = m4 * m9  * m15 - m4 * m11 * m13 - m8 * m5 * m15 + m8 * m7 * m13 + m12 * m5 * m11 - m12 * m7 * m9;
	invM[9] = -m0 * m9  * m15 + m0 * m11 * m13 + m8 * m1 * m15 - m8 * m3 * m13 - m12 * m1 * m11 + m12 * m3 * m9;
	invM[10] = m0 * m5  * m15 - m0 * m7  * m13 - m4 * m1 * m15 + m4 * m3 * m13 + m12 * m1 * m7 - m12 * m3 * m5;
	invM[11] = -m0 * m5  * m11 + m0 * m7  * m9 + m4 * m1 * m11 - m4 * m3 * m9 - m8  * m1 * m7 + m8  * m3 * m5;
	invM[12] = -m4 * m9  * m14 + m4 * m10 * m13 + m8 * m5 * m14 - m8 * m6 * m13 - m12 * m5 * m10 + m12 * m6 * m9;
	invM[13] = m0 * m9  * m14 - m0 * m10 * m13 - m8 * m1 * m14 + m8 * m2 * m13 + m12 * m1 * m10 - m12 * m2 * m9;
	invM[14] = -m0 * m5  * m14 + m0 * m6  * m13 + m4 * m1 * m14 - m4 * m2 * m13 - m12 * m1 * m6 + m12 * m2 * m5;
	invM[15] = m0 * m5  * m10 - m0 * m6  * m9 - m4 * m1 * m10 + m4 * m2 * m9 + m8  * m1 * m6 - m8  * m2 * m5;

	float det = m0 * invM[0] + m1 * invM[4] + m2 * invM[8] + m3 * invM[12];
	if (det != 0.0f)
	{
		det = 1.0f / det;
	}

	for (int i = 0; i < 16; i++)
	{
		invM[i] *= det;
	}

	return inv;
}

inline Matrix4 Matrix4::InverseAffine() const
{
	Matrix4 result;
	const Vector3f& translateVector(GetOrigin());

	// transpose 3x3 portion
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			result.m[row][col] = m[col][row];
		}
	}

	Vector3f inverseTranslate;
	inverseTranslate = -result.TransformPosition(translateVector);

	result.SetOrigin(inverseTranslate);
	return result;
}

inline Vector3f Matrix4::GetAxis(EAxis axis) const
{
	switch (axis)
	{
	case EAxis::X:
		return Vector3f(m[0][0],m[1][0],m[2][0]);
	case EAxis::Y:
		return Vector3f(m[0][1],m[1][1],m[2][1]);
	case EAxis::Z:
		return Vector3f(m[0][2],m[1][2],m[2][2]);
	}
}

inline void Matrix4::SetAxis(EAxis axis, const Vector3f & vec)
{
	int col = -1;
	switch (axis)
	{
	case EAxis::X:
		col = 0;
		break;
	case EAxis::Y:
		col = 1;
		break;
	case EAxis::Z:
		col = 2;
		break;
	default:
		return ;
	}
	m[0][col] = vec.x;
	m[1][col] = vec.y;
	m[2][col] = vec.z;
}

inline Vector3f Matrix4::GetOrigin() const
{
	return Vector3f(m[0][3], m[1][3], m[2][3]);
}

inline void Matrix4::SetOrigin(const Vector3f & origin)
{
	m[0][3] = origin.x; m[1][3] = origin.y; m[2][3] = origin.z;
}

inline Vector3f Matrix4::GetScale() const
{
	return Vector3f(m[0][0],m[1][1],m[2][2]);
}

inline Vector4f Matrix4::GetColum(int col) const
{
	return Vector4f(m[0][col], m[1][col], m[2][col], m[3][col]);
}

inline Vector4f Matrix4::GetRow(int row) const
{
	return Vector4f(m[row][0], m[row][1], m[row][2], m[row][3]);
}


/*****************************
* struct of the look-at matrix
******************************/
struct LookAtMatrix:public Matrix4
{
	LookAtMatrix(const Vector3f &eye, const Vector3f &lookLocation, const Vector3f &upDirection);
	LookAtMatrix& operator = (const Matrix4 &rhs);
};

LookAtMatrix::LookAtMatrix(const Vector3f &eye, const Vector3f &lookLocation, const Vector3f &upDirection)
{
	Vector3f n = -(lookLocation - eye).Normalize();
	Vector3f u = Vector3f::Cross(n, upDirection).Normalize();
	Vector3f v = Vector3f::Cross(u, n).Normalize();

	for (int col = 0; col < 3; col++)
	{
		m[0][col] = u[col];
		m[1][col] = v[col];
		m[2][col] = n[col];
		m[3][col] = 0.0f;
	}

	m[0][3] = -Vector3f::Dot(eye, u);
	m[1][3] = -Vector3f::Dot(eye, v);
	m[2][3] = -Vector3f::Dot(eye, n);
	m[3][3] = 1.0f;
}

inline LookAtMatrix & LookAtMatrix::operator=(const Matrix4 & rhs)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			m[row][col] = rhs.m[row][col];
		}
	}

	return *this;
}

 
