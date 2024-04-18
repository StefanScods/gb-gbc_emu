#ifndef CARTRIDGE_H
#define CARTRIDGE_H
/*
Cartridge class for loading .gb rom files 

date: 2022-05-14
*/
#include "defines.h"
#include <fstream>
#include <string>
#include <filesystem> 
#include <chrono>

class Memory;
class Core;

class RTC {
	private:
		// The value currently held in the clock;
		byte sec = 0;
		byte min = 0;
		byte hour = 0;
		byte dayL = 0;
		bool dayH = 0;

		bool halt = 0;
		bool carry = 0;

		// The unix timestamp of when the clock value was last saved.
		long long lastSaved = 0;
		long long valueSaved = 0;

		std::string targetClockName;

		std::fstream clockFile;

	public:
		void syncToCartridge(std::string cartridgeName);
		void latchClock();

		void reset();

		byte readLatchedData(int targetReg);
		void writeToClock(int targetReg, byte data);

		void saveClockToFile(bool partial = false);
		void loadClockFromFile();
};

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
	RTC realTimeClock;

	uint32_t romSize = 0;
	uint32_t ramSize = 0;

	std::string memoryControllerName = "";
	std::string licensee = "";

	bool CGB_flag = false;
	bool SGB_flag = false;

	std::ifstream romFile;

	std::fstream ramFile;

	byte* headerDataBuffer = nullptr;
	byte* romData = nullptr;
	byte* externalRAM = nullptr;

	bool romLoaded = false;
	bool usingBattery = false;
	bool usingRTC = false;

	std::string cartridgeName = "";
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
	std::string getCartridgeName(){ return cartridgeName;}
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

	/**
	 * @brief Opens, and creates if necessary, the battery back RAM file the emulator
	 * will use to maintain RAM state after "power off".
	*/
	void setUpRAMBatteryFile();

	// Memory Controllers.
	byte noMemoryControllerRead(word address);
	void noMemoryControllerWrite(word address, byte data);
	void noMemoryControllerSaveToState(std::ofstream & stateFile);
	void noMemoryControllerLoadFromState(std::ifstream & stateFile);

	byte controllerMCB1Read(word address);
	void controllerMCB1Write(word address, byte data);
	void controllerMCB1SaveToState(std::ofstream & stateFile);
	void controllerMCB1LoadFromState(std::ifstream & stateFile);

	byte controllerMCB3Read(word address);
	void controllerMCB3Write(word address, byte data);
	void controllerMCB3SaveToState(std::ofstream & stateFile);
	void controllerMCB3LoadFromState(std::ifstream & stateFile);
};


#endif 
