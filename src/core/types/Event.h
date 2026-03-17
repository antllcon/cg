#pragma once
#include "Point.h"

enum class MouseButton
{
	Left,
	Right,
	Middle,
	Unknown
};

enum class KeyCode
{
	T,
	LShift,
	Escape,
	Enter,
	Space,
	Unknown,
	Left,
	Up,
	Down,
	Right,
};

enum class EventType
{
	None,
	Closed,
	Resized,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseWheelScrolled,
	KeyPressed,
	KeyReleased
};

struct Event
{
	EventType type;

	union
	{
		struct
		{
			Point2i position;
			MouseButton button;
		} mouseButton;

		struct
		{
			Point2i position;
			float delta;
			bool isShiftPressed;
		} mouseScroll;

		struct
		{
			Point2i position;
		} mouseMove;

		struct
		{
			KeyCode code;
		} key;

		struct
		{
			int width;
			int height;
		} size;
	};

	Event()
		: type(EventType::None)
		, size{0, 0}
	{
	}
};