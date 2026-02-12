#include "Windows.h"
#include "system/Application.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	Application app;
	app.Run();

	return EXIT_SUCCESS;
}