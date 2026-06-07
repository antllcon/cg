#include "GpuProgram.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace
{
std::string ReadFile(const std::filesystem::path& path)
{
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Не удалось открыть файл ядра: " + path.string());

	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

void AssertIsBuildSuccess(cl_int err, cl_program program, cl_device_id device)
{
	if (err == CL_SUCCESS)
		return;

	size_t logSize = 0;
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);

	std::vector<char> log(logSize);
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, log.data(), nullptr);

	throw std::runtime_error("Ошибка компиляции OpenCL ядра:\n" + std::string(log.data()));
}
} // namespace

GpuProgram::GpuProgram(const GpuContext& ctx, const std::filesystem::path& path)
{
	std::string source = ReadFile(path);
	const char* src = source.c_str();
	size_t srcLen = source.size();

	cl_int err;
	m_handle = clCreateProgramWithSource(ctx.GetContext(), 1, &src, &srcLen, &err);
	if (err != CL_SUCCESS)
		throw std::runtime_error("Не удалось создать OpenCL программу");

	cl_device_id device = ctx.GetDevice();
	err = clBuildProgram(m_handle, 1, &device, nullptr, nullptr, nullptr);
	AssertIsBuildSuccess(err, m_handle, device);
}

GpuProgram::~GpuProgram()
{
	if (m_handle)
		clReleaseProgram(m_handle);
}

cl_program GpuProgram::GetHandle() const noexcept
{
	return m_handle;
}