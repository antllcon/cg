#pragma once

template <typename T>
struct Point
{
	T x{0};
	T y{0};

	bool operator==(const Point&) const = default;

	template <typename U>
	Point<U> As() const
	{
		return Point<U>{static_cast<U>(x), static_cast<U>(y)};
	}

	Point operator+(const Point& other) const
	{
		return Point{x + other.x, y + other.y};
	}

	Point operator-(const Point& other) const
	{
		return Point{x - other.x, y - other.y};
	}
};

using Point2i = Point<int>;
using Point2f = Point<float>;