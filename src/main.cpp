#include "Windows.h"
#include "system/application/Application.h"
#include "utils/console/ConsoleEncoding.h"
#include <iostream>

#ifdef _WIN32
extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main()
{
	ConsoleEncoding encoding;

	try
	{
		Application app;
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Error] \t" << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}