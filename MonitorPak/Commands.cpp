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

uint32_t GlobalUpdateCycles = 0;
uint32_t GlobalRestoreCycles = 0;
uint32_t GlobalDrawCycles = 0;

void DumpCycles(uint32_t cycleCount, uint32_t & currentCycleCount, const std::string& label)
{
	if (cycleCount > currentCycleCount)
	{
		currentCycleCount = cycleCount;

		//std::ostringstream buffer;
		//buffer << label << "Cycles: " << cycleCount << "\n";
		//const auto& str(buffer.str());
		//WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), nullptr, nullptr);
	}
}


void DumpAllCycles()
{
	std::ostringstream buffer;

	buffer
		<< "\nCycle counts:\n"
		<< "Update Cycles: " << GlobalUpdateCycles << "\n"
		<< "Restore Cycles: " << GlobalRestoreCycles << "\n"
		<< "Draw Cycles: " << GlobalDrawCycles << "\n";

	const auto& str(buffer.str());
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.size(), nullptr, nullptr);

}
void ExecuteCommand(unsigned char commandId)
{
	auto dataWord
		= (CommandDataA << 24)
		| (CommandDataB << 16)
		| (CommandDataC << 8)
		| (CommandDataD << 0);

	switch (commandId)
	{
	case Commands::Reset:
		GlobalUpdateCycles = 0;
		GlobalRestoreCycles = 0;
		GlobalDrawCycles = 0;
		break;

	case Commands::ClearScreen:
		ClearScreen();
		break;

	case Commands::DumpMMU:
		DumpMMUCommand();
		break;

	case Commands::DumpAllCycles:
		DumpAllCycles();
		break;

	case Commands::DumpUpdateCycles:
		DumpCycles(dataWord, GlobalUpdateCycles, "Update");
		break;

	case Commands::DumpRestoreCycles:
		DumpCycles(dataWord, GlobalRestoreCycles, "Restore");
		break;

	case Commands::DumpDrawCycles:
		DumpCycles(dataWord, GlobalDrawCycles, "Draw");
		break;
	}
}

