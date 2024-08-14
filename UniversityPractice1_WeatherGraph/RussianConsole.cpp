#include"RussianConsole.h"
#include"Windows.h"

void EnableRussianConsole() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}