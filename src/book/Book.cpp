#include "Book.h"

std::ostream& operator<<(std::ostream& os, const BookChapter& chapter)
{
	os << "\"" << chapter.bookTitle << "\" (" << chapter.bookAuthor << "): " << chapter.chapterTitle;
	return os;
}
