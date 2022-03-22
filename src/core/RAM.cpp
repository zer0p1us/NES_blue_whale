#include "RAM.hpp"

#include <stdint.h>


uint8_t RAM::read(uint16_t addr){
    return ram[addr %= 2048];
}


void RAM::write(uint16_t addr, uint8_t data){
    ram[addr %= 2048] = data;
}
