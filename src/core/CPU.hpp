#ifndef CPU_HEADER
#define CPU_HEADER
#include <vector>
#include <stdint.h>


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
        uint16_t r_program_counter;
        uint8_t r_stack_pointer;
        uint8_t r_accumulator;
        uint8_t r_index_x;
        uint8_t r_index_y;

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

        void ADC(){

        }


};
#endif