/*
Copyright 2015 by Joseph Forgione
This file is part of VCC (Virtual Color Computer).

    VCC (Virtual Color Computer) is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VCC (Virtual Color Computer) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VCC (Virtual Color Computer).  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Windows.h>
#include <stdio.h>
#include "defines.h"
#include "pakinterface.h"
#include "Vcc.h"
#include "coco3.h"
#include "tcc1014mmu.h"


static unsigned char FileType=0;
static unsigned short FileLenth=0;
static  short StartAddress=0;
static unsigned short XferAddress=0;
static unsigned char *MemImage=nullptr;
static FILE *BinImage=nullptr;
static unsigned char Flag=1;
static int temp=255;

unsigned char QuickLoad(const std::filesystem::path& BinFileName)
{
	unsigned int MemIndex=0;

	if (!std::filesystem::exists(BinFileName))
	{
		return 1;				//File Not Found
	}

	BinImage=_wfopen(BinFileName.c_str(), L"rb");
	if (BinImage == nullptr)
	{
		return 2;				//Can't Open File
	}
			
	MemImage=(unsigned char *)malloc(65535);
	if (MemImage==nullptr)
	{
		MessageBox(nullptr,"Can't alocate ram","Error",0);
		return 3;				//Not enough memory
	}

	auto extension(BinFileName.extension().string());
	std::transform(
		extension.begin(),
		extension.end(),
		extension.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (extension == ".rom" || extension == ".ccc" || extension == "*.pak")
	{
		PakInsertRom(BinFileName);
		// FIXME-CHET: Report errors
		return 0;
	}
	if (extension == ".bin")
	{
		while (true)
		{
			temp=fread(MemImage,sizeof(char),5,BinImage);
			FileType=MemImage[0];
			FileLenth=(MemImage[1]<<8) + MemImage[2];
			StartAddress=(MemImage[3]<<8)+MemImage[4];

			switch (FileType)
			{
			case 0:
				temp=fread(&MemImage[0],sizeof(char),FileLenth,BinImage);
				for (MemIndex=0;MemIndex<FileLenth;MemIndex++) //Kluge!!!
					MemWrite8(MemImage[MemIndex],StartAddress++);
				break;
			case 255:
				XferAddress=StartAddress;
				if ( (XferAddress==0) | (XferAddress >32767) |(FileLenth != 0) )
				{
					MessageBox(nullptr,".Bin file is corrupt or invalid Transfer Address","Error",0);
					return 3;
				}
				fclose(BinImage);
				free(MemImage);
				CPUForcePC(XferAddress);
				return 0;				
				break;
			default:
				MessageBox(nullptr,".Bin file is corrupt or invalid","Error",0);
				fclose(BinImage);
				free(MemImage);
				return 3;
				break;
			} //End Switch
		} //End While
	} // End if
	return 255; //Invalid File type
} //End Proc

unsigned short GetXferAddr()
{
	return XferAddress;
}