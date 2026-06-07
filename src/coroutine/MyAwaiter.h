#pragma once
#include <coroutine>

struct MyAwaiter
{
	int m_x;
	int m_y;

	bool await_ready() const noexcept;
	void await_suspend(std::coroutine_handle<>) const noexcept;
	int await_resume() const noexcept;
};
