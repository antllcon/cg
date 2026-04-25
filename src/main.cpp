#include "Windows.h"
#include "system/application/Application.h"
#include "system/managers/AudioManager.h"
#include "system/render/opengl/OpenglRenderer.h"
#include "system/window/GlfwWindow.h"
#include <iostream>
#include <memory>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	try
	{
		auto window = std::make_unique<GlfwWindow>();
		auto renderer = std::make_unique<OpenglRenderer>();
		auto audio = std::make_unique<AudioManager>();

		auto app = std::make_shared<Application>(
			std::move(window),
			std::move(renderer),
			std::move(audio));

		app->Init();
		app->Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Критическая ошибка: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}