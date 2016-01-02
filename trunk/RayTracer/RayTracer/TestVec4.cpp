
#include <iostream>
#include "Vector4.h"

int main()
{
	Vector4f a(1, 3, 4);
	Vector4f b(2, -5, 8);


	Vector4f sum = a + b;
	std::cout << sum << std::endl;

	std::cout << std::endl;

	Vector4f minus = b - a;
	std::cout << minus << std::endl;

	std::cout << std::endl;

	sum = sum * 2;
	std::cout << sum << std::endl;

	std::cout << std::endl;

	sum /= 2;
	std::cout << sum << std::endl;

	std::cout << std::endl;
	std::cout << " a equal b" << (a == b) << std::endl;
	std::cout << " a not equal b" << (a != b) << std::endl;

	std::cout << "z is  " << sum[2] << std::endl;

	std::cout << "the length 2 of a is " << a.LengthSquare3() << std::endl;

	std::cout << "the length3 is of a" << a.Length3() << std::endl;

	std::cout << "the length4 is of a" << a.Length4() << std::endl;

	std::cout << "normalize c=a is ";
	Vector4f c = a;
	c.Normalize3();
	std::cout << c << std::endl;

	std::cout << std::endl << "a dot3 b" << Vector4f::Dot3(a, b) << std::endl;

	std::cout << std::endl << "a dot4 b" << Vector4f::Dot4(a, b) << std::endl;

	std::cout << std::endl << "a cross b" << Vector4f::Cross(a, b) << std::endl;
	return 0;
}