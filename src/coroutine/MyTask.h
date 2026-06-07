#pragma once
#include <coroutine>
#include <exception>
#include <string>

class MyTask
{
public:
	struct promise_type
	{
		std::string m_value;
		std::exception_ptr m_exception;

		MyTask get_return_object() noexcept;
		std::suspend_never initial_suspend() noexcept;
		std::suspend_always final_suspend() noexcept;
		void return_value(std::string value) noexcept;
		void unhandled_exception() noexcept;
	};

	using Handle = std::coroutine_handle<promise_type>;

	explicit MyTask(Handle handle) noexcept;
	~MyTask();

	MyTask(const MyTask&) = delete;
	MyTask& operator=(const MyTask&) = delete;
	MyTask(MyTask&& other) noexcept;
	MyTask& operator=(MyTask&& other) noexcept;

	std::string GetResult() const;

private:
	Handle m_handle;
};
