#include "Dispatcher.h"
#include "AsyncFile.h"
#include <stdexcept>

namespace
{
void AssertIsPortCreated(bool isCreated)
{
	if (!isCreated)
	{
		throw std::runtime_error("Не смог создать");
	}
}

void AssertIsDeviceAssociated(bool isAssociated)
{
	if (!isAssociated)
	{
		throw std::runtime_error("Не вышла ассоциация");
	}
}

void AssertIsPacketDequeued(bool isDequeued)
{
	if (!isDequeued)
	{
		throw std::runtime_error("Проблема с пакетами");
	}
}

IoOperation* IoOperationFromOverlapped(OVERLAPPED* overlapped) noexcept
{
	return reinterpret_cast<IoOperation*>(overlapped);
}

void ResolveCompletion(IoOperation& operation, bool isSuccess, DWORD bytesTransferred) noexcept
{
	if (isSuccess)
	{
		operation.bytesTransferred = bytesTransferred;
		operation.errorCode = ERROR_SUCCESS;
		return;
	}

	DWORD error = GetLastError();
	if (error == ERROR_HANDLE_EOF)
	{
		operation.bytesTransferred = 0;
		operation.errorCode = ERROR_SUCCESS;
		return;
	}

	operation.bytesTransferred = bytesTransferred;
	operation.errorCode = error;
}
} // namespace

Dispatcher::Dispatcher()
	: m_completionPort(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0))
	, m_pendingOperations(0)
{
	AssertIsPortCreated(m_completionPort != nullptr);
}

Dispatcher::~Dispatcher()
{
	if (m_completionPort != nullptr)
	{
		CloseHandle(m_completionPort);
	}
}

void Dispatcher::AssociateDevice(void* fileHandle)
{
	HANDLE result = CreateIoCompletionPort(fileHandle, m_completionPort, 0, 0);
	AssertIsDeviceAssociated(result != nullptr);
}

void Dispatcher::RegisterPendingOperation() noexcept
{
	++m_pendingOperations;
}

void Dispatcher::Run()
{
	while (m_pendingOperations > 0)
	{
		DWORD bytesTransferred = 0;
		ULONG_PTR completionKey = 0;
		OVERLAPPED* overlapped = nullptr;
		BOOL isDequeued = GetQueuedCompletionStatus(m_completionPort, &bytesTransferred, &completionKey, &overlapped, INFINITE);

		AssertIsPacketDequeued(overlapped != nullptr);
		--m_pendingOperations;

		IoOperation* operation = IoOperationFromOverlapped(overlapped);
		ResolveCompletion(*operation, isDequeued != FALSE, bytesTransferred);
		operation->continuation.resume();
	}
}