#include "Fibonacci.h"

#include <iostream>
#include <string>

static long double n1 = 0, n2 = 1, n3;

void fibonacci_main()
{
	std::string input;
	n1 = 0;
	n2 = 1;
	std::cout << "Enter a how many positions to find Fibonacci sequence for: ";
	std::cin >> input;
	const unsigned long n = std::stoul(input);
	if (n < 1)
	{
		throw std::invalid_argument("n must be greater than or equal to 1");
	}
	if (n == 1)
		std::cout << n1 << " ";
	if (n >= 2)
		std::cout << n1 << " " << n2 << " ";
	if (n > 2)
		fibonacci(0, n - 2);
}

void fibonacci(unsigned long count, unsigned long limit)
{
	if (count < limit)
	{
		n3 = n1 + n2;
		n1 = n2;
		n2 = n3;
		std::cout << n3 << " ";
		fibonacci(count + 1, limit);
	}
}