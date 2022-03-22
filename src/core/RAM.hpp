#ifndef RAM_HEADER
#define RAM_HEADER

#include <stdio.h>
#include <stdint.h>

#include "BUS.hpp"

class RAM : public BUS {

//256 byte pages, 8 pages on internal NES RAM
private:
    uint8_t ram[2048] = {0};

public:
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);
};

#endif