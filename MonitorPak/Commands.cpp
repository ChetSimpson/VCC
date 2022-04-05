#include "MonitorPak.h"
#include <windows.h>
#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>



void DumpMMUCommand()
{
	std::ostringstream buffer;
	buffer << "| ";
	for (auto i = 0U; i < MMURegisterMirror.size(); ++i)
	{
		if (i == 8)
		{
			buffer << " | ";
		}
		buffer << "  " << std::hex << std::setw(1) << i;
	}
	buffer << "\n| ";

	for (auto i = 0U; i < MMURegisterMirror.size(); ++i)
	{
		if (i == 8)
		{
			buffer << " | ";
		}
		buffer << ' ' << std::hex << std::setw(2) << std::setfill('0') << (int)MMURegisterMirror[i];
	}
	buffer << "\n";


	const auto& str(buffer.str());
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), nullptr, nullptr);
}


void ExecuteCommand(unsigned char commandId)
{
	switch (commandId)
	{
	case Commands::ClearScreen:
		ClearScreen();
		break;

	case Commands::DumpMMU:
		DumpMMUCommand();
		break;
	}
}

