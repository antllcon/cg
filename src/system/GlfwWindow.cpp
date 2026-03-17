#include "GlfwWindow.h"
#include "AppConfig.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#endif

namespace
{
void AssertIsGlfwInitialized(int result)
{
	if (result != GLFW_TRUE)
	{
		throw std::runtime_error("Не удалось инициализировать GLFW");
	}
}

void AssertIsWindowCreated(const GLFWwindow* window)
{
	if (window == nullptr)
	{
		throw std::runtime_error("Не удалось создать окно GLFW");
	}
}

GlfwWindow* GetSelf(GLFWwindow* window)
{
	return static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
}

void WindowCloseCallback(GLFWwindow* window)
{
	Event event;
	event.type = EventType::Closed;
	GetSelf(window)->PushEvent(event);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Event event;
	event.type = EventType::Resized;
	event.size.width = width;
	event.size.height = height;
	GetSelf(window)->PushEvent(event);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;

	Event event;
	event.type = action == GLFW_PRESS || action == GLFW_REPEAT ? EventType::KeyPressed : EventType::KeyReleased;

	if (key == GLFW_KEY_T)
	{
		event.key.code = KeyCode::T;
	}
	else if (key == GLFW_KEY_ESCAPE)
	{
		event.key.code = KeyCode::Escape;
	}
	else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT)
	{
		event.key.code = KeyCode::LShift;
	}
	else if (key == GLFW_KEY_UP)
	{
		event.key.code = KeyCode::Up;
	}
	else if (key == GLFW_KEY_LEFT)
	{
		event.key.code = KeyCode::Left;
	}
	else if (key == GLFW_KEY_RIGHT)
	{
		event.key.code = KeyCode::Right;
	}
	else
	{
		event.key.code = KeyCode::Unknown;
	}

	GetSelf(window)->PushEvent(event);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	(void)mods;

	Event event;
	event.type = action == GLFW_PRESS ? EventType::MouseButtonPressed : EventType::MouseButtonReleased;

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	event.mouseButton.position = {static_cast<int>(xpos), static_cast<int>(ypos)};

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		event.mouseButton.button = MouseButton::Left;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		event.mouseButton.button = MouseButton::Right;
	}
	else
	{
		event.mouseButton.button = MouseButton::Unknown;
	}

	GetSelf(window)->PushEvent(event);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)xoffset;

	Event event;
	event.type = EventType::MouseWheelScrolled;

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	event.mouseScroll.position = {static_cast<int>(xpos), static_cast<int>(ypos)};
	event.mouseScroll.delta = static_cast<float>(yoffset);
	event.mouseScroll.isShiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

	GetSelf(window)->PushEvent(event);
}
} // namespace

GlfwWindow::GlfwWindow()
	: m_window(nullptr)
{
	AssertIsGlfwInitialized(glfwInit());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(
		AppConfig::WINDOW_WIDTH,
		AppConfig::WINDOW_HEIGHT,
		AppConfig::WINDOW_NAME,
		nullptr,
		nullptr);

	AssertIsWindowCreated(m_window);

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	glfwSetWindowCloseCallback(m_window, WindowCloseCallback);
	glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
	glfwSetKeyCallback(m_window, KeyCallback);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
	glfwSetScrollCallback(m_window, ScrollCallback);
}

GlfwWindow::~GlfwWindow()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}
	glfwTerminate();
}

bool GlfwWindow::IsOpen() const
{
	return !glfwWindowShouldClose(m_window);
}

void GlfwWindow::Close()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

bool GlfwWindow::PollEvent(Event& event)
{
	if (m_events.empty())
	{
		glfwPollEvents();
	}

	if (m_events.empty())
	{
		return false;
	}

	event = m_events.front();
	m_events.pop();

	return true;
}

Point2i GlfwWindow::GetSize() const
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	return {width, height};
}

void GlfwWindow::SetTitleBarTheme(bool isDark)
{
#ifdef _WIN32
	HWND hwnd = glfwGetWin32Window(m_window);
	BOOL useDarkMode = isDark ? TRUE : FALSE;
	DwmSetWindowAttribute(hwnd, 20, &useDarkMode, sizeof(useDarkMode));
#else
	(void)isDark;
#endif
}

void GlfwWindow::SetIconColor(const Color& color)
{
	uint32_t size = AppConfig::ICON_SIZE;
	std::vector<uint8_t> pixels(size * size * 4);

	float center = size / 2.0f;
	float outerRadius = size / 2.2f;
	float innerRadius = outerRadius / 2.0f;

	for (uint32_t y = 0; y < size; ++y)
	{
		for (uint32_t x = 0; x < size; ++x)
		{
			float dx = x - center;
			float dy = y - center;
			float distance = std::sqrt(dx * dx + dy * dy);

			size_t index = (y * size + x) * 4;

			if (distance <= outerRadius && distance >= innerRadius)
			{
				pixels[index] = color.r;
				pixels[index + 1] = color.g;
				pixels[index + 2] = color.b;
				pixels[index + 3] = color.a;
			}
			else
			{
				pixels[index] = 0;
				pixels[index + 1] = 0;
				pixels[index + 2] = 0;
				pixels[index + 3] = 0;
			}
		}
	}

	GLFWimage image;
	image.width = size;
	image.height = size;
	image.pixels = pixels.data();

	glfwSetWindowIcon(m_window, 1, &image);
}

void GlfwWindow::PushEvent(const Event& event)
{
	m_events.push(event);
}