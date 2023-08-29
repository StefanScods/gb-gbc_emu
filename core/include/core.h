/*
The header declaration for the main emulator core.
*/
#ifndef CORE_H
#define CORE_H

#include <wx/wxprec.h>
#include <string>
#include <mutex> 

#include "SDL.h"

#include "cpu.h"
#include "memory.h"
#include "ppu.h"
#include "ioController.h"
#include "register.h"
#include "defines.h"
#include "cartridge.h"
#include "sdlController.h"
#include "../../GUI/include/app.h"


class Core{
private:
    CPU cpu;
    Memory memory;
    Cartridge cartridge;
    PPU ppu;
    IOController ioController; 

    ExecutionModes executionMode = PAUSE;

    bool emulatingGBColour = false;

    // A flag indicating a ROM is loaded.
    bool loadedROM = false;

    std::mutex mtx;
public:
    // Make the core's controller public so that the rest of the codebase can modify bindings without needing wrapper functions.
    SDLController controller;

    Core(ExecutionModes mode);
    ~Core();

    /**
     * @brief The main body of the emulator core. This function 
     * should be called within the GUI's event loop.
     * 
     * This function first sets the execution mode based on any
     * of the raised flags. Then depending on the state, this function
     * will pass execution to the correct function.
     * 
     * @returns void.
     */
    void emulatorMain();

    /**
     * @brief An accessor function for retrieve the memory sub-object.
     * 
     * @returns Memory*.
     */
    Memory* getMemory(){ return &memory;}
    /**
     * @brief An accessor function for retrieve the PPU sub-object.
     * 
     * @returns PPU*.
     */
    PPU* getPPU(){ return &ppu;}

    /**
     * @brief Runs the emulator core for frame in emulator 
     * application time (not one frame of GameBoy Time).
     * 
     * @param breakOnCPU - Stops emulation after executing
     * a single CPU instuction.
     * 
     * @returns void.
     */
    void runForFrame(bool breakOnCPU);

    /**
     * @brief Dumps all important emulation states to stdio.
     * 
     * @returns - void
     */
    void printCurrentState(){cpu.printCurrentState();}

    /**
     * @brief Populates a CPU_State struct with the current state of 
     * the CPU. This function can be used to pipe the current state 
     * of the CPU to other parts of the emulator application or to
     * serialize the CPU for save stating.
     * 
     * @param CPU_StateBuffer The buffer to populate. 
     */
    void populateCpuStateBuffer(CPU_State* CPU_StateBuffer);

    /**
     * @brief Fetches and executes the next instuction pointed to by
     * the CPU's program counter register. Runs any auxillary hardware 
     * for the amount of cycles the CPU ran for. Returns the number of 
     * cycles the CPU ran for.
     * 
     * @return cycles 
     */
    cycles fetchAndExecute();

    /**
     * @brief A callback function which sets the emulator's
     * execution state to PAUSE.
     * 
     * @returns void.
     */
    void pauseEmulatorExecution();
    /**
     * @brief A callback function which sets the emulator's
     * execution state to CONTINUE.
     * 
     * @returns void.
     */
    void continueEmulatorExecution();
    /**
     * @brief A callback function which toggles the emulator's
     * execution state between CONTINUE and PAUSE.
     * 
     * @returns void.
     */
    void toggleEmulatorExecution();
    /**
     * @brief A callback function which sets the emulator's
     * execution state to STEP.
     * 
     * @returns void.
     */
    void stepNextFrameButton();
    /**
     * @brief A callback function which sets the emulator's
     * execution state to STEP_CPU.
     * 
     * @returns void.
     */
    void stepNextInstuctionButton();

    /**
     * @brief Loads a ROM file and begins emulating it as a cartridge.
     * Returns a bool indicating success.
     * 
     * @param filePath The absolute path to the ROM file.
     */
    bool loadROM(std::string filePath);

    /**
     * @brief The logic for when the TIMA timer 
     * overflows.
     */
    void TIMATimerOverflowLogic();

    /**
     * @brief Sets the emulator to either emulate the GameBoy or
     * GameBoyColor. This is determined by the GBC flag within the 
     * game's ROM file.
     */
    void setCGBMode(bool isCGB){ emulatingGBColour = isCGB; }
    bool getCGBMode(){ return emulatingGBColour; }

    /**
     * @brief Checks the CPU's master interrupt flag along with the enable interrupt 
     * register + the interrupt flags to see if there a interrupt to respond to.
     * If so, the core will evoke the CPU to begin handling the interrupt.
     * 
     * Call this function after all the hardware has cycled.
    */
    void handleInterrupts();

     /**
     * @brief Acquires the mutex lock which protects the memory sub-system.
     */
    void acquireMutexLock(){ mtx.lock();}
    /**
     * @brief Releases the mutex lock which protects the memory sub-system.
     */
    void releaseMutexLock(){ mtx.unlock();}
};

#endif