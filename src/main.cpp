#include "Windows.h"
#include "system/application/Application.h"
#include "utils/console/ConsoleEncoding.h"
#include <iostream>

int main()
{
	ConsoleEncoding encoding;

	try
	{
		auto app = Application();
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Error] \t" << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}