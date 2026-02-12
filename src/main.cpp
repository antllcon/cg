#include <iostream>

int main()
{
	try
	{
		std::cout << "CG course!" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}