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

public:

    // Read and write operators for the joypad.
    byte read();
    void write(byte d_data);

    // State modifiers.
    void pressA(){a=true;}
    void releaseA(){a=false;}
    void pressB(){b=true;}
    void releaseB(){b=false;}
    void pressStart(){start=true;}
    void releaseStart(){start=false;}
    void pressSelect(){select=true;}
    void releaseSelect(){select=false;}
    void pressUp(){up=true;}
    void releaseUp(){up=false;}
    void pressDown(){down=true;}
    void releaseDown(){down=false;}
    void pressLeft(){left=true;}
    void releaseLeft(){left=false;}
    void pressRight(){right=true;}
    void releaseRight(){right=false;}

    /**
     * Resets the I/O device back to it's initial state.
    */
    void reset();
};

#endif