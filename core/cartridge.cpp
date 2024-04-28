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
#include <functional>

void RTC::syncToCartridge(std::string cartridgeName){
	std::string timerSaveFilePath = std::string(BAT_SAVE_SUB_DIR)
		.append("\\")
		.append(cartridgeName)
		.append(".rtc");

	// Try an open the RTC save.
	clockFile.open(timerSaveFilePath, std::ios::in | std::ios::out | std::ios::binary);

	// RTC file does not exist. Make a new one.
	if (!clockFile.is_open()){
		clockFile.close();
		// Create a battery save directory if one does not exist.
		if (!std::filesystem::is_directory(BAT_SAVE_SUB_DIR) || !std::filesystem::exists(BAT_SAVE_SUB_DIR)) {
			std::filesystem::create_directory(BAT_SAVE_SUB_DIR);
		}

		// Create a new file. 
		clockFile.open(timerSaveFilePath, std::ios::out | std::ios::binary);
		saveClockToFile();
	} else {
		// Load the buffer with the save files contents.
		loadClockFromFile();
	}
}

void RTC::latchClock(){
	// Get current number of seconds. 
	long long currTime = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::steady_clock::now().time_since_epoch()
	).count();
	// Figure out what value to latch.
	long long timeToLatch = valueSaved;
	if(halt == 0) timeToLatch += (currTime - lastSaved);

	sec = (timeToLatch % 60) & 0XFF;
	timeToLatch /= 60; 
	min = (timeToLatch % 60) & 0XFF;
	timeToLatch /= 60; 
	hour = (timeToLatch % 24) & 0XFF;
	timeToLatch /= 24; 

	dayL = timeToLatch & 0XFF;
	timeToLatch = timeToLatch >> 8;
	dayH = timeToLatch & 0b1;
	timeToLatch = timeToLatch >> 1;

	// Check to see if the clock has overflowed.
	if (timeToLatch > 0) carry = 1;
}

byte RTC::readLatchedData(int targetReg){
	byte value = 0;
	switch(targetReg){
		// RTC S - Seconds.
		case 0x8:
			return sec;
		// RTC M - Minutes.
		case 0x9:
			return min;
		// RTC H - Hours.
		case 0xA:
			return hour;
		// RTC DL - Lower 8 bits of Day Counter.
		case 0xB:
			return dayL;
		// RTC DH - Upper 1 bit of Day Counter, Carry Bit, Halt Flag.
		case 0xC:
			writeBit(value, 7, carry);
			writeBit(value, 6, halt);
			writeBit(value, 0, dayH);
			return value;
		default:
			break;
	}
	return HIGH_IMPEDANCE;
}

void RTC::writeToClock(int targetReg, byte data){
	// Get current number of seconds. 
	long long currTime = std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::steady_clock::now().time_since_epoch()
	).count();
	// Figure out what value to latch.
	long long timeToLatch = valueSaved;
	if(halt == 0) timeToLatch += (currTime - lastSaved);
	
	switch(targetReg){
		// RTC S - Seconds.
		case 0x8:
			sec = data;
			// Replace the sec value.
			valueSaved -= timeToLatch % 60;
			timeToLatch += sec;
			break;
		// RTC M - Minutes.
		case 0x9:
			min = data;
			// Replace the min value.
			timeToLatch -= ((timeToLatch / 60) % 60) * 60;
			timeToLatch += min * 60;
			break;
		// RTC H - Hours.
		case 0xA:
			hour = data;
			// Replace the hour value.
			timeToLatch -= ((timeToLatch / 3600) % 24) * 3600;
			timeToLatch += hour * 3600;
			break;
		// RTC DL - Lower 8 bits of Day Counter.
		case 0xB:
			dayL = data;
			// Replace the day value.
			timeToLatch -= ((timeToLatch / 86400) % 0xFF) * 86400;
			timeToLatch += dayL * 86400;
			break;
		// RTC DH - Upper 1 bit of Day Counter, Carry Bit, Halt Flag.
		case 0xC:
			dayH = readBit(data, 0);
			halt =  readBit(data, 6);
			carry = readBit(data, 7);
			// Replace the day value.
			timeToLatch -= ((timeToLatch / 22032000)) * 22032000;
			timeToLatch += dayH * 22032000;
			break;
		default:
			break;
	}

	valueSaved = timeToLatch;
	lastSaved = currTime;
	saveClockToFile(true);
}

void RTC::reset(){
	sec = 0;
	min = 0;
	hour = 0;
	dayL = 0;
	dayH = 0;
	halt = 0;
	carry = 0;
	lastSaved = 0;
	valueSaved = 0;
	clockFile.close();
}

void RTC::saveClockToFile(bool partial){
	int bytesToWrite = sizeof(long long)*2;
	int offset = sizeof(byte)*4 + sizeof(bool)*3;
	if (!partial){
		bytesToWrite += offset;
		offset = 0;
	}
    
    byte* writeBuffer = new byte[
        bytesToWrite
    ];
    byte* writeBufferStart = writeBuffer;

	// Only write out the entire state if partial is turned off.
	if(!partial){
		std::memcpy(writeBuffer, &sec, sizeof(byte)); writeBuffer+=sizeof(byte);
		std::memcpy(writeBuffer, &min, sizeof(byte)); writeBuffer+=sizeof(byte);
		std::memcpy(writeBuffer, &hour, sizeof(byte)); writeBuffer+=sizeof(byte);
		std::memcpy(writeBuffer, &dayL, sizeof(byte)); writeBuffer+=sizeof(byte);
		
		std::memcpy(writeBuffer, &dayH, sizeof(bool)); writeBuffer+=sizeof(bool);
		std::memcpy(writeBuffer, &halt, sizeof(bool)); writeBuffer+=sizeof(bool);
		std::memcpy(writeBuffer, &carry, sizeof(bool)); writeBuffer+=sizeof(bool);
	}

	std::memcpy(writeBuffer, &lastSaved, sizeof(long long)); writeBuffer+=sizeof(long long);
	std::memcpy(writeBuffer, &valueSaved, sizeof(long long)); writeBuffer+=sizeof(long long);

	// Write out the data.
	clockFile.seekp(offset);
    clockFile.write((char*)writeBufferStart, bytesToWrite);
    delete[] writeBufferStart;
}

void RTC::loadClockFromFile(){
	int bytesToRead = sizeof(byte)*4 + sizeof(bool)*3 + sizeof(long long)*2;
    byte* readBuffer = new byte[
        bytesToRead
    ];
    byte* readBufferStart = readBuffer;
	clockFile.seekg(0);
    clockFile.read((char*)readBufferStart, bytesToRead);
	
	std::memcpy(&sec, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&min, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&hour, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&dayL, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	
	std::memcpy(&dayH, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
	std::memcpy(&halt, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
	std::memcpy(&carry, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

	std::memcpy(&lastSaved, readBuffer, sizeof(long long)); readBuffer+=sizeof(long long);
	std::memcpy(&valueSaved, readBuffer, sizeof(long long)); readBuffer+=sizeof(long long);

	delete[] readBufferStart;
}

LoadCartridgeReturnCodes Cartridge::open(const char* filepath, Core* core) {
	// Clear previous load attempts.
	close();
	Memory* memory = core->getMemory();

	// Read the ROM file.
	romFile.open(filepath, std::ios::in | std::ios::binary);
	if (!romFile.is_open()) return CANNOT_READ_FILE; // Cannot read the ROM file.
	std::filesystem::path cartridgePath(filepath); 
	cartridgeName = cartridgePath.stem().generic_string();
	gbcFileExtention = cartridgePath.extension() == ".gbc";

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
	// Title is one char shorter on GBC carts.
	if(CGB_flag) romTitle[TITLE_END - TITLE_START] = '\0';

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
	usingBattery = false;
	usingRTC = false;
	switch (cartridgeType) {
	case(ROM_ONLY):
		memoryControllerName = "ROM Only";
		memory->setMemoryController(
			std::bind(&Cartridge::noMemoryControllerWrite, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::noMemoryControllerRead, this, std::placeholders::_1),
			std::bind(&Cartridge::noMemoryControllerSaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::noMemoryControllerLoadFromState, this, std::placeholders::_1)
		);
		break;
	case(MBC1):
		memoryControllerName = "MBC1";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB1Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB1Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB1SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB1LoadFromState, this, std::placeholders::_1)
		);
		break;
	case(MBC1_RAM):
		memoryControllerName = "MBC1 + RAM";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB1Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB1Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB1SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB1LoadFromState, this, std::placeholders::_1)
		);
		break;
	case(MBC1_RAM_BATTERY):
		memoryControllerName = "MBC1 + RAM + Battery";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB1Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB1Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB1SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB1LoadFromState, this, std::placeholders::_1)
		);
		usingBattery = true;
		break;
	case(MBC2):
		memoryControllerName = "MBC2";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(MBC2_BATTERY):
		memoryControllerName = "MBC2 + Battery";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(ROM_RAM):
		memoryControllerName = "ROM + RAM";
		return INVALID_MEMORY_CONTROLLER; // !!!todo support this mc.
		break;
	case(ROM_RAM_BATTERY):
		memoryControllerName = "ROM + RAM + Battery";
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
		memoryControllerName = "MBC3 + Timer + Battery";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB3Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB3Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3LoadFromState, this, std::placeholders::_1)
		);
		usingBattery = true;
		usingRTC = true;
		break;
	case(MBC3_TIMER_RAM_BATTERY):
		memoryControllerName = "MBC3 + Timer + RAM + Battery";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB3Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB3Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3LoadFromState, this, std::placeholders::_1)
		);
		usingBattery = true;
		usingRTC = true;
		break;
	case(MBC3):
		memoryControllerName = "MBC3";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB3Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB3Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3LoadFromState, this, std::placeholders::_1)
		);
		break;
	case(MBC3_RAM):
		memoryControllerName = "MBC3 + RAM";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB3Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB3Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3LoadFromState, this, std::placeholders::_1)
		);
		break;
	case(MBC3_RAM_BATTERY):
		memoryControllerName = "MBC3 + RAM + Battery";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB3Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB3Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB3LoadFromState, this, std::placeholders::_1)
		);
		usingBattery = true;
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
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB5Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB5Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB5SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB5LoadFromState, this, std::placeholders::_1)
		);
		break;
	case(MBC5_RAM):
		memoryControllerName = "MBC5 + RAM";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB5Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB5Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB5SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB5LoadFromState, this, std::placeholders::_1)
		);
		break;
	case(MBC5_RAM_BATTERY):
		memoryControllerName = "MBC5 + RAM + Battery";
		memory->setMemoryController(
			std::bind(&Cartridge::controllerMCB5Write, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Cartridge::controllerMCB5Read, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB5SaveToState, this, std::placeholders::_1),
			std::bind(&Cartridge::controllerMCB5LoadFromState, this, std::placeholders::_1)
		);
		usingBattery = true;
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
		romSize = ROM_BANK_SIZE*2;
		mbc1Mask = 0b00000000;
		break;
	case(0x01):
		romSize = ROM_BANK_SIZE*4;
		mbc1Mask = 0b00000011;
		break;
	case(0x02):
		romSize = ROM_BANK_SIZE*8;
		mbc1Mask = 0b00000111;
		break;
	case(0x03):
		romSize = ROM_BANK_SIZE*16;
		mbc1Mask = 0b00001111;
		break;
	case(0x04):
		romSize = ROM_BANK_SIZE*32;
		break;
	case(0x05):
		romSize = ROM_BANK_SIZE*64;
		break;
	case(0x06):
		romSize = ROM_BANK_SIZE*128;
		break;
	case(0x07):
		romSize = ROM_BANK_SIZE*256;
		break;
	// These sizes are unofficial.
	case(0x52):
		romSize = ROM_BANK_SIZE*72;
		break;
	case(0x53):
		romSize = ROM_BANK_SIZE*80;
		break;
	case(0x54):
		romSize = ROM_BANK_SIZE*96;
		break;
	default:
		romSize = 0;
		return INVALID_ROM_SIZE;
	}

	// Get the on-cartridge RAM size.
	byte ramSizeCode = headerDataBuffer[RAM_SIZE_ADDR];
	switch (ramSizeCode) {
	case(0x00):
		ramSize = 0;
		usingBattery = false;
		break;
	case(0x01):
		ramSize = (int)RAM_BANK_SIZE*0.25;
		break;
	case(0x02):
		ramSize = RAM_BANK_SIZE;
		break;
	case(0x03):
		ramSize = RAM_BANK_SIZE*4;
		break;
	case(0x04):
		ramSize = RAM_BANK_SIZE*16;
		break;
	case(0x05):
		ramSize = RAM_BANK_SIZE*8;
		break;
	default:
		ramSize = 0;
		usingBattery = false;
		return INVALID_RAM_SIZE;
	}
	if(ramSize) externalRAM = new byte[ramSize];

	// Set up battery backed memory.
	if(usingBattery) setUpRAMBatteryFile();

	// Set up timer.
	if(usingRTC) realTimeClock.syncToCartridge(cartridgeName);

	// Allocate the ROM.
	romData = new byte[romSize];
	// Read the ROM file into memory. For speed, load the entire ROM into memory -> Worst case 8Mb.
	romFile.seekg(ROMBANK0_START);
	romFile.read((char*) romData, romSize*sizeof(byte));
	// ROMFile is no longer needed -> can close.
	romFile.close();

	// Raise the ROM loaded flag and print debug info.
	romLoaded = true;

	return SUCCESS;
}

void Cartridge::close() {
	// Clear any dynamic data.
	if(headerDataBuffer){
		delete[] headerDataBuffer;
		headerDataBuffer = nullptr;
	}
	if(externalRAM){
		delete[] externalRAM;
		externalRAM = nullptr;
	}
	if(romData){
		delete[] romData;
		romData = nullptr;
	}
	// Close the ROM file.
	if (romLoaded) romFile.close();

	// Close the RAM file.
	if(usingBattery) ramFile.close();

	if(usingRTC) realTimeClock.reset();
		
	// Clear control flags.
	romLoaded = false;
	gbcFileExtention = false;
	CGB_flag = false;
	SGB_flag = false;
	usingBattery = false;
	usingRTC = false;

	// Reset all memory controller regs.
	mbc1Mask = 0b00011111;
	mbc1Mode = 0;
	mbc1ROMBank = 0;
	mbc1ROMSecondaryBank = 0;
	mbc1RAMEnable = false;

	mbc3RAMEnable = false;
	mbc3ROMBank = 0;
	mbc3RAMBank = 0;
	mbc3Latch = 0;

	mbc5RAMEnable = false;
	mbc5ROMBank = 0;
	mbc5RAMBank = 0;
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

void Cartridge::setUpRAMBatteryFile(){
	std::string batSaveFilePath = std::string(BAT_SAVE_SUB_DIR)
		.append("\\")
		.append(cartridgeName)
		.append(".sav");

	// Try an open the battery save.
	ramFile.open(batSaveFilePath, std::ios::in | std::ios::out | std::ios::binary);

	// Save file does not exist. Make a new one.
	if (!ramFile.is_open()){
		ramFile.close();
		// Create a battery save directory if one does not exist.
		if (!std::filesystem::is_directory(BAT_SAVE_SUB_DIR) || !std::filesystem::exists(BAT_SAVE_SUB_DIR)) {
			std::filesystem::create_directory(BAT_SAVE_SUB_DIR);
		}

		// Create a new file. 
		ramFile.open(batSaveFilePath, std::ios::out | std::ios::binary);
		ramFile.write((char*)externalRAM, ramSize);
	} else {
		// Load the buffer with the save files contents.
		ramFile.read((char*)externalRAM, ramSize);
	}
}

/*======================================================================*
 *  						No Controller								*
 *======================================================================*/

byte Cartridge::noMemoryControllerRead(word address){
	if(address >= romSize) return HIGH_IMPEDANCE;
	return romData[address-ROMBANK0_START];
}
void Cartridge::noMemoryControllerWrite(word address, byte data){
	// No Op!
}
void Cartridge::noMemoryControllerSaveToState(std::ofstream & stateFile){
	// No Op!
}
void Cartridge::noMemoryControllerLoadFromState(std::ifstream & stateFile){
	// No Op!
}

/*======================================================================*
 *  							    MCB1								*
 *======================================================================*/

byte Cartridge::controllerMCB1Read(word address){
	// ROM Bank X0.
	if(address >= ROMBANK0_START && address <= ROMBANK0_END){
		// Return the first bank always in mode 0.
		if(mbc1Mode == 0x0) return romData[address - ROMBANK0_START];
		// Else use the secondary bank reg.
		int bankToRead = ((mbc1ROMSecondaryBank << 5));
		if((uint32_t)(bankToRead*ROM_BANK_SIZE) > romSize) bankToRead = 0;
		uint32_t addressToRead =  bankToRead*ROM_BANK_SIZE + (address - ROMBANK0_START);
		return romData[addressToRead];
	// ROM Bank 01-7F.
	} else if(address >= ROMBANKN_START && address <= ROMBANKN_END){
		// Try reading using the secondary bank.
		int bankToRead = ((mbc1ROMSecondaryBank << 5) + mbc1ROMBank);
		// If its too large, just use the bottom bits.
		if((uint32_t)(bankToRead*ROM_BANK_SIZE) > romSize) bankToRead = mbc1ROMBank;
		//std::cout << "Read ROM Bank " << bankToRead << std::endl;
		if(mbc1Mode == 0x1) std::cout << (int) mbc1ROMSecondaryBank << " " << (int) bankToRead << std::endl;
		uint32_t addressToRead = bankToRead*ROM_BANK_SIZE + (address - ROMBANKN_START);
		return romData[addressToRead];
	// RAM Bank 00–03, if any.
	} else if(address >= EXTERNALRAM_START && address <= EXTERNALRAM_END){
		// If RAM is disabled or the read is too large return high impedance.
		if(!mbc1RAMEnable ||  (uint32_t)(address - EXTERNALRAM_START) >= ramSize) return HIGH_IMPEDANCE;
		// Determine which bank to read from.
		int targetBank = 0;
		if (mbc1Mode == 0x01) targetBank = mbc1ROMSecondaryBank;
		// Calculate the read offset.
		uint32_t addressToRead =  targetBank*RAM_BANK_SIZE + (address - EXTERNALRAM_START);
		// Read the value of RAM;
		return externalRAM[addressToRead];
	}

	return HIGH_IMPEDANCE;
}

void Cartridge::controllerMCB1Write(word address, byte data){
	// std::cout << std::hex << (int) address << " " << (int) data << std::endl;
	// RAM Enable.
	if(address >= 0x0000 && address <= 0x1FFF){
		// Enable the external RAM partition iff the data is 0xA. Else disable.
		if(data==0x0A) mbc1RAMEnable = true;
		else mbc1RAMEnable = false;
	// ROM Bank Number.
	} else if (address >= 0x2000 && address <= 0x3FFF){
		// Apply a mask and ensure the selected bank does not go below 1.
		if((data & 0b00011111) == 0x0) data = 0x01; 
		// std::cout << (int) data << std::endl;
		mbc1ROMBank = mbc1Mask & data; // Apply the mask second to emulate the bug found in hw.
	// RAM Bank Number - or - Upper Bits of ROM Bank Number.
	} else if (address >= 0x4000 && address <= 0x5FFF){
		mbc1ROMSecondaryBank = 0b00000011 & data;
		// std::cout << "SECONDARY BANK " << (int) mbc1ROMSecondaryBank << std::endl;
	// ROM/RAM Mode Select.
	} else if (address >= 0x6000 && address <= 0x7FFF){
		mbc1Mode = 0b00000001 & data;
		// std::cout << "MODE SWITCH" << (int) mbc1Mode << std::endl;
		// exit(0);
	// RAM Bank 00–03, if any.
	} else if(address >= EXTERNALRAM_START && address <= EXTERNALRAM_END){
		// If RAM is disabled or the address is too large do nothing.
		if(!mbc1RAMEnable || (uint32_t)(address - EXTERNALRAM_START) >= ramSize) return;
		// Determine which bank to write to.
		int targetBank = 0;
		if (mbc1Mode == 0x01) targetBank = mbc1ROMSecondaryBank;
		// Calculate the write offset.
		uint32_t addressToWrite = targetBank*RAM_BANK_SIZE + (address - EXTERNALRAM_START);
		// Write the value of RAM.
		externalRAM[addressToWrite] = data;

		// Write to the "battery backed" file. !!!TODO do this on another thread and maybe group writes for efficiency.
		if(usingBattery){
			ramFile.seekp(addressToWrite);
			ramFile.write((char*) &data, 1);
		}
	}
	// If we write outside this address space -> simply ignore.
}
void Cartridge::controllerMCB1SaveToState(std::ofstream & stateFile){
	int ramCopySize = sizeof(byte)*ramSize;
    int bytesToWrite = sizeof(byte)*4 + sizeof(bool)*1 + ramCopySize;
    byte* writeBuffer = new byte[
        bytesToWrite
    ];
    byte* writeBufferStart = writeBuffer;

	std::memcpy(writeBuffer, &mbc1Mask, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc1Mode, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc1ROMBank, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc1ROMSecondaryBank, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc1RAMEnable, sizeof(bool)); writeBuffer+=sizeof(bool);

	std::memcpy(writeBuffer, externalRAM, ramCopySize); writeBuffer+=ramCopySize;

	// Write out the data.
    stateFile.write((char*)writeBufferStart, bytesToWrite);
    delete[] writeBufferStart;
}
void Cartridge::controllerMCB1LoadFromState(std::ifstream & stateFile){
	int ramCopySize = sizeof(byte)*ramSize;
    int bytesToRead = sizeof(byte)*4 + sizeof(bool)*1 + ramCopySize;
    byte* readBuffer = new byte[
        bytesToRead
    ];
    byte* readBufferStart = readBuffer;
    stateFile.read((char*)readBufferStart, bytesToRead);

	std::memcpy(&mbc1Mask, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc1Mode, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc1ROMBank, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc1ROMSecondaryBank, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc1RAMEnable, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

	std::memcpy(externalRAM, readBuffer, ramCopySize); readBuffer+=ramCopySize;

	delete[] readBufferStart;
}

/*======================================================================*
 *  							    MCB3								*
 *======================================================================*/
byte Cartridge::controllerMCB3Read(word address){
	// ROM Bank 0.
	if(address >= ROMBANK0_START && address <= ROMBANK0_END){
		return romData[address - ROMBANK0_START];
	// ROM Bank 01-7F.
	} else if(address >= ROMBANKN_START && address <= ROMBANKN_END){
		uint32_t addressToRead = mbc3ROMBank*ROM_BANK_SIZE + (address - ROMBANKN_START);
		return romData[addressToRead];
	// RAM Bank 00–03 or RTC Register 08-0C.
	} else if(address >= EXTERNALRAM_START && address <= EXTERNALRAM_END){
		// If RAM is disabled or the read is too large return high impedance.
		if(!mbc3RAMEnable || (uint32_t)(address - EXTERNALRAM_START) >= ramSize) return HIGH_IMPEDANCE;
		// Different behaviour depending on the current mbc3RAMBank value.
		switch(mbc3RAMBank){
			// RAM Bank 00-03.
			case 0x0:
			case 0x1:
			case 0x2:
			case 0x3:{
				// Calculate the read offset.
				uint32_t addressToWrite = mbc3RAMBank*RAM_BANK_SIZE + (address - EXTERNALRAM_START);
				// Read the value of RAM.
				return externalRAM[addressToWrite];
			}
			// RTC.
			case 0x8:
			case 0x9:
			case 0xA:
			case 0xB:
			case 0xC:
				if(usingRTC) return realTimeClock.readLatchedData(mbc3RAMBank);
			default:
				return HIGH_IMPEDANCE;
		}
	}
	return HIGH_IMPEDANCE;
}
void Cartridge::controllerMCB3Write(word address, byte data){
	// RAM and Timer Enable.
	if(address >= 0x0000 && address <= 0x1FFF){
		// Enable the external RAM partition along with RTC regs iff the data is 0xA. Else disable.
		if(data==0x0A) mbc3RAMEnable = true;
		else mbc3RAMEnable = false;
	// ROM Bank Number.
	} else if (address >= 0x2000 && address <= 0x3FFF){
		// Take 7 bits.
		data = data & 0b01111111;
		// Ensure the data does not equal zero.
		if(data == 0x0) data = 0x01; 
		mbc3ROMBank = data;
	// RAM Bank Number - or - RTC Register Select.
	} else if (address >= 0x4000 && address <= 0x5FFF){
		// Take 3 bits.
		mbc3RAMBank = 0b00001111 & data;
	// Latch Clock Data.
	} else if (address >= 0x6000 && address <= 0x7FFF){
		// When changing the latch from 0 -> 1, latch current time.
		if(mbc3Latch == 0x00 && data == 0x01){
			// Save current time.
			if(usingRTC) realTimeClock.latchClock();
		}
		mbc3Latch = data;
	// RAM Bank 00–03 or RTC Register 08-0C.
	} else if(address >= EXTERNALRAM_START && address <= EXTERNALRAM_END){
		// If RAM is disabled or the address is too large do nothing.
		if(!mbc3RAMEnable || (uint32_t)(address - EXTERNALRAM_START) >= ramSize) return;

		// Different behaviour depending on the current mbc3RAMBank value.
		switch(mbc3RAMBank){
			// RAM Bank 00-03.
			case 0x0:
			case 0x1:
			case 0x2:
			case 0x3:{
				// Calculate the write offset.
				uint32_t addressToWrite = mbc3RAMBank*RAM_BANK_SIZE + (address - EXTERNALRAM_START);
				// Write the value of RAM.
				externalRAM[addressToWrite] = data;

				// Write to the "battery backed" file. !!!TODO do this on another thread and maybe group writes for efficiency.
				if(usingBattery){
					ramFile.seekp(addressToWrite);
					ramFile.write((char*) &data, 1);
				}
				break;
			}
			// RTC.
			case 0x8:
			case 0x9:
			case 0xA:
			case 0xB:
			case 0xC:
				if(usingRTC) realTimeClock.writeToClock(mbc3RAMBank, data);
			default:
				break;
		}

	}
}
void Cartridge::controllerMCB3SaveToState(std::ofstream & stateFile){
	int ramCopySize = sizeof(byte)*ramSize;
    int bytesToWrite = sizeof(byte)*3 + sizeof(bool)*1 + ramCopySize;
    byte* writeBuffer = new byte[
        bytesToWrite
    ];
    byte* writeBufferStart = writeBuffer;

	std::memcpy(writeBuffer, &mbc3ROMBank, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc3RAMBank, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc3Latch, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc3RAMEnable, sizeof(bool)); writeBuffer+=sizeof(bool);

	std::memcpy(writeBuffer, externalRAM, ramCopySize); writeBuffer+=ramCopySize;

	// Write out the data.
    stateFile.write((char*)writeBufferStart, bytesToWrite);
    delete[] writeBufferStart;

	// Save the clock.
	if(usingRTC) realTimeClock.saveClockToFile();
}
void Cartridge::controllerMCB3LoadFromState(std::ifstream & stateFile){
	int ramCopySize = sizeof(byte)*ramSize;
    int bytesToRead = sizeof(byte)*3 + sizeof(bool)*1 + ramCopySize;
    byte* readBuffer = new byte[
        bytesToRead
    ];
    byte* readBufferStart = readBuffer;
    stateFile.read((char*)readBufferStart, bytesToRead);

	std::memcpy(&mbc3ROMBank, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc3RAMBank, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc3Latch, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc3RAMEnable, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

	std::memcpy(externalRAM, readBuffer, ramCopySize); readBuffer+=ramCopySize;

	delete[] readBufferStart;

	// Sync the bat file.
	if(usingBattery){
		ramFile.seekp(0);
		ramFile.write((char*) externalRAM, ramCopySize);
	}

	// Load the clock.
	if(usingRTC) realTimeClock.loadClockFromFile();
}


byte Cartridge::controllerMCB5Read(word address){
	// ROM Bank 0.
	if(address >= ROMBANK0_START && address <= ROMBANK0_END){
		return romData[address - ROMBANK0_START];
	// ROM Bank 01-7F.
	} else if(address >= ROMBANKN_START && address <= ROMBANKN_END){
		uint32_t addressToRead = mbc5ROMBank*ROM_BANK_SIZE + (address - ROMBANKN_START);
		return romData[addressToRead];
	// RAM Bank 00–03.
	} else if(address >= EXTERNALRAM_START && address <= EXTERNALRAM_END){
		// If RAM is disabled or the read is too large return high impedance.
		if(!mbc5RAMEnable || (uint32_t)(address - EXTERNALRAM_START) >= ramSize) return HIGH_IMPEDANCE;
		// Calculate the read offset.
		uint32_t addressToWrite = mbc5RAMBank*RAM_BANK_SIZE + (address - EXTERNALRAM_START);
		// Read the value of RAM.
		return externalRAM[addressToWrite];
	} return HIGH_IMPEDANCE;
}

void Cartridge::controllerMCB5Write(word address, byte data){
	// RAM Enable.
	if(address >= 0x0000 && address <= 0x1FFF){
		// Enable the external RAM partition iff the data is 0xA. Else disable.
		if((data & 0xF) == 0xA) mbc5RAMEnable = true;
		else mbc5RAMEnable = false;
	// ROM Bank Number (lower 8 bits).
	} else if (address >= 0x2000 && address <= 0x2FFF){
		mbc5ROMBank = mbc5ROMBank & 0x0100 | data;
	// ROM Bank Number (top 9th bit).
	} else if (address >= 0x3000 && address <= 0x3FFF){
		mbc5ROMBank = mbc5ROMBank & 0xFF | (data & 0x01) << 8;
	// RAM Bank Number.
	} else if (address >= 0x4000 && address <= 0x5FFF){
		// Take 3 bits.
		mbc5RAMBank = 0b00001111 & data;
	// RAM Bank 00–03 or RTC Register 08-0C.
	} else if(address >= EXTERNALRAM_START && address <= EXTERNALRAM_END){
		// If RAM is disabled or the read is too large return high impedance.
		if(!mbc5RAMEnable || (uint32_t)(address - EXTERNALRAM_START) >= ramSize) return;
		// Calculate the write offset.
		uint32_t addressToWrite = mbc5RAMBank*RAM_BANK_SIZE + (address - EXTERNALRAM_START);
		// Write the value of RAM.
		externalRAM[addressToWrite] = data;
		// Write to the "battery backed" file. !!!TODO do this on another thread and maybe group writes for efficiency.
		if(usingBattery){
			ramFile.seekp(addressToWrite);
			ramFile.write((char*) &data, 1);
		}
	}
}

void Cartridge::controllerMCB5SaveToState(std::ofstream & stateFile){
	int ramCopySize = sizeof(byte)*ramSize;
    int bytesToWrite = sizeof(byte)*1 + sizeof(bool)*1 + sizeof(word)*1 + ramCopySize;
    byte* writeBuffer = new byte[
        bytesToWrite
    ];
    byte* writeBufferStart = writeBuffer;

	std::memcpy(writeBuffer, &mbc5RAMBank, sizeof(byte)); writeBuffer+=sizeof(byte);
	std::memcpy(writeBuffer, &mbc5ROMBank, sizeof(word)); writeBuffer+=sizeof(word);
	std::memcpy(writeBuffer, &mbc5RAMEnable, sizeof(bool)); writeBuffer+=sizeof(bool);

	std::memcpy(writeBuffer, externalRAM, ramCopySize); writeBuffer+=ramCopySize;

	// Write out the data.
    stateFile.write((char*)writeBufferStart, bytesToWrite);
    delete[] writeBufferStart;
}
void Cartridge::controllerMCB5LoadFromState(std::ifstream & stateFile){
	int ramCopySize = sizeof(byte)*ramSize;
    int bytesToRead = sizeof(byte)*1 + sizeof(bool)*1 + sizeof(word)*1 + ramCopySize;
    byte* readBuffer = new byte[
        bytesToRead
    ];
    byte* readBufferStart = readBuffer;
    stateFile.read((char*)readBufferStart, bytesToRead);

	std::memcpy(&mbc5RAMBank, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
	std::memcpy(&mbc5ROMBank, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
	std::memcpy(&mbc5RAMEnable, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

	std::memcpy(externalRAM, readBuffer, ramCopySize); readBuffer+=ramCopySize;

	delete[] readBufferStart;

	// Sync the bat file.
	if(usingBattery){
		ramFile.seekp(0);
		ramFile.write((char*) externalRAM, ramCopySize);
	}
}