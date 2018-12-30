#include "GMC.h"
#include "ROM.h"
#include "sn76496.h"


typedef void(*SETCART)(unsigned char);
typedef void(*SETCARTPOINTER)(SETCART);
typedef void(*DYNAMICMENUCALLBACK)(char *, int, int);



namespace detail
{
	void(*PakSetCart)(unsigned char) = NULL;
	ROM ROMImage;
	SN76489Device PSG;
}






GMC_EXPORT void ModuleName(char *ModName, char *CatNumber, DYNAMICMENUCALLBACK Temp)
{
	strcpy(ModName, "Game Master Catridge (SN76489)");
	CatNumber[0] = 0;
}


GMC_EXPORT void ModuleConfig(unsigned char /*menuId*/)
{
}


GMC_EXPORT void ModuleReset(void)
{
	detail::ROMImage.Load("gmc.rom");
	detail::ROMImage.Reset();
	detail::PSG.device_start();

	if (detail::PakSetCart)
	{
		detail::PakSetCart(1);
	}
}


GMC_EXPORT unsigned char SetCart(SETCART Pointer)
{
	detail::PakSetCart = Pointer;

	return 0;
}





GMC_EXPORT unsigned char PakMemRead8(unsigned short address)
{
	return detail::ROMImage.Read(address);
}




GMC_EXPORT void PackPortWrite(unsigned char port, unsigned char data)
{
	switch (port)
	{
	case 0x40:
		detail::ROMImage.SetBank(data);
		break;

	case 0x41:
		detail::PSG.write(data);
		break;
	}
}


GMC_EXPORT unsigned char PackPortRead(unsigned char port)
{
	if (port == 0x40)
	{
		return detail::ROMImage.GetBank();
	}

	return 0;
}


// This gets called at the end of every scan line 262 Lines * 60 Frames = 15780 Hz 15720
GMC_EXPORT unsigned short ModuleAudioSample(void)
{
	SN76489Device::stream_sample_t lbuffer, rbuffer;

	return detail::PSG.sound_stream_update(lbuffer, rbuffer);
}

