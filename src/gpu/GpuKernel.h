#pragma once

#define CL_TARGET_OPENCL_VERSION 120
#include "GpuBuffer.h"
#include "GpuContext.h"
#include "GpuProgram.h"
#include <CL/cl.h>
#include <stdexcept>
#include <string>

class GpuKernel
{
public:
	GpuKernel(const GpuProgram& program, const std::string& name);
	~GpuKernel();

	GpuKernel(const GpuKernel&) = delete;
	GpuKernel& operator=(const GpuKernel&) = delete;

	template<typename T>
	void SetArg(uint32_t index, const T& value)
	{
		cl_int err = clSetKernelArg(m_handle, index, sizeof(T), &value);
		AssertIsArgSet(err);
	}

	template<typename T>
	void SetArg(uint32_t index, const GpuBuffer<T>& buffer)
	{
		cl_mem handle = buffer.GetHandle();
		cl_int err = clSetKernelArg(m_handle, index, sizeof(cl_mem), &handle);
		AssertIsArgSet(err);
	}

	void Dispatch(
		const GpuContext& ctx,
		size_t globalSize,
		size_t localSize);

private:
	void AssertIsArgSet(cl_int err) const;

	cl_kernel m_handle = nullptr;
};