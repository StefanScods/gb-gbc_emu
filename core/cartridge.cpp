/*
Cartridge class for loading .gb rom files

date: 2022-05-14
*/

#include "include/cartridge.h"
#include "include/memory.h"
#include "include/core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

LoadCartridgeReturnCodes Cartridge::open(const char* filepath, Core* core) {
	// Clear previous load attempts.
	if(headerDataBuffer) delete[] headerDataBuffer;
	if(romFile.is_open()) romFile.close();
	romLoaded = false;

	// Read the ROM file.
	romFile.open(filepath, std::ios::in | std::ios::binary);
	if (!romFile.is_open()) return CANNOT_READ_FILE; // Cannot read the ROM file.

	// Determine the size of the cartridge header.
	int bytesToRead = CARTRIDGE_HEADER_SIZE * sizeof(byte);

	// Read the header of the cartridge.
	headerDataBuffer = new byte[bytesToRead];
	if(headerDataBuffer == NULL) return OTHER;
	romFile.read((char *)headerDataBuffer, bytesToRead);

	// Extract the Mode Flag.
	CGB_flag = false;
	SGB_flag = false;
	if(((int) headerDataBuffer[CGB_FLAG_ADDR]) == 0xC0) {
		// Compatible with just CGB.
		CGB_flag = true;
	}
	if(((int) headerDataBuffer[SGB_FLAG_ADDR]) == 0x03) {
		// Compatible with just SGB.
		SGB_flag = true;
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
		newLicenseCode[i] = headerDataBuffer[i + NEWLICENSECODE_START];
	}
	newLicenseCode[NEWLICENSECODE_END - NEWLICENSECODE_START] = '\0';
	// Extract the old license Code.
	oldLicenseCode = headerDataBuffer[OLD_LICENSE_CODE_ADDR];
	resolveLicensee();

	// Extract the Destination Code.
	destinationCode = headerDataBuffer[DESTINATION_CODE_ADDR];

	// Extract the Mask ROM Version number.
	versionNumber = headerDataBuffer[ROM_VERSION_NUMBER_ADDR];

	// Extract the cartridge type and performs some cartridge specific initialization.
	cartridgeType = headerDataBuffer[CARTRIDGE_TYPE_ADDR];
	switch (cartridgeType) {
	case(ROM_ONLY):
		memoryControllerName = "ROM Only";
		break;
	case(MBC1):
		memoryControllerName = "MBC1";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC1_RAM):
		memoryControllerName = "MBC1 + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC1_RAM_BATTERY):
		memoryControllerName = "MBC1 + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC2):
		memoryControllerName = "MBC1";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC2_BATTERY):
		memoryControllerName = "MBC2 + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(ROM_RAM):
		memoryControllerName = "MBC2 + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(ROM_RAM_BATTERY):
		memoryControllerName = "MBC2 + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MMM01):
		memoryControllerName = "MMM01";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MMM01_RAM):
		memoryControllerName = "MMM01 + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MMM01_RAM_BATTERY):
		memoryControllerName = "MMM01 + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC3_TIMER_BATTERY):
		memoryControllerName = "MMM01 + Timer + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC3_TIMER_RAM_BATTERY):
		memoryControllerName = "MMM01 + Timer + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC3):
		memoryControllerName = "MBC3";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC3_RAM):
		memoryControllerName = "MBC3 + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC3_RAM_BATTERY):
		memoryControllerName = "MBC3 + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC4):
		memoryControllerName = "MBC4";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC4_RAM):
		memoryControllerName = "MBC4 + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC4_RAM_BATTERY):
		memoryControllerName = "MBC4 + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC5):
		memoryControllerName = "MBC5";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC5_RAM):
		memoryControllerName = "MBC5 + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC5_RAM_BATTERY):
		memoryControllerName = "MBC5 + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC5_RUMBLE):
		memoryControllerName = "MBC5 + Rumble";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC5_RUMBLE_RAM):
		memoryControllerName = "MBC5 + Rumble + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC5_RUMBLE_RAM_BATTERY):
		memoryControllerName = "MBC5 + Rumble + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(POCKET_CAMERA):
		memoryControllerName = "Pocket Camera";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(BANDAI_TAMA5):
		memoryControllerName = "Bandai TAMA5";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(HuC3):
		memoryControllerName = "HuC3";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(HuC1_RAM_BATTERY):
		memoryControllerName = "HuC1 + RAM + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	default:
		memoryControllerName = "Unknown";
		return INVALID_MEMORY_CONTROLLER;
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
		return INVALID_ROM_SIZE;
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
		return INVALID_RAM_SIZE;
	}

	// Free the header buffer.
	delete[] headerDataBuffer;
	headerDataBuffer = nullptr;

	// Store the first two banks of ROM into memory.
	storeROMBankIntoMemory(0, core->getMemory());
	storeROMBankIntoMemory(1, core->getMemory());

	// Raise the ROM loaded flag and print debug info.
	romLoaded = true;

	// !!!todo Remove this print once PPU has been updated.
	if(CGB_flag) std::cout << "GameBoy Colour is currently not fully emulated :(!" << std::endl;
	if(SGB_flag) std::cout << "Super GameBoy is currently not fully emulated :(!" << std::endl;
	return SUCCESS;
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

std::string Cartridge::convertBytesToHumanReadable(uint32_t size){
	const int SUPPORTED_MAG = 4;
	std::string suffixes[SUPPORTED_MAG] = {
 		"B",
		"KB",
		"MB",
		"GB"
	};

	int magnitude = 0;
	double whole = size;
	while(whole >= 1024 || magnitude > SUPPORTED_MAG){
		whole = whole / 1024;
		magnitude++;
	}

	if(magnitude > SUPPORTED_MAG) return "";
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << whole << suffixes[magnitude];
	return stream.str();
}

void Cartridge::resolveLicensee(){
	// According to https://gbdev.io/pandocs/The_Cartridge_Header.html#014b--old-licensee-code.
	// todo!!! implement conversion chart.
	licensee = "";
}