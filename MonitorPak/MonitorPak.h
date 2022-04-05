#pragma once
#include <array>
#include <cstdint>


#ifdef CPAK_EXPORTS
#define CPAK_EXPORT extern "C" __declspec(dllexport)
#else
#define CPAK_EXPORT
#endif


struct Ports
{
	static const unsigned char Command = 0x68;
	static const unsigned char PutChar = 0x69;
};

struct Commands
{
	static const unsigned char ClearScreen = 0;
	static const unsigned char DumpMMU = 1;
};



typedef void(*DYNAMICMENUCALLBACK)(const char *, int, int);

extern std::array<unsigned char, 16> MMURegisterMirror;


//	Console
void ClearScreen();
void WriteLogChar(unsigned char ch);

//	Commands
void ExecuteCommand(unsigned char commandId);

