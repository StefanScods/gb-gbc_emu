#include <iostream>
#include <cstdint>
#include "core\include\cpu.h"
#include "core\include\memory.h"
#include "core\include\register.h"


#include "GUI/include/screen.h"

int main(int argc, char** argv){

    Screen screen;

  
    CPU cpu;
    Memory memory;


    cpu.init();
    memory.init();
    cpu.bindMemory(&memory);

    memory.write(0x100, INC_B);
    memory.write(0x101, DEC_B);


    cpu.fetchAndExecute();
    cpu.fetchAndExecute();
    cpu.outputState();


    //cpu.executeCycles((CALL_NZ_a16_CYCLES_MAX+INC_A_CYCLES+RET_NZ_CYCLES_MAX));
    //cpu.outputState();
    //std::cout<< (int)memory.read(0) <<std::endl;

    screen.mainloop();
    screen.exit();

    memory.destroy();

    return 0; 
}

