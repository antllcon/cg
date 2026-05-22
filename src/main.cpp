#include "Windows.h"
#include "system/application/Application.h"
#include "utils/console/ConsoleEncoding.h"
#include <iostream>

extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

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