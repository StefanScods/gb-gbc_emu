/**
 * Class implementation for the GB DMA Controller. 
 */
#include "include/dmaController.h"

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