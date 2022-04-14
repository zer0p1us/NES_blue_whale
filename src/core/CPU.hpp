#ifndef CPU_HEADER
#define CPU_HEADER

#include <vector>
#include <stdint.h>
#include <functional>
#include <string>

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
        void cycle(uint8_t);

        uint8_t fetch();
        void execute(uint8_t);

        std::string debug();

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
            f_carry = 0, // on carry is on
            f_zero =  1, // on if last instruction resulted in zero
            f_interupt_disabled =  2, // on to ingore IRQ
            f_decimal_mode =  3,
            f_break_4 = 4, // on BRK instruction was executed
            f_break_5 = 5,
            f_overflow = 6, // overflow of 2's complement
            f_negative = 7, // on if sign bit is on (bit 7)
        };

        void set_status_register(CPU::flags flag, bool status);

        RAM ram;
        Mapper* mapper;
        PPU* ppu;

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);

        uint8_t pop();
        void push(uint8_t data);

        // addressing modes

        uint16_t immediate(); // PC + 2

        uint16_t zero_page();
        uint16_t zero_page_x();
        uint16_t zero_page_y();

        uint16_t absolute();
        uint16_t absolute_x(bool extraCycle);
        uint16_t absolute_y(bool extraCycle);

        void boundary_check(uint16_t address, uint16_t new_address); // if page boundary has been crossed call cycle()

        uint16_t indirect();
        uint16_t indirect_x();
        uint16_t indirect_y(bool extraCycle);

        uint16_t relative();

        void ADC(std::function<uint16_t()> address); // Add with carry
        void AND(std::function<uint16_t()> address); // AND with accumulator
        void ASL(std::function<uint16_t()> address); // Arithmetic Shift Left
        void BCC(std::function<uint16_t()> address); // Branch on Carry Clear
        void BCS(std::function<uint16_t()> address); // Branch on Carry Set
        void BEQ(std::function<uint16_t()> address); // Branchg on Results Zero
        void BIT(std::function<uint16_t()> address); // Test Bits in Memory with Accumulator
        void BMI(std::function<uint16_t()> address); // Branch on Result Minus
        void CLD(); // Clear Decimal Mode
        void JSR(std::function<uint16_t()> address); // Jump and save return address
        void LDA(std::function<uint16_t()> address); // Load Data into Accumulator
        void LDX(std::function<uint16_t()> address); // Load Data into X register
        void STX(std::function<uint16_t()> address); // Store X register to memory




};
#endif