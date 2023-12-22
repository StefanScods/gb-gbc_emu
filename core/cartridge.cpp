/*
Cartridge class for loading .gb rom files

date: 2022-05-14
*/

#include "include/cartridge.h"
#include "include/memory.h"
#include "include/core.h"
#include <iostream>
#include <fstream>


//!!! ignores the checksums 
bool Cartridge::open(const char* filepath, Core* core) {

	// Read the ROM file.
	romLoaded = false;
	romFile.open(filepath, std::ios::in | std::ios::binary);
	if (!romFile.is_open()) return false; // Cannot read the ROM file.

	// Determine the size of the cartridge header.
	int bytesToRead = CARTRIDGE_HEADER_SIZE * sizeof(byte);

	// Read the header of the cartridge.
	byte* headerDataBuffer = new byte[bytesToRead];
	if(headerDataBuffer == NULL) return false;
	romFile.read((char *)headerDataBuffer, bytesToRead);

	// Extract the CGB Flag.
	if(((int) headerDataBuffer[CGB_FLAG_ADDR]) == 0x80) {
		// Compatible with both GB and CGB.
		CGB_flag = false;
	}
	else if(((int) headerDataBuffer[CGB_FLAG_ADDR]) == 0xC0) {
		// Compatible with just CGB.
		CGB_flag = true;
	}

	// Extract the title text from the ROM.
	for (int i = 0; i < TITLE_END - TITLE_START + 1; i++) {
		romTitle[i] = headerDataBuffer[i + TITLE_START];
	}
	romTitle[TITLE_END - TITLE_START + 1] = '\0';

	// Extract the Manufacturer Code.
	for (int i = 0; i < MANUFACTURERCODE_END - MANUFACTURERCODE_START + 1; i++) {
		manufacturerCode[i] = headerDataBuffer[i + MANUFACTURERCODE_START];
	}
	manufacturerCode[MANUFACTURERCODE_END - MANUFACTURERCODE_START + 1] = '\0';

	// Extract the New License Code.
	for (int i = 0; i < NEWLICENSECODE_END - NEWLICENSECODE_START + 1; i++) {
		manufacturerCode[i] = headerDataBuffer[i + NEWLICENSECODE_START];
	}
	newLicenseCode[NEWLICENSECODE_END - NEWLICENSECODE_START] = '\0';

	// Extract the Destination Code.
	destinationCode = headerDataBuffer[DESTINATION_CODE_ADDR];

	// Extract the old license Code.
	oldLicenseCode = headerDataBuffer[OLD_LICENSE_CODE_ADDR];

	// Extract the Mask ROM Version number.
	versionNumber = headerDataBuffer[ROM_VERSION_NUMBER_ADDR];

	// Extract the cartridge type and performs some cartridge specific initialization.
	cartridgeType = headerDataBuffer[CARTRIDGE_TYPE_ADDR];
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

	// Get the ROM size.
	byte romSizeCode = headerDataBuffer[ROM_SIZE_ADDR];
	switch (romSizeCode) {
	case(0x00):
		// No bank switching necessary. 
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

	// Get the on-cartridge RAM size.
	byte ramSizeCode = headerDataBuffer[RAM_SIZE_ADDR];
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

	// Free the header buffer.
	delete[] headerDataBuffer;
	headerDataBuffer = nullptr;

	// Store the first two banks of ROM into memory.
	storeROMBankIntoMemory(0, core->getMemory());
	storeROMBankIntoMemory(1, core->getMemory());

	// Raise the ROM loaded flag and print debug info.
	romLoaded = true;
	std::cout << "\nSuccessfully Loaded ROM:" << std::endl;
	std::cout << "Title: " <<  romTitle << std::endl;
	std::cout << "GameBoy Mode: ";
	if(CGB_flag) std::cout << "GameBoy Color"  << std::endl;
	else         std::cout << "GameBoy"  	   << std::endl;
	std::cout << "ROM Size: " <<  romSize << " Bytes" << std::endl;
	std::cout << std::endl;
	return true;
}

void Cartridge::close() {
	if (romLoaded) romFile.close();
	romLoaded = false;
	CGB_flag = false;
}

void Cartridge::storeROMBankIntoMemory(int ROMBankNumber, Memory* memory){
	if (!romFile.is_open()) return;
	memory->storeROMBank(ROMBankNumber, &romFile);
}