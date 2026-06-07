#include "BookChapterGenerator.h"
#include <ranges>

namespace
{
	std::generator<BookChapter> ListChapters(const Book& book)
	{
		for (const auto& chapterTitle : book.chapters)
		{
			co_yield BookChapter{
				.bookTitle = book.title,
				.bookAuthor = book.author,
				.chapterTitle = chapterTitle,
			};
		}
	}
}

std::generator<BookChapter> ListBookChapters(const std::vector<Book>& books)
{
	for (const auto& book : books)
	{
		co_yield std::ranges::elements_of(ListChapters(book));
	}
}
