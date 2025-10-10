#include <vcc/common/ModuleDefs.h>

namespace
{

	template<class Type_>
	Type_ GetImportedProcAddress(HMODULE module, LPCSTR procName, Type_ defaultFunction)
	{
		const auto importedFunction(reinterpret_cast<Type_>(GetProcAddress(module, procName)));

		return importedFunction ? importedFunction : defaultFunction;
	}

}


LegacyModulePlugin::LegacyModulePlugin(HMODULE moduleHandle)
	:
	GetModuleName_(GetImportedProcAddress(moduleHandle, "ModuleName", default_get_module_name)),
	ConfigModule_ (GetImportedProcAddress(moduleHandle, "ModuleConfig", default_config_module)),
	SetInteruptCallPointer_(GetImportedProcAddress(moduleHandle, "AssertInterupt", default_set_interupt_call_pointer)),
	DmaMemPointer_(GetImportedProcAddress(moduleHandle, "MemPointers", default_dma_mem_pointers)),
	HeartBeat_(GetImportedProcAddress(moduleHandle, "HeartBeat", default_heartbeat)),
	PakPortWrite_(GetImportedProcAddress(moduleHandle, "PackPortWrite", default_pack_port_write)),
	PakPortRead_(GetImportedProcAddress(moduleHandle, "PackPortRead", default_pack_port_read)),
	PakMemWrite8_(GetImportedProcAddress(moduleHandle, "PakMemWrite8", default_mem_write8)),
	PakMemRead8_(GetImportedProcAddress(moduleHandle, "PakMemRead8", default_mem_read8)),
	ModuleStatus_(GetImportedProcAddress(moduleHandle, "ModuleStatus", default_module_status)),
	ModuleAudioSample_(GetImportedProcAddress(moduleHandle, "ModuleAudioSample", default_module_audio_sample)),
	ModuleReset_(GetImportedProcAddress(moduleHandle, "ModuleReset", default_module_reset)),
	SetIniPath_(GetImportedProcAddress(moduleHandle,"SetIniPath", default_set_ini_path)),
	PakSetCart_(GetImportedProcAddress(moduleHandle,"SetCart", default_set_cart_pointer))
{
}


void LegacyModulePlugin::GetModuleName(char* moduleName, char* projectId, CARTMENUCALLBACK menuCallback)
{
	GetModuleName_(moduleName, projectId, menuCallback);
}

void LegacyModulePlugin::ConfigModule(unsigned char)
{ }
void LegacyModulePlugin::SetInteruptCallPointer(PAKINTERUPT)
{ }
void LegacyModulePlugin::DmaMemPointer(MEMREAD8, MEMWRITE8)
{ }
void LegacyModulePlugin::HeartBeat(void)
{ }
void LegacyModulePlugin::PakPortWrite(unsigned char, unsigned char)
{ }
unsigned char LegacyModulePlugin::PakPortRead(unsigned char)
{ 
	return {};
}
void LegacyModulePlugin::PakMemWrite8(unsigned char, unsigned short)
{ }
unsigned char LegacyModulePlugin::PakMemRead8(unsigned short)
{
	return {};
}
void LegacyModulePlugin::ModuleStatus(char*)
{ }
unsigned short LegacyModulePlugin::ModuleAudioSample(void)
{
	return {};
}
void LegacyModulePlugin::ModuleReset(void)
{ }
void LegacyModulePlugin::SetIniPath(const char*)
{ }
void LegacyModulePlugin::PakSetCart(SETCART)
{ }

void LegacyModulePlugin::default_get_module_name(char*, char*, CARTMENUCALLBACK)
{
}

void LegacyModulePlugin::default_config_module(unsigned char)
{
}


void LegacyModulePlugin::default_set_interupt_call_pointer(PAKINTERUPT)
{
}


void LegacyModulePlugin::default_dma_mem_pointers(MEMREAD8, MEMWRITE8)
{
}


void LegacyModulePlugin::default_heartbeat(void)
{
}


void LegacyModulePlugin::default_pack_port_write(unsigned char, unsigned char)
{
}


unsigned char LegacyModulePlugin::default_pack_port_read(unsigned char)
{
	return 0;
}


void LegacyModulePlugin::default_mem_write8(unsigned char, unsigned short)
{
}


unsigned char LegacyModulePlugin::default_mem_read8(unsigned short)
{
	return 0;
}


void LegacyModulePlugin::default_set_cart(unsigned char)
{
}


void LegacyModulePlugin::default_module_status(char*)
{
}


unsigned short LegacyModulePlugin::default_module_audio_sample(void)
{
	return 0;
}


void LegacyModulePlugin::default_module_reset(void)
{
}


void LegacyModulePlugin::default_set_ini_path(const char*)
{
}


void LegacyModulePlugin::default_set_cart_pointer(SETCART)
{
}

