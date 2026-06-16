#pragma once
#include "IoOperation.h"
#include "OpenMode.h"
#include <coroutine>
#include <string>

class Dispatcher;
class AsyncFile;

class ReadAwaiter
{
public:
	ReadAwaiter(Dispatcher& dispatcher, void* fileHandle, ULONGLONG* position, void* buffer, DWORD size) noexcept;

	bool await_ready() const noexcept;
	bool await_suspend(std::coroutine_handle<> continuation);
	DWORD await_resume() const;

private:
	Dispatcher& m_dispatcher;
	void* m_fileHandle;
	ULONGLONG* m_position;
	ULONGLONG m_offset;
	void* m_buffer;
	DWORD m_size;
	IoOperation m_operation;
};

class WriteAwaiter
{
public:
	WriteAwaiter(Dispatcher& dispatcher, void* fileHandle, ULONGLONG* position, const void* buffer, DWORD size) noexcept;

	bool await_ready() const noexcept;
	bool await_suspend(std::coroutine_handle<> continuation);
	void await_resume() const;

private:
	Dispatcher& m_dispatcher;
	void* m_fileHandle;
	ULONGLONG* m_position;
	ULONGLONG m_offset;
	const void* m_buffer;
	DWORD m_size;
	IoOperation m_operation;
};

class OpenAwaiter
{
public:
	OpenAwaiter(Dispatcher& dispatcher, std::string path, OpenMode mode) noexcept;

	bool await_ready() const noexcept;
	bool await_suspend(std::coroutine_handle<>) const noexcept;
	AsyncFile await_resume() const;

private:
	Dispatcher& m_dispatcher;
	std::string m_path;
	OpenMode m_mode;
};

class AsyncFile
{
public:
	explicit AsyncFile(void* fileHandle) noexcept;
	~AsyncFile();

	AsyncFile(const AsyncFile&) = delete;
	AsyncFile& operator=(const AsyncFile&) = delete;
	AsyncFile(AsyncFile&& other) noexcept;
	AsyncFile& operator=(AsyncFile&& other) noexcept;

	ReadAwaiter ReadAsync(Dispatcher& dispatcher, void* buffer, std::size_t size);
	WriteAwaiter WriteAsync(Dispatcher& dispatcher, const void* buffer, std::size_t size);

private:
	void* m_fileHandle;
	ULONGLONG m_position;
};

OpenAwaiter AsyncOpenFile(Dispatcher& dispatcher, std::string path, OpenMode mode);