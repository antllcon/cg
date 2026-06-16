#pragma once
#include <cstddef>

class Dispatcher
{
public:
	Dispatcher();
	~Dispatcher();

	Dispatcher(const Dispatcher&) = delete;
	Dispatcher& operator=(const Dispatcher&) = delete;
	Dispatcher(Dispatcher&&) = delete;
	Dispatcher& operator=(Dispatcher&&) = delete;

	void AssociateDevice(void* fileHandle);
	void RegisterPendingOperation() noexcept;
	void Run();

private:
	void* m_completionPort;
	std::size_t m_pendingOperations;
};