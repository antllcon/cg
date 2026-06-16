#include "coroutine/AsyncFile.h"
#include "coroutine/Dispatcher.h"
#include "coroutine/Task.h"
#include "utils/console/ConsoleEncoding.h"
#include <iostream>
#include <string>
#include <vector>

namespace
{
constexpr std::size_t BufferSize = 1024;
}

Task AsyncCopyFile(Dispatcher& dispatcher, std::string from, std::string to)
{
	AsyncFile input = co_await AsyncOpenFile(dispatcher, std::move(from), OpenMode::Read);
	AsyncFile output = co_await AsyncOpenFile(dispatcher, std::move(to), OpenMode::Write);

	std::vector<char> buffer(BufferSize);

	for (DWORD bytesRead;
		(bytesRead = co_await input.ReadAsync(dispatcher, buffer.data(), buffer.size())) != 0;)
	{
		co_await output.WriteAsync(dispatcher, buffer.data(), bytesRead);
	}
}

Task AsyncCopyTwoFiles(Dispatcher& dispatcher)
{
	auto firstCopy = AsyncCopyFile(dispatcher, "a.in", "a.out");
	auto secondCopy = AsyncCopyFile(dispatcher, "b.in", "b.out");
	co_await firstCopy;
	co_await secondCopy;
}

int main()
{
	ConsoleEncoding encoding;

	try
	{
		Dispatcher dispatcher;
		Task task = AsyncCopyTwoFiles(dispatcher);
		dispatcher.Run();
		std::cout << "Copy finished" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Error]\t" << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}