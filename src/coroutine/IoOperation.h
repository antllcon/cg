#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <coroutine>

struct IoOperation
{
	OVERLAPPED overlapped{};
	std::coroutine_handle<> continuation{};
	DWORD bytesTransferred{};
	DWORD errorCode{};
};