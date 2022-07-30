#include "include\defines.h"
#include <chrono>

void debug::scream() {
	std::cout << "ahhhh" << std::endl;
}


void debug::loudScream() {
	std::cout << "AHHHHHH" << std::endl;
}


/*
* Since the window's library sleep function is not nanosecond accurate, this function implements a spin lock for accurate "sleep" times 
* Use function for only small delays as spin locks are cpu resource intensive
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