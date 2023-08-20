#include "include\defines.h"
#include <chrono>

void debug::scream() {
	std::cout << "ahhhh" << std::endl;
}


void debug::loudScream() {
	std::cout << "AHHHHHH" << std::endl;
}


/*
* Since Window's library sleep function is not nanosecond accurate, this function implements a spin lock for accurate "sleep" times 
* Use function for only small delays as spin locks are CPU resource intensive.
*/
void debug::spinSleep(long long nanosecondsToSleep) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long long elpasedTime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    while (elpasedTime < nanosecondsToSleep) {
        end = std::chrono::steady_clock::now();
        elpasedTime = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();

    }
}

void convertWordToHexNotation(word value, char *output)
{
    output[6] = NULL;
    output[5] = toHex[value % 16];
    value = value >> 4;
    output[4] = toHex[value % 16];
    value = value >> 4;
    output[3] = toHex[value % 16];
    value = value >> 4;
    output[2] = toHex[value % 16];
    output[1] = 'x';
    output[0] = '0';
}

void convertWordToBinaryNotation(word value, char *output)
{
    output[9] = NULL;
    int offset = 1;
    for(int i = 7 ; i >= 0; i--){
        output[ i + offset] = (value % 2) ? '1' : '0';
        value = value >> 1;

        if(i % 4 == 0) offset--;
    }
}