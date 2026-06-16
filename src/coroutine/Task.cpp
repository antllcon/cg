#include "Task.h"
#include <utility>

bool Task::FinalAwaiter::await_ready() const noexcept
{
	return false;
}

std::coroutine_handle<> Task::FinalAwaiter::await_suspend(Handle handle) const noexcept
{
	std::coroutine_handle<> continuation = handle.promise().m_continuation;
	if (continuation)
	{
		return continuation;
	}
	return std::noop_coroutine();
}

void Task::FinalAwaiter::await_resume() const noexcept
{
}

Task Task::promise_type::get_return_object() noexcept
{
	return Task{ Handle::from_promise(*this) };
}

std::suspend_never Task::promise_type::initial_suspend() noexcept
{
	return {};
}

Task::FinalAwaiter Task::promise_type::final_suspend() noexcept
{
	return {};
}

void Task::promise_type::return_void() noexcept
{
}

void Task::promise_type::unhandled_exception() noexcept
{
	m_exception = std::current_exception();
}

Task::Task(Handle handle) noexcept
	: m_handle(handle)
{
}

Task::~Task()
{
	if (m_handle)
	{
		m_handle.destroy();
	}
}

Task::Task(Task&& other) noexcept
	: m_handle(std::exchange(other.m_handle, {}))
{
}

Task& Task::operator=(Task&& other) noexcept
{
	if (this != &other)
	{
		if (m_handle)
		{
			m_handle.destroy();
		}
		m_handle = std::exchange(other.m_handle, {});
	}
	return *this;
}

bool Task::await_ready() const noexcept
{
	return !m_handle || m_handle.done();
}

std::coroutine_handle<> Task::await_suspend(std::coroutine_handle<> awaiting) noexcept
{
	m_handle.promise().m_continuation = awaiting;
	return std::noop_coroutine();
}

void Task::await_resume() const
{
	if (m_handle.promise().m_exception)
	{
		std::rethrow_exception(m_handle.promise().m_exception);
	}
}