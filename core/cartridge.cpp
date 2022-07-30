/*
Cartridge class for loading .gb rom files

date: 2022-05-14
*/

#include "include/cartridge.h"
#include <iostream>
#include <fstream>


//!!! ignores the checksums 
bool Cartridge::open(const char* filepath) {


	romFile.open(filepath, std::ios::in | std::ios::binary);
	if (!romFile.is_open()) return false; //cannot load the rom file

	int bytestoread = CARTRIDGE_HEADER_SIZE * sizeof(char);

	dataBuff = (char*) malloc(bytestoread);
	if(dataBuff == NULL) return false;

	romFile.read(dataBuff, bytestoread);

	//extract the title text 
	for (int i = 0; i < TITLE_END - TITLE_START + 1; i++) {
		romTitle[i] = dataBuff[i + TITLE_START];
	}
	romTitle[TITLE_END - TITLE_START] = '\0';

	std::cout << romTitle << std::endl;

	//extract the Manufacturer Code
	for (int i = 0; i < MANUFACTURERCODE_END - MANUFACTURERCODE_START + 1; i++) {
		manufacturerCode[i] = dataBuff[i + MANUFACTURERCODE_START];
	}
	romTitle[MANUFACTURERCODE_END - MANUFACTURERCODE_START] = '\0';

	//extract the CGB Flag
	if(dataBuff[CGB_FLAG_ADDR] == 0x80) {
		//compatible with both gb and cgb 
		CGB_flag = false;
	}
	else if(dataBuff[CGB_FLAG_ADDR] == 0xC0) {
		//compatible with just cgb
		CGB_flag = true;
	}

	//extract the New License Code
	for (int i = 0; i < NEWLICENSECODE_END - NEWLICENSECODE_START + 1; i++) {
		manufacturerCode[i] = dataBuff[i + NEWLICENSECODE_START];
	}
	newLicenseCode[NEWLICENSECODE_END - NEWLICENSECODE_START] = '\0';

	//extract the Destination Code
	destinationCode = dataBuff[DESTINATION_CODE_ADDR];

	//extract the old license Code
	destinationCode = dataBuff[OLD_LICENSE_CODE_ADDR];

	//extract the Mask ROM Version number
	versionNumber = dataBuff[ROM_VERSION_NUMBER_ADDR];

	//extract the cartridge type and performs some catridge specific initalization
	cartridgeType = dataBuff[CARTRIDGE_TYPE_ADDR];

	switch (cartridgeType) {
	case(ROM_ONLY):
		break;
	case(MBC1):
		break;
	case(MBC1_RAM):
		break;
	case(MBC1_RAM_BATTERY):
		break;
	case(MBC2):
		break;
	case(MBC2_BATTERY):
		break;
	case(ROM_RAM):
		break;
	case(ROM_RAM_BATTERY):
		break;
	case(MMM01):
		break;
	case(MMM01_RAM):
		break;
	case(MMM01_RAM_BATTERY):
		break;
	case(MBC3_TIMER_BATTERY):
		break;
	case(MBC3_TIMER_RAM_BATTERY):
		break;
	case(MBC3):
		break;
	case(MBC3_RAM):
		break;
	case(MBC3_RAM_BATTERY):
		break;
	case(MBC4):
		break;
	case(MBC4_RAM):
		break;
	case(MBC4_RAM_BATTERY):
		break;
	case(MBC5):
		break;
	case(MBC5_RAM):
		break;
	case(MBC5_RAM_BATTERY):
		break;
	case(MBC5_RUMBLE):
		break;
	case(MBC5_RUMBLE_RAM):
		break;
	case(MBC5_RUMBLE_RAM_BATTERY):
		break;
	case(POCKET_CAMERA):
		break;
	case(BANDAI_TAMA5):
		break;
	case(HuC3):
		break;
	case(HuC1_RAM_BATTERY):
		break;
	default:
		break;
	}

	//get the rom size
	byte romSizeCode = dataBuff[ROM_SiZE_ADDR];
	switch (romSizeCode) {
	case(0x00):
		romSize = 32768;
		break;
	case(0x01):
		romSize = 65536;
		break;
	case(0x02):
		romSize = 131072;
		break;
	case(0x03):
		romSize = 262144;
		break;
	case(0x04):
		romSize = 524288;
		break;
	case(0x05):
		romSize = 1048576;
		break;
	case(0x06):
		romSize = 2097152;
		break;
	case(0x07):
		romSize = 4194304;
		break;
	case(0x52):
		romSize = 1179648;
		break;
	case(0x53):
		romSize = 1310720;
		break;
	case(0x54):
		romSize = 1572864;
		break;
	default:
		romSize = 0;
		break;
	}

	//get the ram size
	byte ramSizeCode = dataBuff[RAM_SiZE_ADDR];
	switch (ramSize) {
	case(0x00):
		ramSize = 0;
		break;
	case(0x01):
		ramSize = 2048;
		break;
	case(0x02):
		ramSize = 8192;
		break;
	case(0x03):
		ramSize = 32768;
		break;
	default:
		ramSize = 0;
		break;
	}

	romLoaded = true;

	return true;
}

byte Cartridge::readDataBuff(word address) {
	if (dataBuff == NULL) return 0;

	unsigned int parsedByte = dataBuff[address];
	if (parsedByte < 0) {
		return (byte)(parsedByte + 256);
	}

	return (byte)(parsedByte);
}

void  Cartridge::close() {
	if (romLoaded) {
		free(dataBuff);
		romLoaded = false;
		romFile.close();
	}
}