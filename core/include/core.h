/*
The header declaration for the main emulator core.
*/
#ifndef CORE_H
#define CORE_H

#include <wx/wxprec.h>
#include <vector>

#include "SDL.h"

#include "cpu.h"
#include "memory.h"
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

    ExecutionModes executionMode = CONTINUE;

    // Used to determine how many cycles to run per frame.
    cycles cyclesPerFrame;
    // A flag indicating a ROM is loaded.
    bool loadedROM = false;

    // Variables used to determine debug stepping behaviour.

    // An array holding all current breakpoints. Simply add the desired value of PC to this register to pause execution.
    std::vector<word> CPUBreakpoints = {};

    // A boolean indicating if a user is currently holding the "step next frame" button.
    bool holdingStepNextFrameButton = false;
    // Determines the number of frames remaining before the hole core can step a frame again.
    int holdingStepNextFrameDelay = 0;
    // Used to implement decreasing pauses between steps.
    int holdingStepNextFrameDelayStage = 1;
    // A boolean indicating if a user is currently holding the "step next instuction" button.
    bool holdingStepNextInstuctionButton = false;
    // Determines the number of frames remaining before the CPU can step again.
    int holdingStepNextInstuctionDelay = 0;
    // Used to implement decreasing pauses between steps.
    int holdingStepNextInstuctionDelayStage = 1;


public:
    // Make the core's controller public so that the rest of the codebase can modify bindings without needing wrapper functions.
    SDLController controller;

    Core(ExecutionModes mode);

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
     * @brief Runs the emulator core for exactly one frame.
     * 
     * @returns void.
     */
    void runForFrame();
    /**
     * @brief Sets `cyclesPerFrame` to reach the target 
     * framerate.
     * 
     * @returns void.
     */
    void updateCyclesPerFrame();

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
     * @brief A callback function which sets `holdingStepNextFrameButton`
     * to true.
     * 
     * @returns void.
     */
    void keyDownStepNextFrameButton();
    /**
     * @brief A callback function which sets `holdingStepNextFrameButton`
     * to false. This function also resets any debug step logic. 
     * 
     * @returns void.
     */
    void keyUpStepNextFrameButton();
    /**
     * @brief A callback function which sets `holdingStepNextInstuctionButton`
     * to true.
     * 
     * @returns void.
     */
    void keyDownStepNextInstuctionButton();
    /**
     * @brief A callback function which sets `holdingStepNextInstuctionButton`
     * to false. This function also resets any debug step logic. 
     * 
     * @returns void.
     */
    void keyUpStepNextInstuctionButton();

    ~Core();
};

#endif