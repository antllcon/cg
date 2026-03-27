#pragma once
#include "src/core/interfaces/IGeometryFactory.h"

#include <filesystem>

class OpenglGeometryFactory final : public IGeometryFactory
{
public:
	OpenglGeometryFactory() = default;

	std::shared_ptr<IMesh> CreateCube(float size) override;
	std::shared_ptr<IMesh> CreateSphere(float radius, uint32_t segments, uint32_t rings) override;
	std::shared_ptr<IMesh> CreatePlane(float width, float depth) override;
	std::shared_ptr<IMesh> CreateCylinder(float baseRadius, float topRadius, float height, uint32_t sectors) override;
	std::shared_ptr<IMesh> CreateFromObj(const std::filesystem::path& path) override;
};