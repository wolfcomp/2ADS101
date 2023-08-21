// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ostream>
#include <thread>

#include "Factorial.h"
#include "Fibonacci.h"

int main()
{
	char c;

	while (true)
	{
		try
		{
			std::cout << "Choose an option: " << std::endl;
			std::cout << "1. Factorial" << std::endl;
			std::cout << "2. Fibonacci" << std::endl;
			std::cout << "q. Quit" << std::endl;
			std::cin >> c;
			std::cout << "\x1B[2J\x1B[H";
			switch (c)
			{
			case '1':
				factorial_main();
				break;
			case '2':
				fibonacci_main();
				break;
			case 'q':
				return 0;
			}
			std::this_thread::sleep_for(std::chrono::seconds(5));

			std::cout << "\x1B[2J\x1B[H";
		}
		catch (std::invalid_argument& e)
		{
			std::cout << e.what() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));

			return 1;
		}
		catch (...)
		{
			std::cout << "An unknown error occurred" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));

			return 1;
		}
	}
}