#ifndef PPU_HEADER
#define PPU_HEADER

#include "../util/typedefs.hpp"

class PPU {

public:

    PPU(/* args */) = default;
    ~PPU() { };

    u32 buffer[256*240] = { 0 };

    bool renderFrame;


private:
    /* data */

};


#endif