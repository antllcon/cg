#pragma once
#include <coroutine>
#include <exception>

class MyTask
{
public:
	struct promise_type;
	using Handle = std::coroutine_handle<promise_type>;

	struct promise_type
	{
		std::exception_ptr m_exception;

		MyTask get_return_object() noexcept;
		std::suspend_never initial_suspend() noexcept;
		std::suspend_always final_suspend() noexcept;
		void return_void() noexcept;
		void unhandled_exception() noexcept;
	};

	explicit MyTask(Handle handle) noexcept;
	~MyTask();

	MyTask(const MyTask&) = delete;
	MyTask& operator=(const MyTask&) = delete;
	MyTask(MyTask&& other) noexcept;
	MyTask& operator=(MyTask&& other) noexcept;

	void Resume();

private:
	Handle m_handle;
};
