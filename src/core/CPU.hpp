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
        void reset(); // set all registers to initial state

        uint8_t fetch();
        void execute(uint8_t);


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
        enum flags{
            f_carry = 0, // on carry os on
            f_zero =  1, // on if last instruction resulted in zero
            f_interupt_disabled =  2, // on to ingore IRQ
            f_decimal_mode =  3,
            f_break_4 = 4, // on BRK instruction was executed
            f_break_5 = 5,
            f_overflow = 6, // overflow of 2's complement
            f_negative = 7, // on if sign bit is on (bit 7)
        };

        void set_status_register(CPU::flags flag, bool status);

        void set_carry(bool status) { set_status_register(flags::f_carry, status); }
        void set_zero(bool status) { set_status_register(flags::f_zero, status); }
        void set_interupt_desabled(bool status) { set_status_register(flags::f_interupt_disabled, status); }
        void set_decimal_mode(bool status) { set_status_register(flags::f_decimal_mode, status); }
        void set_break_4(bool status) { set_status_register(flags::f_break_4, status); }
        void set_break_5(bool status) { set_status_register(flags::f_break_5, status); }
        void set_overflow(bool status) { set_status_register(flags::f_overflow, status); }
        void set_negative(bool status) { set_status_register(flags::f_negative, status); }

        RAM ram;
        Mapper* mapper;
        PPU* ppu;

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);

        // addressing modes

        uint16_t immediate(); // PC + 2

        uint16_t zero_page();
        uint16_t zero_page_x();
        uint16_t zero_page_y();

        uint16_t absolute();
        uint16_t absolute_x(bool);
        uint16_t absolute_y(bool);

        uint16_t indirectX();
        uint16_t indirectY(bool);

        uint16_t relative();


        enum opcode{

        };

        void ADC(){

        }


};
#endif