#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <string>
#include <iostream>

#define DEBUG

static void debug_out(std::string debug_message){
    #ifdef DEBUG
        std::cout << debug_message << '\n';
    #endif
}
#endif