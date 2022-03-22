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


CPU::CPU(){

}

void CPU::step(){
    
}
