#include "MyTask.h"
#include <stdexcept>
#include <utility>

namespace
{
	void AssertIsHandleValid(bool isValid)
	{
		if (!isValid)
		{
			throw std::runtime_error("Хэндл корутины недействителен");
		}
	}

	void AssertIsExceptionAbsent(const std::exception_ptr& exception)
	{
		if (exception)
		{
			std::rethrow_exception(exception);
		}
	}
}

MyTask MyTask::promise_type::get_return_object() noexcept
{
	return MyTask{ Handle::from_promise(*this) };
}

std::suspend_never MyTask::promise_type::initial_suspend() noexcept
{
	return {};
}

std::suspend_always MyTask::promise_type::final_suspend() noexcept
{
	return {};
}

void MyTask::promise_type::return_value(std::string value) noexcept
{
	m_value = std::move(value);
}

void MyTask::promise_type::unhandled_exception() noexcept
{
	m_exception = std::current_exception();
}

MyTask::MyTask(Handle handle) noexcept
	: m_handle(handle)
{
}

MyTask::~MyTask()
{
	if (m_handle)
	{
		m_handle.destroy();
	}
}

MyTask::MyTask(MyTask&& other) noexcept
	: m_handle(std::exchange(other.m_handle, {}))
{
}

MyTask& MyTask::operator=(MyTask&& other) noexcept
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

std::string MyTask::GetResult() const
{
	AssertIsHandleValid(static_cast<bool>(m_handle));
	AssertIsExceptionAbsent(m_handle.promise().m_exception);
	return m_handle.promise().m_value;
}
