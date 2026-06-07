#include "Windows.h"
#include "coroutine/MyAwaiter.h"
#include "coroutine/MyTask.h"
#include "utils/console/ConsoleEncoding.h"
#include <iostream>

MyTask CoroutineWithAwait(int x, int y)
{
	std::cout << "Before await" << std::endl;
	int result = co_await MyAwaiter{ x, y };
	std::cout << result << std::endl;
	std::cout << "After await" << std::endl;
}

int main()
{
	ConsoleEncoding encoding;

	try
	{
		auto task = CoroutineWithAwait(30, 12);
		std::cout << "Before resume" << std::endl;
		task.Resume();
		std::cout << "After resume" << std::endl;
		CoroutineWithAwait(5, 10).Resume();
		std::cout << "End of main" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Error]\t" << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
