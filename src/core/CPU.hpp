#ifndef CPU_HEADER
#define CPU_HEADER

#include <vector>
#include <stdint.h>
#include <functional>

#include "RAM.hpp"
#include "Mapper.hpp"
#include "PPU.hpp"


class CPU {

    public:
        CPU(Mapper* mapper, PPU* ppu): mapper(mapper), ppu(ppu) { }
        ~CPU() { };

        enum InteruptType{
            IRQ,    // hardware interput
            NMI,    //
            RESET   //
        };

        void step();

    private:

        //Registers
        uint16_t r_program_counter;
        uint8_t r_stack_pointer = 0xFD; // documented starting point
        uint8_t r_accumulator;
        uint8_t r_index_x;
        uint8_t r_index_y;
        uint8_t r_status_register = 0x24; // documented stating point

        //Flags
        //Status flags.
        //Is storing them in one byte better ?
        RAM ram;
        Mapper* mapper;
        PPU* ppu;

        bool f_overflow; //overflow of 2's complement
        bool f_sign; //on if sign bit is on (bit 7)


        enum opcode{

        };

        void ADC(){

        }


};
#endif