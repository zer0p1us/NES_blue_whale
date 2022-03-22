#include "CPU.hpp"

#include <iostream>
#include <stdint.h>

#include "../debug.hpp"
#include "ROM.hpp"

void CPU::step(){
    execute(fetch());
    ++r_program_counter;
}

void CPU::reset(){
    r_program_counter = read(0xFFFD) * 256 + read(0xFFFC);
    std::cout << "Initial program counter: " << r_program_counter << '\n';
    r_stack_pointer = 0xFD;
}

uint8_t CPU::fetch(){
    return mapper->prg_read(r_program_counter);
}

void CPU::execute(uint8_t instruction){
    switch (instruction) {

        case 0xd8:
            #ifdef DEBUG
                std::cout << "CLD" << '\n';
            #endif
            CLD();
            break;
        case 0xA2:
            #ifdef DEBUG
                std::cout << "LDX immediate" << '\n';
            #endif
            LDX(std::bind(&CPU::immediate, this));
            break; // immediate
        case 0xA6:
            #ifdef DEBUG
                std::cout << "LDX zeropage" << '\n';
            #endif
            LDX(std::bind(&CPU::zero_page, this));
            break; // zeropage
        case 0xB6:
            #ifdef DEBUG
                std::cout << "LDX zeropage Y" << '\n';
            #endif
            LDX(std::bind(&CPU::zero_page_y, this));
            break; // zeropage + y register
        case 0xAE:
            #ifdef DEBUG
                std::cout << "LDX absolute" << '\n';
            #endif
            LDX(std::bind(&CPU::absolute, this));
            break; // absolute
        case 0xBE:
            #ifdef DEBUG
                std::cout << "LDX absolute Y" << '\n';
            #endif
            LDX(std::bind(&CPU::absolute_y, this, true));
            break; // absolute + y register


    default:
        break;
    }
}

void CPU::set_status_register(CPU::flags flag, bool status){
    if (status){
        r_status_register |= (1 << flag);
    }else{
        r_status_register &= ~(1 << flag);
    }
}

void CPU::cycle(uint8_t cycles){
    for (int i = 0; i < cycles; i++){
        // 3 ppu calls per loop
    }

}


// Addressing mode //

// each function returns the address of operand

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

void CPU::ADC(std::function<uint16_t()> address){
    uint8_t data = read(address());
    uint8_t carry_bit = r_status_register & 1;
    uint16_t sum = r_accumulator + data + carry_bit;
    set_carry(sum > 0xFF);
    r_accumulator = sum;
    set_overflow((r_accumulator ^ sum) & (data & sum) & 0x80);
    set_zero(r_accumulator == 0);
    set_negative(r_accumulator & 0x80);
}

void CPU::CLD(){
    set_decimal_mode(false);
    cycle(2);
}


void CPU::LDX(std::function<uint16_t()> address){
    uint8_t data = read(address());
    r_index_x = data;
    set_zero(r_index_x == 0);
    set_negative(r_index_x && 0x80);
}

void CPU::debug(){
    std::cout << "--CPU Registers--" << '\n';
    std::cout << "Program Counter: " << r_program_counter << '\n';
    std::cout << "Accumulator: " << r_accumulator << '\n';
    std::cout << "Stack Pointer: " << r_stack_pointer << '\n';
    std::cout << "X Register: " << r_index_x << '\n';
    std::cout << "Y Register: " << r_index_y << '\n';
    std::cout << "Status Register: " << r_status_register << '\n';
}