#include "Windows.h"
#include "coroutine/MyTask.h"
#include "utils/console/ConsoleEncoding.h"
#include <iostream>

MyTask SimpleCoroutine()
{
	co_return "Hello from coroutine!";
}

int main()
{
	ConsoleEncoding encoding;

	try
	{
		MyTask task = SimpleCoroutine();
		std::cout << task.GetResult() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Error]\t" << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
