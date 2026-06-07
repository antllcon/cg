#pragma once
#include <ostream>
#include <string>
#include <vector>

struct Book
{
	std::string title;
	std::string author;
	std::vector<std::string> chapters;
};

struct BookChapter
{
	std::string bookTitle;
	std::string bookAuthor;
	std::string chapterTitle;
};

std::ostream& operator<<(std::ostream& os, const BookChapter& chapter);
