#pragma once
#include "Book.h"
#include <generator>
#include <vector>

std::generator<BookChapter> ListBookChapters(const std::vector<Book>& books);
