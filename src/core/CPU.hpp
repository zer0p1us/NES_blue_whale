#ifndef CPU_HEADER
#define CPU_HEADER
#include "../util/typedefs.hpp"
#include <vector>


class CPU {

    public:
        CPU();
        ~CPU() { };

        enum InteruptType{
            IRQ,    // hardware interput
            NMI,    //
            RESET   //
        };

        void step();

    private:

        //Registers
        Address r_program_counter;
        Byte r_stack_pointer;
        Byte r_accumulator;
        Byte r_index_x;
        Byte r_index_y;

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
#endif