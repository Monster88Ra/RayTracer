
#include "Matrix4.h"

int main()
{
	Matrix4 A = Matrix4{
		1, 1, 1, 1,
		1, 1, -1, -1,
		1, -1, 1, -1,
		1, -1, -1, 1
	};
	std::cout << A.Transpose() <<std::endl;

	std::cout << A.Inverse() << std::endl;

	std::cout << A.InverseAffine() << std::endl;

	std::cout << A * A << std::endl;

	std::cout << A * A * A <<  std::endl;

	std::cout << 4 * A << std::endl;

	Matrix4 matRotate = Matrix4();
	matRotate.Rotate(EAxis::Y, 45);
	std::cout << matRotate << std::endl;

	Matrix4 matRotate1 = Matrix4();
	matRotate1.Rotate(Vector3f(0, 1, 0), 45);
	std::cout << matRotate1 << std::endl;

	A.Scale(Vector3f(1,2,3));
	std::cout << "A scale vector3f(1,2,3)" << std::endl << A << std::endl;

	return 0;
}
