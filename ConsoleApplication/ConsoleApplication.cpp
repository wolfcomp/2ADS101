// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ostream>
#include <thread>

#include "Factorial.h"
#include "Fibonacci.h"

int main()
{
	char s;

	while (true)
	{
		try
		{
			std::cout << "Choose an option: " << std::endl;
			std::cout << "1. Factorial" << std::endl;
			std::cout << "2. Fibonacci" << std::endl;
			std::cout << "q. Quit" << std::endl;
			std::cin >> s;
			std::cout << "\x1B[2J\x1B[H";
			switch (s)
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
		}
		catch (...)
		{
			std::cout << "An unknown error occurred" << std::endl;
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
