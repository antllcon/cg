#pragma once
#include <memory>
#include <filesystem>

class IMesh;

class IGeometryFactory
{
public:
	virtual ~IGeometryFactory() = default;

	virtual std::shared_ptr<IMesh> CreateCube(float size) = 0;
	virtual std::shared_ptr<IMesh> CreateSphere(float radius, uint32_t segments, uint32_t rings) = 0;
	virtual std::shared_ptr<IMesh> CreatePlane(float width, float depth) = 0;
	virtual std::shared_ptr<IMesh> CreateCylinder(float baseRadius, float topRadius, float height, uint32_t sectors) = 0;
	virtual std::shared_ptr<IMesh> CreateFromObj(const std::filesystem::path& path) = 0;
};