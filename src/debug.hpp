#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <string>

#define DEBUG

static void debug_out(std::string debug_message){
    #ifdef DEBUG
        std::cout << debug_message << '\n';
    #endif
}
#endif