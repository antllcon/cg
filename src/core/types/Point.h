#pragma once

template <typename T>
struct Point
{
	T x{0};
	T y{0};

	bool operator==(const Point&) const = default;
};

using Point2i = Point<int>;
using Point2f = Point<float>;