#include <iostream>
#include "Factorial.h"

#include <string>

long double factorial(const unsigned long n)
{
	if (n == 0)
	{
		return 1;
	}
	const long double ret = factorial(n - 1);
	return n * ret;
}

void factorial_main()
{
	std::string input;
	std::cout << "Enter a number: ";
	std::cin >> input;
	const unsigned long n = std::stoul(input);
	std::cout << "Factorial of " << input << " is " << factorial(n) << std::endl;
}