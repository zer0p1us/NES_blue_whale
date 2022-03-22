#include "CPU.hpp"

#include <iostream>
#include <stdint.h>

#include "../debug.hpp"
#include "ROM.hpp"


// Addressing mode //

// return the address of operands

// next byte contains operand
uint16_t CPU::immediate(){
    return ++r_program_counter;
}

// next byte contains address of first 256 bytes containing the data
uint16_t CPU::zero_page(){
    uint8_t zero_page = read(++r_program_counter);
    return zero_page % 256;
}

// next byte contains address of first 256 bytes with x register offset containing the data
uint16_t CPU::zero_page_x(){
    uint8_t zero_page = read(++r_program_counter);
    return (zero_page + r_index_x) % 256;
}

// next byte contains address of first 256 bytes with y register offset containing the data
uint16_t CPU::zero_page_y(){
    uint8_t zero_page = read(++r_program_counter);
    return (zero_page + r_index_y) % 256;
}

uint16_t CPU::absolute(){
    uint8_t lmb = read(++r_program_counter);
    uint8_t rmb = read(++r_program_counter);
    return rmb * 265 + lmb;
}



uint8_t CPU::read(uint16_t address){
    if (address >= 0x0000 & address < 0x2000){
        return ram.read(address);
    } else if (address >= 0x2000 & address < 0x4000){
        return ppu->read(address);
    } else if (address >= 0x8000 & address <= 0xFFFD){
        return mapper->prg_read(address);
    }

    else{
        return 0;
    }

}

void CPU::write(uint16_t address, uint8_t data){
    if (address >= 0x0000 & address < 0x2000 ){
        ram.write(address, data);
    } else if (address >= 0x2000 & address < 0x4000){
        ppu->write(address, data);
    }
}

}

void CPU::step(){
    
}
