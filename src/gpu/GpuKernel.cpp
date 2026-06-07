#include "GpuKernel.h"
#include <stdexcept>

GpuKernel::GpuKernel(const GpuProgram& program, const std::string& name)
{
	cl_int err;
	m_handle = clCreateKernel(program.GetHandle(), name.c_str(), &err);
	if (err != CL_SUCCESS)
		throw std::runtime_error("Не удалось создать OpenCL ядро: " + name);
}

GpuKernel::~GpuKernel()
{
	if (m_handle)
		clReleaseKernel(m_handle);
}

void GpuKernel::Dispatch(
	const GpuContext& ctx,
	size_t globalSize,
	size_t localSize)
{
	cl_int err = clEnqueueNDRangeKernel(
		ctx.GetQueue(),
		m_handle,
		1,
		nullptr,
		&globalSize,
		&localSize,
		0, nullptr, nullptr);

	if (err != CL_SUCCESS)
		throw std::runtime_error("Не удалось поставить в очередь OpenCL ядро");
}

void GpuKernel::AssertIsArgSet(cl_int err) const
{
	if (err != CL_SUCCESS)
		throw std::runtime_error("Не удалось установить аргумент OpenCL ядра");
}
