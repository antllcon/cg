#include "GpuContext.h"
#include <stdexcept>
#include <vector>

namespace
{
void AssertIsSuccess(cl_int err, const char* msg)
{
	if (err != CL_SUCCESS)
		throw std::runtime_error(msg);
}

cl_platform_id SelectPlatform()
{
	cl_uint count = 0;
	clGetPlatformIDs(0, nullptr, &count);
	if (count == 0)
		throw std::runtime_error("Платформы OpenCL не найдены");

	std::vector<cl_platform_id> platforms(count);
	clGetPlatformIDs(count, platforms.data(), nullptr);
	return platforms[0];
}

cl_device_id SelectDevice(cl_platform_id platform)
{
	cl_uint count = 0;
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &count);
	if (count == 0)
		throw std::runtime_error("GPU устройства OpenCL не найдены");

	std::vector<cl_device_id> devices(count);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, count, devices.data(), nullptr);
	return devices[0];
}
} // namespace

GpuContext::GpuContext()
{
	m_platform = SelectPlatform();
	m_device = SelectDevice(m_platform);

	cl_int err;
	m_context = clCreateContext(nullptr, 1, &m_device, nullptr, nullptr, &err);
	AssertIsSuccess(err, "Не удалось создать контекст OpenCL");

	m_queue = clCreateCommandQueue(m_context, m_device, 0, &err);
	AssertIsSuccess(err, "Не удалось создать очередь команд OpenCL");
}

GpuContext::~GpuContext()
{
	if (m_queue)
		clReleaseCommandQueue(m_queue);
	if (m_context)
		clReleaseContext(m_context);
}

cl_context GpuContext::GetContext() const noexcept
{
	return m_context;
}

cl_command_queue GpuContext::GetQueue() const noexcept
{
	return m_queue;
}

cl_device_id GpuContext::GetDevice() const noexcept
{
	return m_device;
}

void GpuContext::Finish() const
{
	clFinish(m_queue);
}