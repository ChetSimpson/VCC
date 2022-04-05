#include "MonitorPak.h"
#include <windows.h>
#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>


std::array<u_char, 16> MMURegisterMirror = { 0 };



static HANDLE GetConsoleHandle()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (consoleHandle == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error("Invalid console handle");
	}
	return consoleHandle;
}



void ClearScreen()
{
	COORD cursorPosition = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	DWORD screenBufferSize;

	if (!GetConsoleScreenBufferInfo(GetConsoleHandle(), &consoleScreenBufferInfo))
	{
		//LogToOutputWindow(CreateHresultMessage("Failed to clear screen with error ", GetLastError()));
		return;
	}

	screenBufferSize = consoleScreenBufferInfo.dwSize.X * consoleScreenBufferInfo.dwSize.Y;

	// Clear screen by filling it with blanks
	if (!FillConsoleOutputCharacter(GetConsoleHandle(),
		(CHAR)' ',            // Character to write to the buffer
		screenBufferSize,      // Number of cells to write
		cursorPosition,       //  Coordinates of first cell
		&cCharsWritten))      //  Receive number of characters written
	{
		return;
	}

	if (!SetConsoleCursorPosition(GetConsoleHandle(), cursorPosition))
	{
		//LogToOutputWindow(CreateHresultMessage("Failed to reset console cursor position whilst clearing the screen with error ", GetLastError()));
	}
}


void WriteLogChar(unsigned char ch)
{
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1, nullptr, nullptr);
	if (ch == '\r')
	{
		ch = '\n';
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1, nullptr, nullptr);
	}
}
