#pragma once

#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>

class GpuContext
{
public:
	GpuContext();
	~GpuContext();

	GpuContext(const GpuContext&) = delete;
	GpuContext& operator=(const GpuContext&) = delete;

	cl_context GetContext() const noexcept;
	cl_command_queue GetQueue() const noexcept;
	cl_device_id GetDevice() const noexcept;

	void Finish() const;

private:
	cl_platform_id m_platform = nullptr;
	cl_device_id m_device = nullptr;
	cl_context m_context = nullptr;
	cl_command_queue m_queue = nullptr;
};