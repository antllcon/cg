#include "MyAwaiter.h"

bool MyAwaiter::await_ready() const noexcept
{
	return false;
}

void MyAwaiter::await_suspend(std::coroutine_handle<>) const noexcept
{
}

int MyAwaiter::await_resume() const noexcept
{
	return m_x + m_y;
}
