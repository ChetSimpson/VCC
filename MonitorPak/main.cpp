#include "MonitorPak.h"
#include <windows.h>
#include <array>
#include <iostream>
#include <iomanip>
#include <sstream>



BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/, DWORD fdwReason, LPVOID /*lpReserved*/)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		break;

	case DLL_PROCESS_DETACH:
		FreeConsole();
		break;
	}

	return TRUE;
}



CPAK_EXPORT void ModuleName(char *moduleName, char *catalogId, DYNAMICMENUCALLBACK /*addMenuCallback*/)
{
	strcpy(moduleName, "Monitor Pak");
	strcpy(catalogId, "MPak-80");
}


CPAK_EXPORT void ModuleStatus(char *statusBuffer)
{
	strcpy(statusBuffer, "MPak Active");
}




CPAK_EXPORT void PackPortWriteEx(unsigned char port, unsigned char data)
{
	if (port >= 0xa0 && port <= 0xaf)
	{
		MMURegisterMirror[port & 0x0f] = data;
	}

	switch (port)
	{
	case Ports::DataA:
		CommandDataA = data;
		break;
	case Ports::DataB:
		CommandDataB = data;
		break;
	case Ports::DataC:
		CommandDataC = data;
		break;
	case Ports::DataD:
		CommandDataD = data;
		break;
	case Ports::Command:
		ExecuteCommand(data);
		break;
	case Ports::PutChar:
		WriteLogChar(data);
	}
}
