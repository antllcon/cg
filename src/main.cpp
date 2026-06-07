#include "Windows.h"
#include "book/BookChapterGenerator.h"
#include "utils/console/ConsoleEncoding.h"
#include <iostream>
#include <vector>

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
		std::vector<Book> books = {
			{"The Great Gatsby", "F. Scott Fitzgerald", {"Chapter 1", "Chapter 2"}},
			{"1984", "George Orwell", {"Chapter 1", "Chapter 2", "Chapter 3"}},
			{"To Kill a Mockingbird", "Harper Lee", {"Chapter 1"}},
		};

		for (const auto& chapter : ListBookChapters(books))
		{
			std::cout << chapter << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Error]\t" << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
