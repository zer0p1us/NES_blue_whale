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
        case 0x20:
            #ifdef DEBUG
                std::cout << "JSR" << '\n';
            #endif
            JSR(std::bind(&CPU::absolute, this));
        case 0xA9:
            #ifdef DEBUG
                std::cout << "LDA immediate" << '\n';
            #endif
            LDA(std::bind(&CPU::immediate, this));
            break;
        case 0xA5:
            #ifdef DEBUG
                std::cout << "LDA zeropage" << '\n';
            #endif
            LDA(std::bind(&CPU::zero_page, this));
            break;
        case 0xB5:
            #ifdef DEBUG
                std::cout << "LDA zeropage + X" << '\n';
            #endif
            LDA(std::bind(&CPU::zero_page_x, this));
            break;
        case 0xAD:
            #ifdef DEBUG
                std::cout << "LDA absolute" << '\n';
            #endif
            LDA(std::bind(&CPU::absolute, this));
            break;
        case 0xBD:
            #ifdef DEBUG
                std::cout << "LDA absolute + X" << '\n';
            #endif
            LDA(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0xB9:
            #ifdef DEBUG
                std::cout << "LDA absolute + Y" << '\n';
            #endif
            LDA(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0xA1:
            #ifdef DEBUG
                std::cout << "LDA indirect + X" << '\n';
            #endif
            LDA(std::bind(&CPU::indirect_x, this));
            break;
        case 0xB1:
            #ifdef DEBUG
                std::cout << "LDA indirect + Y" << '\n';
            #endif
            LDA(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0xA2:
            #ifdef DEBUG
                std::cout << "LDX immediate" << '\n';
            #endif
            LDX(std::bind(&CPU::immediate, this));
            break;
        case 0xA6:
            #ifdef DEBUG
                std::cout << "LDX zeropage" << '\n';
            #endif
            LDX(std::bind(&CPU::zero_page, this));
            break;
        case 0xB6:
            #ifdef DEBUG
                std::cout << "LDX zeropage + Y" << '\n';
            #endif
            LDX(std::bind(&CPU::zero_page_y, this));
            break;
        case 0xAE:
            #ifdef DEBUG
                std::cout << "LDX absolute" << '\n';
            #endif
            LDX(std::bind(&CPU::absolute, this));
            break;
        case 0xBE:
            #ifdef DEBUG
                std::cout << "LDX absolute Y" << '\n';
            #endif
            LDX(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0x86:
            #ifdef DEBUG
                std::cout << "STX zeropage" << '\n';
            #endif
            STX(std::bind(&CPU::zero_page, this));
            break;
        case 0x96:
            #ifdef DEBUG
                std::cout << "STX zeropage Y" << '\n';
            #endif
            STX(std::bind(&CPU::zero_page_y, this));
            break;
        case 0x8E:
            #ifdef DEBUG
                std::cout << "STX zeropage" << '\n';
            #endif
            STX(std::bind(&CPU::absolute, this));
            break;


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

void CPU::cycle(uint8_t cycles=1){
    for (int i = 0; i < cycles; i++){
        // 3 ppu calls per loop
    }

}


//==Addressing mode==

// each function returns the address

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

// operand contains 2 byte address
uint16_t CPU::absolute(){
    uint8_t least_significant_bit = read(++r_program_counter);
    uint8_t most_significant_bit = read(++r_program_counter);
    return most_significant_bit * 256 + least_significant_bit;
}

// operand + X offset contains 2 byte address
uint16_t CPU::absolute_x(bool extraCycle){
    uint8_t least_significant_bit = read(++r_program_counter);
    uint8_t most_significant_bit = read(++r_program_counter);
    uint16_t address = (most_significant_bit * 256 + least_significant_bit);

    // do extra cycle if crossing page boundary
    if (extraCycle) {
        boundary_check(address, address + r_index_x);
    }

    return address + r_index_x;
}

// operand + Y offset contains 2 byte address
uint16_t CPU::absolute_y(bool extraCycle){
    uint8_t least_significant_bit = read(++r_program_counter);
    uint8_t most_significant_bit = read(++r_program_counter);
    uint16_t address = (most_significant_bit * 256 + least_significant_bit);

    // do extra cycle if crossing page boundary
    if (extraCycle) {
        boundary_check(address, address + r_index_y);
    }

    return address + r_index_y;
}

void CPU::boundary_check(uint16_t address, uint16_t new_address){
    if ((address>>8)!=(new_address>>8)) cycle();
}

// operand contains pointer to address of 2 byte address
uint16_t CPU::indirect(){
    uint16_t pointer = read(++r_program_counter);

    uint8_t least_significant_bit = read(pointer);
    uint8_t most_significant_bit = read(++pointer);

    return read(most_significant_bit * 256 + least_significant_bit );
}

// operand + X offset contains pointer to address of 2 byte address
uint16_t CPU::indirect_x(){
    uint16_t pointer = read(++r_program_counter) + r_index_x;

    uint8_t least_significant_bit = read(pointer);
    uint8_t most_significant_bit = read(++pointer);

    return read(most_significant_bit * 256 + least_significant_bit);
}

// operand + X offset contains pointer to address of 2 byte address
uint16_t CPU::indirect_y(bool extraCycle){
    uint16_t pointer = read(++r_program_counter);

    uint8_t least_significant_bit = read(pointer);
    uint8_t most_significant_bit = read(++pointer);

    uint16_t address = most_significant_bit * 256 + least_significant_bit;

    // do extra cycle if crossing page boundary
    if (extraCycle) {
        boundary_check(address, address + r_index_y);
    }

    return read(address + r_index_y);
}

// next byte contains offset for PC giving operand address
uint16_t CPU::relative(){
    return r_program_counter + read(++r_program_counter);
}

//==Memory Functions==

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

uint8_t CPU::pop(){
    uint8_t data = read(r_stack_pointer+256);
    r_stack_pointer++;
    return data;
}

void CPU::push(uint8_t data){
    r_stack_pointer--;
    write(r_stack_pointer+256, data);
}

//==Opcodes==

void CPU::ADC(std::function<uint16_t()> address){
    uint8_t data = read(address());
    uint8_t carry_bit = r_status_register & 1;
    uint16_t sum = r_accumulator + data + carry_bit;
    set_status_register(f_carry, sum > 0xFF);
    r_accumulator = sum;
    set_status_register(f_overflow, (r_accumulator ^ sum) & (data & sum) & 0x80);
    set_status_register(f_zero, r_accumulator == 0);
    set_status_register(f_negative, r_accumulator & 0x80);
}

void CPU::CLD(){
    set_status_register(f_decimal_mode, false);
    cycle(2);
}


void CPU::JSR(std::function<uint16_t()> address){
    uint8_t least_significant_bit = r_program_counter & 0xFF;
    uint8_t most_significant_bit = r_program_counter & 0xFF00;
    push(least_significant_bit);
    push(most_significant_bit);
    r_program_counter = address() - 1;
    cycle();
}

void CPU::LDA(std::function<uint16_t()> address){
    r_accumulator = read(address());
}

void CPU::LDX(std::function<uint16_t()> address){
    uint8_t data = read(address());
    r_index_x = data;
    set_status_register(f_zero, r_index_x == 0);
    set_status_register(f_negative, r_index_x && 0x80);
}

void CPU::STX(std::function<uint16_t()> address){
    write(address(), r_index_x);
}

//==Debug==
void CPU::debug(){
    std::cout << "--CPU Registers--" << '\n';
    std::cout << "Program Counter: " << r_program_counter << '\n';
    std::cout << "Accumulator: " << r_accumulator << '\n';
    std::cout << "Stack Pointer: " << r_stack_pointer << '\n';
    std::cout << "X Register: " << r_index_x << '\n';
    std::cout << "Y Register: " << r_index_y << '\n';
    std::cout << "Status Register: " << r_status_register << '\n';
}