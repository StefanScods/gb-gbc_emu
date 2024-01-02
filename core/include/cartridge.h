#ifndef CARTRIDGE_H
#define CARTRIDGE_H
/*
Cartridge class for loading .gb rom files 

date: 2022-05-14
*/
#include "defines.h"
#include <fstream>
#include <string>

class Memory;
class Core;

class Cartridge {
private:
	byte romTitle[TITLE_END - TITLE_START + 2] = "";
	byte manufacturerCode[MANUFACTURERCODE_END - MANUFACTURERCODE_START + 2] = "";
	byte newLicenseCode[NEWLICENSECODE_END - NEWLICENSECODE_START + 2] = "";
	byte oldLicenseCode = 0;
	byte cartridgeType = 0;
	byte destinationCode = 0;
	byte versionNumber = 0;

	byte mbc1Mask = 0;
	byte mbc1Mode = 0;
	byte mbc1ROMBank = 0;
	byte mbc1ROMSecondaryBank = 0;
	bool mbc1RAMEnable = false;

	bool mbc3RAMEnable = false;
	byte mbc3ROMBank = 0;
	byte mbc3RAMBank = 0;
	byte mbc3Latch = 0;

	uint32_t romSize = 0;
	uint32_t ramSize = 0;

	std::string memoryControllerName = "";
	std::string licensee = "";

	bool CGB_flag = false;
	bool SGB_flag = false;

	std::ifstream romFile;
	byte* headerDataBuffer = nullptr;
	byte* romData = nullptr;
	byte* externalRAM = nullptr;

	bool romLoaded = false;

public:
	
	//!!!todo handle checksums when loading cartridge.
	/**
	 * @brief Opens a GameBoy or GameBoy Color ROM file. Returns a 
	 * error code indicating the success of the loading process. 0 
	 * or SUCCESS is returned if no errors were encountered.
	 * 
	 * @param filepath The path to the ROM file.
	 * @param Core A pointer to the emulator's core.
	 * 
	 * @returns LoadCartridgeReturnCodes. 0/SUCCESS upon success. 
     * Positive integer upon failure.
	 */
	LoadCartridgeReturnCodes open(const char* filepath, Core* core);

	/**
	 * @brief Returns whether a ROM cart is currently loaded.
	 */
	bool isROMLoaded(){return romLoaded;}

	/**
	 * @brief Returns whether a ROM cart is a GBC cart only.
	 */
	bool isGBCROM(){return CGB_flag;}

	/**
	 * @brief Closes the loaded ROM file and performs any required clean up.
	 */
	void close();

	/**
	 * Accessors for the currently loaded cartridge. Note, these will return
	 * the most recent cartridge in the case that the cartridge is removed.
	*/
	std::string getROMName(){ return std::string((char*) romTitle);}
	std::string getMemoryControllerName(){ return memoryControllerName;}
	std::string getVersionNumberText(){ return std::to_string((int)versionNumber);}
	std::string getLicensee() {return licensee;}
	std::string getROMSizeText(bool humanReadable = true){return humanReadable ? convertBytesToHumanReadable(romSize) : std::to_string(romSize);}
	std::string getRAMSizeText(bool humanReadable = true){return humanReadable ? convertBytesToHumanReadable(ramSize) : std::to_string(ramSize);}
	std::string getOverseasOnlyText(){ return destinationCode == 0x1 ? "True" : "False";}
	std::string getGameBoyOnlyText(){ return CGB_flag ? "True" : "False";}
	std::string getManufacturerCode(){ return std::string((char*) manufacturerCode);}
	std::string getLicenseeCode(){ return oldLicenseCode == 0x33 ? std::string((char*) newLicenseCode) : std::to_string((int) oldLicenseCode);}

	/**
	 * @brief Helper function to convert large number into human readable forms.
	 * EX 32768 -> 32KB.
	*/
	std::string convertBytesToHumanReadable(uint32_t size);

	/**
	 * @brief Uses the current values of oldLicenseCode and newLicenseCode
	 * to resolve the name of the licensee.
	*/
	void resolveLicensee();

	// Memory Controllers.
	byte noMemoryControllerRead(word address);
	void noMemoryControllerWrite(word address, byte data);

	byte controllerMCB1Read(word address);
	void controllerMCB1Write(word address, byte data);

	byte controllerMCB3Read(word address);
	void controllerMCB3Write(word address, byte data);
};


#endif 
