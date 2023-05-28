#ifndef CARTRIDGE_H
#define CARTRIDGE_H
/*
Cartridge class for loading .gb rom files 

date: 2022-05-14
*/
#include "defines.h"
#include <fstream>

class Memory;

class Cartridge {
private:
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
	
	/**
	 * @brief Opens a GameBoy or GameBoy Color ROM file. Returns a 
	 * boolean indicating the success of the loading process. Returns 
	 * false upon failure.
	 * 
	 * @param filepath The path to the ROM file.
	 * @param memory A pointer to the emulators memory.
	 * @return boolean
	 */
	bool open(const char* filepath, Memory* memory);

	/**
	 * @brief Closes the loaded ROM file and performs any required clean up.
	 */
	void close();

	/**
	 * @brief Due to the second ROM bank having a dynamic value, i.e. points to different 
	 * blocks of the file, this function is used to load a new bank into memory,
	 * 
	 * @param ROMBankNumber The bank number to switch too.
	 * @param memory - The pointer to the emulator's memory.
	 */
	void storeROMBankIntoMemory(int ROMBankNumber, Memory* memory);
};


#endif 
