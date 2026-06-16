#include "AsyncFile.h"
#include "Dispatcher.h"
#include <stdexcept>
#include <utility>

namespace
{
void AssertIsSizeValid(bool isValid)
{
	if (!isValid)
	{
		throw std::runtime_error("Размер буфера превышает допустимый предел операции");
	}
}

void AssertIsFileOpened(bool isOpened)
{
	if (!isOpened)
	{
		throw std::runtime_error("Не удалось открыть файл");
	}
}

void AssertIsReadStarted(bool isStarted)
{
	if (!isStarted)
	{
		throw std::runtime_error("Не удалось запустить асинхронное чтение");
	}
}

void AssertIsWriteStarted(bool isStarted)
{
	if (!isStarted)
	{
		throw std::runtime_error("Не удалось запустить асинхронную запись");
	}
}

void AssertIsOperationSucceeded(DWORD errorCode)
{
	if (errorCode != ERROR_SUCCESS)
	{
		throw std::runtime_error("Асинхронная операция завершилась с ошибкой");
	}
}

void AssertIsFullyWritten(bool isFull)
{
	if (!isFull)
	{
		throw std::runtime_error("Записан не весь запрошенный объём данных");
	}
}

void PrepareOperation(IoOperation& operation, ULONGLONG offset, std::coroutine_handle<> continuation) noexcept
{
	operation.overlapped = OVERLAPPED{};
	operation.overlapped.Offset = static_cast<DWORD>(offset & 0xFFFFFFFFull);
	operation.overlapped.OffsetHigh = static_cast<DWORD>(offset >> 32);
	operation.continuation = continuation;
	operation.bytesTransferred = 0;
	operation.errorCode = ERROR_SUCCESS;
}

DWORD ToTransferSize(std::size_t size)
{
	AssertIsSizeValid(size <= 0xFFFFFFFFull);
	return static_cast<DWORD>(size);
}
} // namespace

ReadAwaiter::ReadAwaiter(Dispatcher& dispatcher, void* fileHandle, ULONGLONG* position, void* buffer, DWORD size) noexcept
	: m_dispatcher(dispatcher)
	, m_fileHandle(fileHandle)
	, m_position(position)
	, m_offset(*position)
	, m_buffer(buffer)
	, m_size(size)
	, m_operation()
{
}

bool ReadAwaiter::await_ready() const noexcept
{
	return false;
}

bool ReadAwaiter::await_suspend(std::coroutine_handle<> continuation)
{
	PrepareOperation(m_operation, m_offset, continuation);

	BOOL isStarted = ReadFile(m_fileHandle, m_buffer, m_size, nullptr, &m_operation.overlapped);
	if (isStarted == FALSE)
	{
		DWORD error = GetLastError();
		if (error == ERROR_IO_PENDING)
		{
			m_dispatcher.RegisterPendingOperation();
			return true;
		}
		if (error == ERROR_HANDLE_EOF)
		{
			m_operation.bytesTransferred = 0;
			return false;
		}
		AssertIsReadStarted(false);
	}

	m_dispatcher.RegisterPendingOperation();
	return true;
}

DWORD ReadAwaiter::await_resume() const
{
	AssertIsOperationSucceeded(m_operation.errorCode);
	*m_position += m_operation.bytesTransferred;
	return m_operation.bytesTransferred;
}

WriteAwaiter::WriteAwaiter(Dispatcher& dispatcher, void* fileHandle, ULONGLONG* position, const void* buffer, DWORD size) noexcept
	: m_dispatcher(dispatcher)
	, m_fileHandle(fileHandle)
	, m_position(position)
	, m_offset(*position)
	, m_buffer(buffer)
	, m_size(size)
	, m_operation()
{
}

bool WriteAwaiter::await_ready() const noexcept
{
	return false;
}

bool WriteAwaiter::await_suspend(std::coroutine_handle<> continuation)
{
	PrepareOperation(m_operation, m_offset, continuation);

	BOOL isStarted = WriteFile(m_fileHandle, m_buffer, m_size, nullptr, &m_operation.overlapped);
	if (isStarted == FALSE)
	{
		DWORD error = GetLastError();
		if (error == ERROR_IO_PENDING)
		{
			m_dispatcher.RegisterPendingOperation();
			return true;
		}
		AssertIsWriteStarted(false);
	}

	m_dispatcher.RegisterPendingOperation();
	return true;
}

void WriteAwaiter::await_resume() const
{
	AssertIsOperationSucceeded(m_operation.errorCode);
	AssertIsFullyWritten(m_operation.bytesTransferred == m_size);
	*m_position += m_operation.bytesTransferred;
}

OpenAwaiter::OpenAwaiter(Dispatcher& dispatcher, std::string path, OpenMode mode) noexcept
	: m_dispatcher(dispatcher)
	, m_path(std::move(path))
	, m_mode(mode)
{
}

bool OpenAwaiter::await_ready() const noexcept
{
	return true;
}

bool OpenAwaiter::await_suspend(std::coroutine_handle<>) const noexcept
{
	return false;
}

AsyncFile OpenAwaiter::await_resume() const
{
	DWORD access = (m_mode == OpenMode::Read) ? GENERIC_READ : GENERIC_WRITE;
	DWORD share = (m_mode == OpenMode::Read) ? FILE_SHARE_READ : 0;
	DWORD creation = (m_mode == OpenMode::Read) ? OPEN_EXISTING : CREATE_ALWAYS;

	HANDLE handle = CreateFileA(m_path.c_str(), access, share, nullptr, creation, FILE_FLAG_OVERLAPPED, nullptr);
	AssertIsFileOpened(handle != INVALID_HANDLE_VALUE);

	m_dispatcher.AssociateDevice(handle);
	return AsyncFile(handle);
}

AsyncFile::AsyncFile(void* fileHandle) noexcept
	: m_fileHandle(fileHandle)
	, m_position(0)
{
}

AsyncFile::~AsyncFile()
{
	if (m_fileHandle != nullptr && m_fileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_fileHandle);
	}
}

AsyncFile::AsyncFile(AsyncFile&& other) noexcept
	: m_fileHandle(std::exchange(other.m_fileHandle, INVALID_HANDLE_VALUE))
	, m_position(std::exchange(other.m_position, 0))
{
}

AsyncFile& AsyncFile::operator=(AsyncFile&& other) noexcept
{
	if (this != &other)
	{
		if (m_fileHandle != nullptr && m_fileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_fileHandle);
		}
		m_fileHandle = std::exchange(other.m_fileHandle, INVALID_HANDLE_VALUE);
		m_position = std::exchange(other.m_position, 0);
	}
	return *this;
}

ReadAwaiter AsyncFile::ReadAsync(Dispatcher& dispatcher, void* buffer, std::size_t size)
{
	return ReadAwaiter(dispatcher, m_fileHandle, &m_position, buffer, ToTransferSize(size));
}

WriteAwaiter AsyncFile::WriteAsync(Dispatcher& dispatcher, const void* buffer, std::size_t size)
{
	return WriteAwaiter(dispatcher, m_fileHandle, &m_position, buffer, ToTransferSize(size));
}

OpenAwaiter AsyncOpenFile(Dispatcher& dispatcher, std::string path, OpenMode mode)
{
	return OpenAwaiter(dispatcher, std::move(path), mode);
}