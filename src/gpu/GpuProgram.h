#pragma once

#define CL_TARGET_OPENCL_VERSION 120
#include "GpuContext.h"
#include <CL/cl.h>
#include <filesystem>

class GpuProgram
{
public:
	GpuProgram(const GpuContext& ctx, const std::filesystem::path& path);
	~GpuProgram();

	GpuProgram(const GpuProgram&) = delete;
	GpuProgram& operator=(const GpuProgram&) = delete;

	cl_program GetHandle() const noexcept;

private:
	cl_program m_handle = nullptr;
};