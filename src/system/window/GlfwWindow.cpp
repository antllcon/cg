#include "GlfwWindow.h"
#include "GLFW/glfw3.h"
#include "Windows.h"
#include "src/system/AppConfig.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "stb_image.h"
#include <GLFW/glfw3native.h>
#include <dwmapi.h>
#include <filesystem>
#include <stdexcept>

namespace
{
constexpr int GLD_VERSION_MAJOR = 3;
constexpr int GLD_VERSION_MINOR = 3;

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

void AssertIsFileExists(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("Файл иконки не найден");
	}
}

void AssertIsImageLoaded(const unsigned char* data)
{
	if (data == nullptr)
	{
		throw std::runtime_error("Не удалось декодировать изображение иконки");
	}
}

GlfwWindow* GetSelf(GLFWwindow* window)
{
	return static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
}

Point2i GetCursorPosition(GLFWwindow* window)
{
	double xpos;
	double ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return {static_cast<int>(xpos), static_cast<int>(ypos)};
}

KeyCode MapGlfwKey(int key)
{
	switch (key)
	{
	case GLFW_KEY_A:
		return KeyCode::A;
	case GLFW_KEY_B:
		return KeyCode::B;
	case GLFW_KEY_C:
		return KeyCode::C;
	case GLFW_KEY_D:
		return KeyCode::D;
	case GLFW_KEY_E:
		return KeyCode::E;
	case GLFW_KEY_F:
		return KeyCode::F;
	case GLFW_KEY_G:
		return KeyCode::G;
	case GLFW_KEY_H:
		return KeyCode::H;
	case GLFW_KEY_I:
		return KeyCode::I;
	case GLFW_KEY_J:
		return KeyCode::J;
	case GLFW_KEY_K:
		return KeyCode::K;
	case GLFW_KEY_L:
		return KeyCode::L;
	case GLFW_KEY_M:
		return KeyCode::M;
	case GLFW_KEY_N:
		return KeyCode::N;
	case GLFW_KEY_O:
		return KeyCode::O;
	case GLFW_KEY_P:
		return KeyCode::P;
	case GLFW_KEY_Q:
		return KeyCode::Q;
	case GLFW_KEY_R:
		return KeyCode::R;
	case GLFW_KEY_S:
		return KeyCode::S;
	case GLFW_KEY_T:
		return KeyCode::T;
	case GLFW_KEY_U:
		return KeyCode::U;
	case GLFW_KEY_V:
		return KeyCode::V;
	case GLFW_KEY_W:
		return KeyCode::W;
	case GLFW_KEY_X:
		return KeyCode::X;
	case GLFW_KEY_Y:
		return KeyCode::Y;
	case GLFW_KEY_Z:
		return KeyCode::Z;
	case GLFW_KEY_0:
		return KeyCode::Num0;
	case GLFW_KEY_1:
		return KeyCode::Num1;
	case GLFW_KEY_2:
		return KeyCode::Num2;
	case GLFW_KEY_3:
		return KeyCode::Num3;
	case GLFW_KEY_4:
		return KeyCode::Num4;
	case GLFW_KEY_5:
		return KeyCode::Num5;
	case GLFW_KEY_6:
		return KeyCode::Num6;
	case GLFW_KEY_7:
		return KeyCode::Num7;
	case GLFW_KEY_8:
		return KeyCode::Num8;
	case GLFW_KEY_9:
		return KeyCode::Num9;
	case GLFW_KEY_ESCAPE:
		return KeyCode::Escape;
	case GLFW_KEY_LEFT_CONTROL:
		return KeyCode::LControl;
	case GLFW_KEY_LEFT_SHIFT:
		return KeyCode::LShift;
	case GLFW_KEY_LEFT_ALT:
		return KeyCode::LAlt;
	case GLFW_KEY_LEFT_SUPER:
		return KeyCode::LSystem;
	case GLFW_KEY_RIGHT_CONTROL:
		return KeyCode::RControl;
	case GLFW_KEY_RIGHT_SHIFT:
		return KeyCode::RShift;
	case GLFW_KEY_RIGHT_ALT:
		return KeyCode::RAlt;
	case GLFW_KEY_RIGHT_SUPER:
		return KeyCode::RSystem;
	case GLFW_KEY_MENU:
		return KeyCode::Menu;
	case GLFW_KEY_LEFT_BRACKET:
		return KeyCode::LBracket;
	case GLFW_KEY_RIGHT_BRACKET:
		return KeyCode::RBracket;
	case GLFW_KEY_SEMICOLON:
		return KeyCode::Semicolon;
	case GLFW_KEY_COMMA:
		return KeyCode::Comma;
	case GLFW_KEY_PERIOD:
		return KeyCode::Period;
	case GLFW_KEY_APOSTROPHE:
		return KeyCode::Quote;
	case GLFW_KEY_SLASH:
		return KeyCode::Slash;
	case GLFW_KEY_BACKSLASH:
		return KeyCode::Backslash;
	case GLFW_KEY_GRAVE_ACCENT:
		return KeyCode::Tilde;
	case GLFW_KEY_EQUAL:
		return KeyCode::Equal;
	case GLFW_KEY_MINUS:
		return KeyCode::Hyphen;
	case GLFW_KEY_SPACE:
		return KeyCode::Space;
	case GLFW_KEY_ENTER:
		return KeyCode::Enter;
	case GLFW_KEY_BACKSPACE:
		return KeyCode::Backspace;
	case GLFW_KEY_TAB:
		return KeyCode::Tab;
	case GLFW_KEY_PAGE_UP:
		return KeyCode::PageUp;
	case GLFW_KEY_PAGE_DOWN:
		return KeyCode::PageDown;
	case GLFW_KEY_END:
		return KeyCode::End;
	case GLFW_KEY_HOME:
		return KeyCode::Home;
	case GLFW_KEY_INSERT:
		return KeyCode::Insert;
	case GLFW_KEY_DELETE:
		return KeyCode::Delete;
	case GLFW_KEY_KP_ADD:
		return KeyCode::Add;
	case GLFW_KEY_KP_SUBTRACT:
		return KeyCode::Subtract;
	case GLFW_KEY_KP_MULTIPLY:
		return KeyCode::Multiply;
	case GLFW_KEY_KP_DIVIDE:
		return KeyCode::Divide;
	case GLFW_KEY_LEFT:
		return KeyCode::Left;
	case GLFW_KEY_RIGHT:
		return KeyCode::Right;
	case GLFW_KEY_UP:
		return KeyCode::Up;
	case GLFW_KEY_DOWN:
		return KeyCode::Down;
	case GLFW_KEY_KP_0:
		return KeyCode::Numpad0;
	case GLFW_KEY_KP_1:
		return KeyCode::Numpad1;
	case GLFW_KEY_KP_2:
		return KeyCode::Numpad2;
	case GLFW_KEY_KP_3:
		return KeyCode::Numpad3;
	case GLFW_KEY_KP_4:
		return KeyCode::Numpad4;
	case GLFW_KEY_KP_5:
		return KeyCode::Numpad5;
	case GLFW_KEY_KP_6:
		return KeyCode::Numpad6;
	case GLFW_KEY_KP_7:
		return KeyCode::Numpad7;
	case GLFW_KEY_KP_8:
		return KeyCode::Numpad8;
	case GLFW_KEY_KP_9:
		return KeyCode::Numpad9;
	case GLFW_KEY_F1:
		return KeyCode::F1;
	case GLFW_KEY_F2:
		return KeyCode::F2;
	case GLFW_KEY_F3:
		return KeyCode::F3;
	case GLFW_KEY_F4:
		return KeyCode::F4;
	case GLFW_KEY_F5:
		return KeyCode::F5;
	case GLFW_KEY_F6:
		return KeyCode::F6;
	case GLFW_KEY_F7:
		return KeyCode::F7;
	case GLFW_KEY_F8:
		return KeyCode::F8;
	case GLFW_KEY_F9:
		return KeyCode::F9;
	case GLFW_KEY_F10:
		return KeyCode::F10;
	case GLFW_KEY_F11:
		return KeyCode::F11;
	case GLFW_KEY_F12:
		return KeyCode::F12;
	default:
		return KeyCode::Unknown;
	}
}
MouseButton MapGlfwButton(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		return MouseButton::Left;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return MouseButton::Right;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return MouseButton::Middle;
	default:
		return MouseButton::Unknown;
	}
}

void WindowCloseCallback(GLFWwindow* window)
{
	GetSelf(window)->PushEvent(WindowClosedEvent{});
}

void KeyCallback(GLFWwindow* window, int key, int, int action, int mods)
{
	KeyCode code = MapGlfwKey(key);
	bool isShift = (mods & GLFW_MOD_SHIFT) != 0;
	bool isCtrl = (mods & GLFW_MOD_CONTROL) != 0;
	bool isAlt = (mods & GLFW_MOD_ALT) != 0;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		GetSelf(window)->PushEvent(KeyPressedEvent{code, isShift, isCtrl, isAlt});
	}
	else if (action == GLFW_RELEASE)
	{
		GetSelf(window)->PushEvent(KeyReleasedEvent{code});
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
	MouseButton mappedButton = MapGlfwButton(button);
	Point2i position = GetCursorPosition(window);

	if (action == GLFW_PRESS)
	{
		GetSelf(window)->PushEvent(MouseButtonPressedEvent{position, mappedButton});
	}
	else if (action == GLFW_RELEASE)
	{
		GetSelf(window)->PushEvent(MouseButtonReleasedEvent{position, mappedButton});
	}
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Point2i position{static_cast<int>(xpos), static_cast<int>(ypos)};
	GetSelf(window)->PushEvent(MouseMovedEvent{position});
}

void ScrollCallback(GLFWwindow* window, double, double yoffset)
{
	Point2i position = GetCursorPosition(window);
	GetSelf(window)->PushEvent(MouseScrolledEvent{position, static_cast<float>(yoffset)});
}

void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	GetSelf(window)->PushEvent(WindowResizedEvent{width, height});
}
} // namespace

GlfwWindow::GlfwWindow()
	: m_window(nullptr)
{
	AssertIsGlfwInitialized(glfwInit());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLD_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLD_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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
	glfwSetKeyCallback(m_window, KeyCallback);
	glfwSetMouseButtonCallback(m_window, MouseButtonCallback);
	glfwSetCursorPosCallback(m_window, CursorPosCallback);
	glfwSetScrollCallback(m_window, ScrollCallback);
	glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
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

std::optional<Event> GlfwWindow::PollEvent()
{
	if (m_events.empty())
	{
		glfwPollEvents();
	}

	if (m_events.empty())
	{
		return std::nullopt;
	}

	Event event = m_events.front();
	m_events.pop();

	return event;
}

void GlfwWindow::PushEvent(const Event& event)
{
	m_events.push(event);
}

Point2i GlfwWindow::GetSize() const
{
	int width;
	int height;
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
			float dx = static_cast<float>(x) - center;
			float dy = static_cast<float>(y) - center;
			float distance = std::sqrt(dx * dx + dy * dy);

			size_t index = (y * size + x) * 4;

			if (distance <= outerRadius && distance >= innerRadius)
			{
				pixels[index] = color.GetRAsByte();
				pixels[index + 1] = color.GetGAsByte();
				pixels[index + 2] = color.GetBAsByte();
				pixels[index + 3] = color.GetAAsByte();
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
	image.width = static_cast<int>(size);
	image.height = static_cast<int>(size);
	image.pixels = pixels.data();

	glfwSetWindowIcon(m_window, 1, &image);
}

void GlfwWindow::SetIconFromFile(const std::filesystem::path& path)
{
	AssertIsFileExists(path);

	int width;
	int height;
	int channels;

	unsigned char* pixels = stbi_load(path.string().c_str(), &width, &height, &channels, 4);

	AssertIsImageLoaded(pixels);

	GLFWimage image;
	image.width = width;
	image.height = height;
	image.pixels = pixels;

	glfwSetWindowIcon(m_window, 1, &image);

	stbi_image_free(pixels);
}