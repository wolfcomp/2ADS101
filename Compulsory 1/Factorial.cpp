#include <iostream>
#include "Factorial.h"

int factorial(const int n)
{
	if(n < 0)
	{
		throw std::invalid_argument("n must be greater than or equal to 0");
	}
	if (n == 0)
	{
		return 1;
	}
	return n * factorial(n - 1);
}

void factorial_main()
{
	int n;
	int count;
	std::cout << "Enter a number: ";
	std::cin >> n;
	std::cout << "Factorial of " << n << " is " << factorial(n) << std::endl;
}