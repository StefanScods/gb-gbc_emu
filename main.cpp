#include <iostream>
#include <cstdint>
#include "core\include\cpu.h"
#include "core\include\memory.h"
#include "core\include\register.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char** argv){

    int temp;
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


    memory.destroy();

    std::cin >> temp;

    return 0; 
}

