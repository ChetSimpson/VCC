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
	static const unsigned char DataA   = 0x68;
	static const unsigned char DataB   = 0x69;
	static const unsigned char DataC   = 0x6A;
	static const unsigned char DataD   = 0x6B;
	static const unsigned char Command = 0x6E;
	static const unsigned char PutChar = 0x6F;
};

struct Commands
{
	static const unsigned char Reset = 0;
	static const unsigned char ClearScreen = 1;
	static const unsigned char DumpMMU = 2;
	static const unsigned char DumpAllCycles = 3;
	static const unsigned char DumpUpdateCycles = 4;
	static const unsigned char DumpRestoreCycles = 5;
	static const unsigned char DumpDrawCycles = 6;
};



typedef void(*DYNAMICMENUCALLBACK)(const char *, int, int);

extern std::array<unsigned char, 16> MMURegisterMirror;
extern unsigned char CommandDataA;
extern unsigned char CommandDataB;
extern unsigned char CommandDataC;
extern unsigned char CommandDataD;

//	Console
void ClearScreen();
void WriteLogChar(unsigned char ch);

//	Commands
void ExecuteCommand(unsigned char commandId);

