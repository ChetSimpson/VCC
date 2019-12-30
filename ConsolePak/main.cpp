#include <windows.h>


#ifdef GMC_EXPORTS
#define GMC_EXPORT extern "C" __declspec(dllexport)
#else
#define GMC_EXPORT
#endif


struct Ports
{
	static const unsigned char PutChar = 0x68;
};

typedef void(*DYNAMICMENUCALLBACK)(const char *, int, int);


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



GMC_EXPORT void ModuleName(char *moduleName, char *catalogId, DYNAMICMENUCALLBACK /*addMenuCallback*/)
{
	strcpy(moduleName, "Console Pak");
	strcpy(catalogId, "CPak-80");
}


GMC_EXPORT void ModuleStatus(char *statusBuffer)
{
	strcpy(statusBuffer, "CPak Active");
}


GMC_EXPORT void PackPortWrite(unsigned char port, unsigned char data)
{
	if(port == Ports::PutChar)
	{
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), &data, 1, nullptr, nullptr);
		if (data == '\r')
		{
			data = '\n';
			WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), &data, 1, nullptr, nullptr);
		}
	}
}

