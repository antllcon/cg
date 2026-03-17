#include "Windows.h"
#include "system/Application.h"
#include "system/GlfwWindow.h"
#include "system/OpenglRenderer.h"

#include <memory>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	auto window = std::make_unique<GlfwWindow>();
	auto renderer = std::make_unique<OpenglRenderer>();

	auto app = std::make_shared<Application>(std::move(window), std::move(renderer));

	app->Init();
	app->Run();

	return EXIT_SUCCESS;
}