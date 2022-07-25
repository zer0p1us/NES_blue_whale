#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <string>
#include <iostream>


static void debug_out(std::string debug_message){
    #ifndef NDEBUG // Not Debug Macro defined by CMAKE
        std::cout << debug_message << '\n';
    #endif
}
#endif