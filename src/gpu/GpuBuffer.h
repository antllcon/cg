#pragma once

#define CL_TARGET_OPENCL_VERSION 120
#include "GpuContext.h"
#include <CL/cl.h>
#include <stdexcept>
#include <vector>

template<typename T>
class GpuBuffer
{
public:
	GpuBuffer(const GpuContext& ctx, size_t count)
		: m_count(count)
	{
		cl_int err;
		m_handle = clCreateBuffer(
			ctx.GetContext(),
			CL_MEM_READ_WRITE,
			count * sizeof(T),
			nullptr,
			&err);
		AssertIsCreated(err);
	}

	GpuBuffer(const GpuContext& ctx, const std::vector<T>& data)
		: m_count(data.size())
	{
		cl_int err;
		m_handle = clCreateBuffer(
			ctx.GetContext(),
			CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
			data.size() * sizeof(T),
			const_cast<T*>(data.data()),
			&err);
		AssertIsCreated(err);
	}

	~GpuBuffer()
	{
		if (m_handle)
			clReleaseMemObject(m_handle);
	}

	GpuBuffer(const GpuBuffer&) = delete;
	GpuBuffer& operator=(const GpuBuffer&) = delete;

	GpuBuffer(GpuBuffer&& other) noexcept
		: m_handle(other.m_handle)
		, m_count(other.m_count)
	{
		other.m_handle = nullptr;
	}

	GpuBuffer& operator=(GpuBuffer&& other) noexcept
	{
		if (this != &other)
		{
			if (m_handle)
				clReleaseMemObject(m_handle);
			m_handle = other.m_handle;
			m_count = other.m_count;
			other.m_handle = nullptr;
		}
		return *this;
	}

	void Upload(const GpuContext& ctx, const std::vector<T>& data)
	{
		cl_int err = clEnqueueWriteBuffer(
			ctx.GetQueue(), m_handle, CL_TRUE,
			0, data.size() * sizeof(T), data.data(),
			0, nullptr, nullptr);
		AssertIsTransferred(err);
	}

	void Download(const GpuContext& ctx, std::vector<T>& out) const
	{
		out.resize(m_count);
		cl_int err = clEnqueueReadBuffer(
			ctx.GetQueue(), m_handle, CL_TRUE,
			0, m_count * sizeof(T), out.data(),
			0, nullptr, nullptr);
		AssertIsTransferred(err);
	}

	cl_mem GetHandle() const noexcept { return m_handle; }
	size_t GetCount() const noexcept { return m_count; }

private:
	static void AssertIsCreated(cl_int err)
	{
		if (err != CL_SUCCESS)
			throw std::runtime_error("Не удалось создать GPU буфер");
	}

	static void AssertIsTransferred(cl_int err)
	{
		if (err != CL_SUCCESS)
			throw std::runtime_error("Ошибка передачи данных GPU буфера");
	}

	cl_mem m_handle = nullptr;
	size_t m_count = 0;
};