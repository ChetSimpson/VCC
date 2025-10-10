////////////////////////////////////////////////////////////////////////////////
//	Copyright 2015 by Joseph Forgione
//	This file is part of VCC (Virtual Color Computer).
//	
//	VCC (Virtual Color Computer) is free software: you can redistribute itand/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	
//	VCC (Virtual Color Computer) is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	
//	You should have received a copy of the GNU General Public License
//	along with VCC (Virtual Color Computer).  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vcc/common/exports.h>
#include <Windows.h>

// Module DLL capability pointers
typedef void (*CARTMENUCALLBACK)(const char* MenuName, int MenuId, int MenuType);
typedef void (*GETNAME)(char*, char*, CARTMENUCALLBACK);
typedef void (*CONFIGIT)(unsigned char);
typedef void (*HEARTBEAT) (void);
typedef unsigned char (*PACKPORTREAD)(unsigned char);
typedef void (*PACKPORTWRITE)(unsigned char, unsigned char);
typedef void (*PAKINTERUPT)(unsigned char, unsigned char);
typedef unsigned char (*MEMREAD8)(unsigned short);
typedef void (*SETCART)(unsigned char);
typedef void (*SETCARTPOINTER)(SETCART);
typedef void (*MEMWRITE8)(unsigned char, unsigned short);
typedef void (*MODULESTATUS)(char*);
typedef void (*DMAMEMPOINTERS) (MEMREAD8, MEMWRITE8);
typedef void (*SETINTERUPTCALLPOINTER) (PAKINTERUPT);
typedef unsigned short (*MODULEAUDIOSAMPLE)(void);
typedef void (*MODULERESET)(void);
typedef void (*SETINIPATH)(const char*);
typedef void (*ASSERTINTERUPT)(unsigned char, unsigned char);


struct LIBCOMMON_EXPORT ModulePlugin
{
	using CARTMENUCALLBACK = void (*)(const char* MenuName, int MenuId, int MenuType);
	using SETCART = void (*)(unsigned char);
	using PAKINTERUPT = void (*)(unsigned char, unsigned char);

	virtual void GetModuleName(char*, char*, CARTMENUCALLBACK) = 0;
	virtual void ConfigModule(unsigned char) = 0;
	virtual void SetInteruptCallPointer(PAKINTERUPT) = 0;
	virtual void DmaMemPointer(MEMREAD8, MEMWRITE8) = 0;
	virtual void HeartBeat(void) = 0;
	virtual void PakPortWrite(unsigned char, unsigned char) = 0;
	virtual unsigned char PakPortRead(unsigned char) = 0;
	virtual void PakMemWrite8(unsigned char, unsigned short) = 0;
	virtual unsigned char PakMemRead8(unsigned short) = 0;
	virtual void ModuleStatus(char*) = 0;
	virtual unsigned short ModuleAudioSample(void) = 0;
	virtual void ModuleReset(void) = 0;
	virtual void SetIniPath(const char*) = 0;
	virtual void PakSetCart(SETCART) = 0;
};

class LegacyModulePlugin : public ModulePlugin
{
public:
	LegacyModulePlugin(HMODULE moduleHandle);

	void GetModuleName(char* moduleName, char* projectId, CARTMENUCALLBACK menuCallback) override;
	void ConfigModule(unsigned char menuId) override;	//	FIXME: should be menu clicked
	void SetInteruptCallPointer(PAKINTERUPT assertCallback) override;
	void DmaMemPointer(MEMREAD8 readDataFunction, MEMWRITE8 writeDataFunction) override;
	void HeartBeat(void) override;
	void PakPortWrite(unsigned char, unsigned char) override;
	unsigned char PakPortRead(unsigned char) override;
	void PakMemWrite8(unsigned char, unsigned short) override;
	unsigned char PakMemRead8(unsigned short) override;
	void ModuleStatus(char*) override;
	unsigned short ModuleAudioSample(void) override;
	void ModuleReset(void) override;
	void SetIniPath(const char*) override;
	void PakSetCart(SETCART) override;

private:

	const GETNAME GetModuleName_;
	const CONFIGIT ConfigModule_;
	const SETINTERUPTCALLPOINTER SetInteruptCallPointer_;
	const DMAMEMPOINTERS DmaMemPointer_;
	const HEARTBEAT HeartBeat_;
	const PACKPORTWRITE  PakPortWrite_;
	const PACKPORTREAD PakPortRead_;
	const MEMWRITE8 PakMemWrite8_;
	const MEMREAD8 PakMemRead8_;
	const MODULESTATUS ModuleStatus_;
	const MODULEAUDIOSAMPLE ModuleAudioSample_;
	const MODULERESET ModuleReset_;
	const SETINIPATH SetIniPath_;
	const SETCARTPOINTER PakSetCart_;

private:

	static void default_get_module_name(char*, char*, CARTMENUCALLBACK);
	static void default_config_module(unsigned char);
	static void default_set_interupt_call_pointer(PAKINTERUPT);
	static void default_dma_mem_pointers(MEMREAD8, MEMWRITE8);
	static void default_heartbeat(void);
	static void default_pack_port_write(unsigned char, unsigned char);
	static unsigned char default_pack_port_read(unsigned char);
	static void default_mem_write8(unsigned char, unsigned short);
	static unsigned char default_mem_read8(unsigned short);
	static void default_set_cart(unsigned char);
	static void default_module_status(char*);
	static unsigned short default_module_audio_sample(void);
	static void default_module_reset(void);
	static void default_set_ini_path(const char*);
	static void default_set_cart_pointer(SETCART);
};



