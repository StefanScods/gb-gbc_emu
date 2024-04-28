/**
 * Class implementation for the GB DMA Controller. 
 */
#include "include/dmaController.h"
#include "include/register.h"

void DMAController::reset(){
    transferActive = false;
    cyclesPassed = 0;
    targetAddress = 0x0000;
}

void DMAController::cycle(bool cpuDoubleSpeed){
    if(!transferActive) return;
    // Cycle the DMA.
    cyclesPassed++;
    if(cpuDoubleSpeed) cyclesPassed++;

    // If enough cycles passed, perform the copy.
    if(cyclesPassed == CYCLES_PER_OAM_DMA){
        for(int i = 0; i < NUMBER_OF_ADDRESS_PER_OAM_DMA_TRANSFER; i++){
            byte srcData = memory->read(targetAddress + i);
            memory->write(0xFE00 + i, srcData);
        }
        // Reset the DMA controller to prepare for the next write.
        reset();
    }
}

bool DMAController::startTransfer(word startingAddress){
    // Ignore colliding DMA requests.
    if(transferActive){
        std::cout << "Warning: DMA Collision!!!!!" << std::endl;
        return false;
    }

    targetAddress = startingAddress;
    transferActive = true;
    return true;
}

void DMAController::saveToState(byte*& writeBuffer){
    std::memcpy(writeBuffer, &transferActive, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &cyclesPassed, sizeof(cycles)); writeBuffer+=sizeof(cycles);
    std::memcpy(writeBuffer, &targetAddress, sizeof(word)); writeBuffer+=sizeof(word);
}

void DMAController::loadFromState(byte*& readBuffer){
    std::memcpy(&transferActive, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&cyclesPassed, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);
    std::memcpy(&targetAddress, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
}

void HDMAController::reset(){
    transferActive = false;
    targetAddress = 0x0000;
    sourceAddress = 0x0000;
    generalPurpose = false;
    transferSize = 0;
}

void HDMAController::setSourceAddr(byte data, bool high){
    if(high) sourceAddress = (sourceAddress & 0x00FF) | (((word) data) << 8);
    else sourceAddress = (sourceAddress & 0xFF00) | (((word) data) & 0xF0);
}

void HDMAController::setTargetAddr(byte data, bool high){
    if(high) targetAddress = (targetAddress & 0x00FF) | (((word) data & 0x1F) << 8);
    else targetAddress = (targetAddress & 0xFF00) | (((word) data) & 0xF0);
}

void HDMAController::setStatus(byte data){
    // Force stop HDMA without reseting anything.
    if(transferActive && readBit(data, 7) == 0){
        transferActive = false;
        return;
    }

    // Start transfer.
    transferActive = true;
    generalPurpose = readBit(data, 7);
    transferSize = data & 0b01111111;

    // Do a blocking copy if generalPurpose == 0;
    if(!generalPurpose) 
        while(transferActive){transferChunk();}
}

void HDMAController::transferChunk(){
    // No work to do. Return.
    if(!transferActive) return;

    // Transfer the chunk of memory.
    word destAddr = targetAddress + VRAM_START;
    for(int i = 0; i < NUMBER_OF_ADDRESS_PER_HDMA_TRANSFER; i++){
        byte srcData = memory->read(sourceAddress + i);
        memory->write(destAddr + i, srcData);
    }
    targetAddress += NUMBER_OF_ADDRESS_PER_HDMA_TRANSFER;
    sourceAddress += NUMBER_OF_ADDRESS_PER_HDMA_TRANSFER;

    transferSize -= 1;
    // Check to see when the transfer ends.
    if(transferSize == 0xFF) transferActive = false;
}

byte HDMAController::readStatus(){
    byte returnValue = transferSize & 0b01111111;
    writeBit(returnValue, 7, !transferActive);
    return returnValue;
}

void HDMAController::saveToState(byte*& writeBuffer){
    std::memcpy(writeBuffer, &transferActive, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &targetAddress, sizeof(word)); writeBuffer+=sizeof(word);
    std::memcpy(writeBuffer, &sourceAddress, sizeof(word)); writeBuffer+=sizeof(word);
    std::memcpy(writeBuffer, &generalPurpose, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &transferSize, sizeof(byte)); writeBuffer+=sizeof(byte);
}

void HDMAController::loadFromState(byte*& readBuffer){
    std::memcpy(&transferActive, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&targetAddress, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
    std::memcpy(&sourceAddress, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
    std::memcpy(&generalPurpose, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&transferSize, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
}