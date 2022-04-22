#ifndef PPU_HEADER
#define PPU_HEADER

#include <stdint.h>

#include "BUS.hpp"
#include "Mapper.hpp"

class PPU : BUS {

public:

    PPU(Mapper* p_mapper) : mapper(p_mapper) {}
    ~PPU() { };

    uint32_t buffer[256*240] = { 0 };

    bool renderFrame;


    uint8_t read(uint16_t addr) {return 0;}
    void write(uint16_t addr, uint8_t data) { };

    void step() { };
private:
    Mapper* mapper;

};


#endif