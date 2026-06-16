#pragma once
#include <coroutine>
#include <exception>

class Task
{
public:
	struct promise_type;
	using Handle = std::coroutine_handle<promise_type>;

	struct FinalAwaiter
	{
		bool await_ready() const noexcept;
		std::coroutine_handle<> await_suspend(Handle handle) const noexcept;
		void await_resume() const noexcept;
	};

	struct promise_type
	{
		std::coroutine_handle<> m_continuation;
		std::exception_ptr m_exception;

		Task get_return_object() noexcept;
		std::suspend_never initial_suspend() noexcept;
		FinalAwaiter final_suspend() noexcept;
		void return_void() noexcept;
		void unhandled_exception() noexcept;
	};

	explicit Task(Handle handle) noexcept;
	~Task();

	Task(const Task&) = delete;
	Task& operator=(const Task&) = delete;
	Task(Task&& other) noexcept;
	Task& operator=(Task&& other) noexcept;

	bool await_ready() const noexcept;
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) noexcept;
	void await_resume() const;

private:
	Handle m_handle;
};