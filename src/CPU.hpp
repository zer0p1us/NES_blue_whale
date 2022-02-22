#ifndef CPU_HPP
#define CPU_HPP
#include "Cartridge.hpp"

namespace bw{

    class CPU {

    public:

        enum InteruptType{
            IRQ,    //hardware interput
            NMI,    //
            RESET   //
        };

        CPU(/* args */);
        ~CPU();

    private:

        //Registers
        bw::Address r_program_counter;
        bw::Byte r_stack_pointer;
        bw::Byte r_accumulator;
        bw::Byte r_index_x;
        bw::Byte r_index_y;

        //Flags
        //Status flags.
        //Is storing them in one byte better ?
        bool f_carry;
        bool f_zero; //on if last instruction resulted in zero
        bool f_interupt_disabled; //on to ingore IRQ
        bool f_decimal_mode;
        bool f_break; //on BRK instruction was executed

        bool f_overflow; //overflow of 2's complement
        bool f_sign; //on if sign bit is on (bit 7)


        enum opcode{

        };

    };

}
#endif