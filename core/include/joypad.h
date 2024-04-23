/*
The header declaration for the GB's joypad I/O device.
*/
#ifndef JOYPAD_H
#define JOYPAD_H

#include "defines.h"

class Joypad {
private:
    bool a = false;
    bool b = false;
    bool start = false;
    bool select = false;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    byte data = 0xFF;

    voidFuncWithNoArguments raiseInterrupt = nullptr; 

public:

    // Read and write operators for the joypad.
    byte read();
    void write(byte d_data);

    void setInterruptCallback(voidFuncWithNoArguments func){raiseInterrupt = func;}

    // State modifiers.
    void pressA(){a=true; raiseInterrupt();}
    void releaseA(){a=false;}
    void pressB(){b=true; raiseInterrupt();}
    void releaseB(){b=false;}
    void pressStart(){start=true; raiseInterrupt();}
    void releaseStart(){start=false;}
    void pressSelect(){select=true; raiseInterrupt();}
    void releaseSelect(){select=false;}
    void pressUp(){up=true; raiseInterrupt();}
    void releaseUp(){up=false;}
    void pressDown(){down=true; raiseInterrupt();}
    void releaseDown(){down=false;}
    void pressLeft(){left=true; raiseInterrupt();}
    void releaseLeft(){left=false;}
    void pressRight(){right=true; raiseInterrupt();}
    void releaseRight(){right=false;}

    /**
     * Resets the I/O device back to it's initial state.
    */
    void reset();
};

#endif