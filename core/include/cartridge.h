#ifndef CARTRIDGE_H
#define CARTRIDGE_H
/*
Cartridge class for loading .gb rom files 

date: 2022-05-14
*/
#include "defines.h"
#include <fstream>


class Cartridge {
private:

	char* dataBuff = NULL;

	byte romTitle[TITLE_END - TITLE_START + 2] = "";
	byte manufacturerCode[MANUFACTURERCODE_END - MANUFACTURERCODE_START + 2] = "";
	byte newLicenseCode[NEWLICENSECODE_END - NEWLICENSECODE_START + 2] = "";
	byte oldLicenseCode = 0;
	byte cartridgeType = 0;
	byte destinationCode = 0;
	byte versionNumber = 0;

	uint32_t romSize = 0;
	uint32_t ramSize = 0;

	bool CGB_flag = false;
	bool SGB_flag = false;

	std::ifstream romFile;

	bool romLoaded = false;

public:
	
	//opens a rom
	bool open(const char* filepath);

	//safely reads from the dataBuff
	byte readDataBuff(word address);

	//closes the loaded rom 
	void close();

};


#endif 
