#include "CPU.hpp"

#include <iostream>
#include <string>

#include "../debug.hpp"
#include "ROM.hpp"

void CPU::step(){
    // if (ppu->genNMI()){
    //     NMI();
    //     CPU::cycles = 0;
    // }
    execute(fetch());
    ++r_program_counter;
}

void CPU::reset(){
    r_program_counter = read(0xFFFD) * 256 + read(0xFFFC);
    char temp[4];
    sprintf(temp, "%X", r_program_counter);
    std::cout << "Initial program counter: " << temp << '\n';
    r_stack_pointer = 0xFF;
    cycles = 0;
}

uint8_t CPU::fetch(){
    cycle();
    return mapper->prg_read(r_program_counter);
}

void CPU::execute(uint8_t instruction){
    switch (instruction) {

        case 0x69:
            debug_out("ADC immediate");
            ADC(std::bind(&CPU::immediate, this));
            break;
        case 0x65:
            debug_out("ADC zeropage");
            ADC(std::bind(&CPU::zeropage, this));
            break;
        case 0x75:
            debug_out("ADC zeropage + X");
            ADC(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x6D:
            debug_out("ADC absolute");
            ADC(std::bind(&CPU::absolute, this));
            break;
        case 0x7D:
            debug_out("ADC absolute + X");
            ADC(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0x79:
            debug_out("ADC absolute + Y");
            ADC(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0x61:
            debug_out("ADC indirect + X");
            ADC(std::bind(&CPU::indirect_x, this));
            break;
        case 0x71:
            debug_out("ADC indirect + Y");
            ADC(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0x29:
            debug_out("AND immediate");
            AND(std::bind(&CPU::immediate, this));
            break;
        case 0x25:
            debug_out("AND zeropage");
            AND(std::bind(&CPU::zeropage, this));
            break;
        case 0x35:
            debug_out("AND zeropage + X");
            AND(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x2D:
            debug_out("AND absolute");
            AND(std::bind(&CPU::absolute, this));
            break;
        case 0x3D:
            debug_out("AND absolute + X");
            AND(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0x39:
            debug_out("AND absolute + Y");
            AND(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0x21:
            debug_out("AND indirect + X");
            AND(std::bind(&CPU::indirect_x, this));
            break;
        case 0x31:
            debug_out("AND indirect + Y");
            AND(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0x0A:
            debug_out("ASL accumulator");
            ASL(nullptr); // accumulator
            break;
        case 0x06:
            debug_out("ASL zeropage");
            ASL(std::bind(&CPU::zeropage, this));
            break;
        case 0x16:
            debug_out("ASL zeropage + X");
            ASL(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x0E:
            debug_out("ASL absolute");
            ASL(std::bind(&CPU::absolute, this));
            break;
        case 0x1E:
            debug_out("ASL absolute + X");
            ASL(std::bind(&CPU::absolute_x, this, false));
            cycle();
            break;
        case 0x90:
            debug_out("BCC");
            BCC(std::bind(&CPU::relative, this));
            break;
        case 0xB0:
            debug_out("BCS");
            BCS(std::bind(&CPU::relative, this));
            break;
        case 0xF0:
            debug_out("BEQ");
            BEQ(std::bind(&CPU::relative, this));
            break;
        case 0x24:
            debug_out("BIT zeropage");
            BIT(std::bind(&CPU::zeropage, this));
            break;
        case 0x2C:
            debug_out("BIT absolute");
            BIT(std::bind(&CPU::absolute, this));
            break;
        case 0x30:
            debug_out("BMI");
            BMI(std::bind(&CPU::relative, this));
            break;
        case 0xD0:
            debug_out("BNE");
            BNE(std::bind(&CPU::relative, this));
            break;
        case 0x10:
            debug_out("BPL");
            BPL(std::bind(&CPU::relative, this));
            break;
        case 0x00:
            debug_out("BRK");
            BRK();
            break;
        case 0x50:
            debug_out("BVC");
            BVC(std::bind(&CPU::relative, this));
            break;
        case 0x70:
            debug_out("BVS");
            BVS(std::bind(&CPU::relative, this));
            break;
        case 0x18:
            debug_out("CLC");
            CLC();
            break;
        case 0xd8:
            debug_out("CLD");
            CLD();
            break;
        case 0x58:
            debug_out("CLI");
            CLI();
            break;
        case 0xB8:
            debug_out("CLV");
            CLV();
            break;
        case 0xC9:
            debug_out("CMP immediate");
            CMP(std::bind(&CPU::immediate, this));
            break;
        case 0xC5:
            debug_out("CMP zeropage");
            CMP(std::bind(&CPU::zeropage, this));
            break;
        case 0xD5:
            debug_out("CMP zeropage + X");
            CMP(std::bind(&CPU::zeropage_x, this));
            break;
        case 0xCD:
            debug_out("CMP absolute");
            CMP(std::bind(&CPU::absolute, this));
        case 0xDD:
            debug_out("CMP absolute + X");
            CMP(std::bind(&CPU::absolute_x, this, true));
        case 0xD9:
            debug_out("CMP absolute + Y");
            CMP(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0xC1:
            debug_out("CMP indirect + Y");
            CMP(std::bind(&CPU::indirect_x, this));
            break;
        case 0xD1:
            debug_out("CMP indirect + Y");
            CMP(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0xE0:
            debug_out("CPX immediate");
            CPX(std::bind(&CPU::immediate, this));
            break;
        case 0xE4:
            debug_out("CPX zeropage");
            CPX(std::bind(&CPU::zeropage, this));
            break;
        case 0xEC:
            debug_out("CPX absolute");
            CPX(std::bind(&CPU::absolute, this));
            break;
        case 0xC0:
            debug_out("CPY immediate");
            CPY(std::bind(&CPU::immediate, this));
            break;
        case 0xC4:
            debug_out("CPY zeropage");
            CPY(std::bind(&CPU::zeropage, this));
            break;
        case 0xCC:
            debug_out("CPY absolute");
            CPY(std::bind(&CPU::absolute, this));
            break;
        case 0xC6:
            debug_out("DEC zeropage");
            DEC(std::bind(&CPU::zeropage, this));
            break;
        case 0xD6:
            debug_out("DEC zeropage + X");
            DEC(std::bind(&CPU::zeropage_x, this));
            break;
        case 0xCE:
            debug_out("DEC absolute");
            DEC(std::bind(&CPU::absolute, this));
            break;
        case 0xDE:
            debug_out("DEC absolute + X");
            DEC(std::bind(&CPU::absolute_x, this, false));
            cycle();
            break;
        case 0xCA:
            debug_out("DEX");
            DEX();
            break;
        case 0x88:
            debug_out("DEY");
            DEY();
            break;
        case 0x49:
            debug_out("EOR immediate");
            EOR(std::bind(&CPU::immediate, this));
            break;
        case 0x45:
            debug_out("EOR zeropage");
            EOR(std::bind(&CPU::zeropage, this));
            break;
        case 0x55:
            debug_out("EOR zeropage + X");
            EOR(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x4D:
            debug_out("EOR absolute");
            EOR(std::bind(&CPU::absolute, this));
            break;
        case 0x5D:
            debug_out("EOR absolute + X");
            EOR(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0x59:
            debug_out("EOR absolute + Y");
            EOR(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0x41:
            debug_out("EOR indirect + X");
            EOR(std::bind(&CPU::indirect_x, this));
            break;
        case 0x51:
            debug_out("EOR indirect + Y");
            EOR(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0xE6:
            debug_out("INC zeropage");
            INC(std::bind(&CPU::zeropage, this));
            break;
        case 0xF6:
            debug_out("INC zeropage + X");
            INC(std::bind(&CPU::zeropage_x, this));
            break;
        case 0xEE:
            debug_out("INC absolute");
            INC(std::bind(&CPU::absolute, this));
            break;
        case 0xFE:
            debug_out("INC absolute + X");
            INC(std::bind(&CPU::absolute_x, this, false));
            cycle();
            break;
        case 0xE8:
            debug_out("INX");
            INX();
            break;
        case 0xC8:
            debug_out("INY");
            INY();
            break;
        case 0x4C:
            debug_out("JMP absolute");
            JMP(std::bind(&CPU::absolute, this));
            break;
        case 0x6C:
            debug_out("JMP indirect");
            JMP(std::bind(&CPU::indirect, this));
            break;
        case 0x20:
            debug_out("JSR");
            JSR(std::bind(&CPU::absolute, this));
            break;
        case 0xA9:
            debug_out("LDA immediate");
            LDA(std::bind(&CPU::immediate, this));
            break;
        case 0xA5:
            debug_out("LDA zeropage");
            LDA(std::bind(&CPU::zeropage, this));
            break;
        case 0xB5:
            debug_out("LDA zeropage + X");
            LDA(std::bind(&CPU::zeropage_x, this));
            break;
        case 0xAD:
            debug_out("LDA absolute");
            LDA(std::bind(&CPU::absolute, this));
            break;
        case 0xBD:
            debug_out("LDA absolute + X");
            LDA(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0xB9:
            debug_out("LDA absolute + Y");
            LDA(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0xA1:
            debug_out("LDA indirect + X");
            LDA(std::bind(&CPU::indirect_x, this));
            break;
        case 0xB1:
            debug_out("LDA indirect + Y");
            LDA(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0xA2:
            debug_out("LDX immediate");
            LDX(std::bind(&CPU::immediate, this));
            break;
        case 0xA6:
            debug_out("LDX zeropage");
            LDX(std::bind(&CPU::zeropage, this));
            break;
        case 0xB6:
            debug_out("LDX zeropage + Y");
            LDX(std::bind(&CPU::zeropage_y, this));
            break;
        case 0xAE:
            debug_out("LDX absolute");
            LDX(std::bind(&CPU::absolute, this));
            break;
        case 0xBE:
            debug_out("LDX absolute Y");
            LDX(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0xA0:
            debug_out("LDY immediate");
            LDY(std::bind(&CPU::immediate, this));
            break;
        case 0xA4:
            debug_out("LDY zeropage");
            LDY(std::bind(&CPU::zeropage, this));
            break;
        case 0xB4:
            debug_out("LDY zeropage + X");
            LDY(std::bind(&CPU::zeropage_x, this));
            break;
        case 0xAC:
            debug_out("LDY absolute");
            LDY(std::bind(&CPU::absolute, this));
            break;
        case 0xBC:
            debug_out("LDY absolute + X");
            LDY(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0x4A:
            debug_out("LSR accumulator");
            LSR(nullptr);
            break;
        case 0x46:
            debug_out("LSR zeropage");
            LSR(std::bind(&CPU::zeropage, this));
            break;
        case 0x56:
            debug_out("LSR zeropage + X");
            LSR(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x4E:
            debug_out("LSR absolute");
            LSR(std::bind(&CPU::absolute, this));
            break;
        case 0x5E:
            debug_out("LSR absolute + X");
            LSR(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0xEA:
            debug_out("NOP");
            NOP(nullptr);
            break;
        case 0x09:
            debug_out("ORA immediate");
            ORA(std::bind(&CPU::immediate, this));
            break;
        case 0x05:
            debug_out("ORA zeropage");
            ORA(std::bind(&CPU::zeropage, this));
            break;
        case 0x15:
            debug_out("ORA zeropage + X");
            ORA(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x0D:
            debug_out("ORA absolute");
            ORA(std::bind(&CPU::absolute, this));
            break;
        case 0x1D:
            debug_out("ORA absolute + X");
            ORA(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0x19:
            debug_out("ORA absolute + Y");
            ORA(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0x01:
            debug_out("ORA indirect + X");
            ORA(std::bind(&CPU::indirect_x, this));
            break;
        case 0x11:
            debug_out("ORA indirect + Y");
            ORA(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0x48:
            debug_out("PHA");
            PHA();
            break;
        case 0x08:
            debug_out("PHP");
            PHP();
            break;
        case 0x68:
            debug_out("PLA");
            PLA();
            break;
        case 0x28:
            debug_out("PLP");
            PLP();
            break;
        case 0x2A:
            debug_out("ROL accumulator");
            ROL(nullptr);
            break;
        case 0x26:
            debug_out("ROL zeropage");
            ROL(std::bind(&CPU::zeropage, this));
            break;
        case 0x36:
            debug_out("ROL zeropage + X");
            ROL(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x2E:
            debug_out("ROL absolute");
            ROL(std::bind(&CPU::absolute, this));
            break;
        case 0x3E:
            debug_out("ROL absolute + X");
            ROL(std::bind(&CPU::absolute_x, this, false));
            cycle();
            break;
        case 0x6A:
            debug_out("ROR accumulator");
            ROR(nullptr); break;
        case 0x66:
            debug_out("ROR zeropage");
            ROR(std::bind(&CPU::zeropage, this));
            break;
        case 0x76:
            debug_out("ROR zeropage + X");
            ROR(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x6E:
            debug_out("ROR absolute");
            ROR(std::bind(&CPU::absolute, this));
            break;
        case 0x7E:
            debug_out("ROR absolute + X");
            ROR(std::bind(&CPU::absolute_x, this, false));
            cycle();
            break;
        case 0x40:
            debug_out("RTI");
            RTI();
            break;
        case 0x60:
            debug_out("RTS");
            RTS();
            break;
        case 0xE9:
            debug_out("SBC immediate");
            SBC(std::bind(&CPU::immediate, this));
            break;
        case 0xE5:
            debug_out("SBC zeropage");
            SBC(std::bind(&CPU::zeropage, this));
            break;
        case 0xF5:
            debug_out("SBC zeropage + X");
            SBC(std::bind(&CPU::zeropage_x, this));
            break;
        case 0xED:
            debug_out("SBC absolute");
            SBC(std::bind(&CPU::absolute, this));
            break;
        case 0xFD:
            debug_out("SBC absolute + X");
            SBC(std::bind(&CPU::absolute_x, this, true));
            break;
        case 0xF9:
            debug_out("SBC absolute + Y");
            SBC(std::bind(&CPU::absolute_y, this, true));
            break;
        case 0xE1:
            debug_out("SBC indirect + X");
            SBC(std::bind(&CPU::indirect_x, this));
            break;
        case 0xF1:
            debug_out("SBC indirect + Y");
            SBC(std::bind(&CPU::indirect_y, this, true));
            break;
        case 0x38:
            debug_out("SEC");
            SEC();
            break;
        case 0xF8:
            debug_out("SED");
            SED();
            break;
        case 0x78:
            debug_out("SEI");
            SEI();
            break;
        case 0x85:
            debug_out("STA zeropage");
            STA(std::bind(&CPU::zeropage, this));
            break;
        case 0x95:
            debug_out("STA zeropage + X");
            STA(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x8D:
            debug_out("STA absolute");
            STA(std::bind(&CPU::absolute, this));
            break;
        case 0x9D:
            debug_out("STA absolute + X");
            STA(std::bind(&CPU::absolute_x, this, false));
            cycle();
            break;
        case 0x99:
            debug_out("STA absolute + Y");
            STA(std::bind(&CPU::absolute_y, this, false));
            cycle();
            break;
        case 0x81:
            debug_out("STA indirect + X");
            STA(std::bind(&CPU::indirect_x, this));
            break;
        case 0x91:
            debug_out("STA indirect + Y");
            STA(std::bind(&CPU::indirect_y, this, false));
            break;
        case 0x86:
            debug_out("STX zeropage");
            STX(std::bind(&CPU::zeropage, this));
            break;
        case 0x96:
            debug_out("STX zeropage + Y");
            STX(std::bind(&CPU::zeropage_y, this));
            break;
        case 0x8E:
            debug_out("STX absolute");
            STX(std::bind(&CPU::absolute, this));
            break;
        case 0x84:
            debug_out("STY zeropage");
            STY(std::bind(&CPU::zeropage, this));
            break;
        case 0x94:
            debug_out("STY zeropage + X");
            STY(std::bind(&CPU::zeropage_x, this));
            break;
        case 0x8C:
            debug_out("STY absolute");
            STY(std::bind(&CPU::absolute, this));
            break;
        case 0xAA:
            debug_out("TAX");
            TAX();
            break;
        case 0xA8:
            debug_out("TAY");
            TAY();
            break;
        case 0xBA:
            debug_out("TSX");
            TSX();
            break;
        case 0x8A:
            debug_out("TXA");
            TXA();
            break;
        case 0x9A:
            debug_out("TXS");
            TXS();
            break;
        case 0x98:
            debug_out("TYA");
            TYA();
            break;


    default:
        char err[2];
        sprintf(err, "%X", instruction);
        debug_out(static_cast<std::string>(err) + " opcode not recognised");
        break;
    }
    debug_out(CPU::debug());
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
        // 3 PPU cycles per CPU cycles
        ppu->step();
        ppu->step();
        ppu->step();
    }
    CPU::cycles += cycles;

}

void CPU::NMI(){
    SEI();
    push(r_program_counter & 0xFF);
    push(r_program_counter >> 8);
    push(r_status_register);
    r_program_counter = read(0xFFFB) * 256 + read(0xFFFA);
    cycle();
}

//==Addressing mode==

// each function returns the address

// next byte contains operand
uint16_t CPU::immediate(){
    return ++r_program_counter;
}

// next byte contains address of first 256 bytes containing the data
uint16_t CPU::zeropage(){
    return read(++r_program_counter) % 256;
}

// next byte contains address of first 256 bytes with x register offset containing the data
uint16_t CPU::zeropage_x(){
    cycle();
    uint8_t zeropage = read(++r_program_counter);
    return (zeropage + r_index_x) % 256;
}

// next byte contains address of first 256 bytes with y register offset containing the data
uint16_t CPU::zeropage_y(){
    cycle();
    uint8_t zeropage = read(++r_program_counter);
    return (zeropage + r_index_y) % 256;
}

// operand contains 2 byte address
uint16_t CPU::absolute(){
    uint8_t low_byte = read(++r_program_counter);
    uint8_t high_byte = read(++r_program_counter);
    return high_byte * 256 + low_byte;
}

// operand + X offset contains 2 byte address
uint16_t CPU::absolute_x(bool extraCycle){
    uint8_t low_byte = read(++r_program_counter);
    uint8_t high_byte = read(++r_program_counter);
    uint16_t address = (high_byte * 256 + low_byte);

    // do extra cycle if crossing page boundary
    if (extraCycle) {
        boundary_check(address, address + r_index_x);
    }

    return address + r_index_x;
}

// operand + Y offset contains 2 byte address
uint16_t CPU::absolute_y(bool extraCycle){
    uint8_t low_byte = read(++r_program_counter);
    uint8_t high_byte = read(++r_program_counter);
    uint16_t address = (high_byte * 256 + low_byte);

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
    uint8_t low_byte = read(++r_program_counter);
    uint8_t high_byte = read(++r_program_counter);

    uint16_t pointer = high_byte * 256 + low_byte;

    low_byte = read(pointer);
    high_byte = read(++pointer);
    return (high_byte * 256 + low_byte );
}

// operand + X offset contains pointer to address of 2 byte address
uint16_t CPU::indirect_x(){
    cycle();
    uint16_t pointer = read(++r_program_counter) + r_index_x;

    uint8_t low_byte = read(pointer);
    uint8_t high_byte = read(++pointer);

    return (high_byte * 256 + low_byte);
}

// operand + Y offset contains pointer to address of 2 byte address
uint16_t CPU::indirect_y(bool extraCycle){
    uint16_t pointer = read(++r_program_counter);

    uint8_t low_byte = read(pointer);
    uint8_t high_byte = read(++pointer);

    uint16_t address = high_byte * 256 + low_byte;

    // do extra cycle if crossing page boundary
    if (extraCycle) {
        boundary_check(address, address + r_index_y);
    }

    return (address + r_index_y);
}

// next byte contains offset for PC giving operand address
uint16_t CPU::relative(){
    int8_t offset = read(++r_program_counter); // operand is in two's complement
    return r_program_counter + offset;
}

//==Memory Functions==

uint8_t CPU::read(uint16_t address){
    cycle(); // 1 cycle per read
    if (address >= 0x0000 & address < 0x2000){ // reading from RAM address
        return ram.read(address);
    } else if (address >= 0x2000 & address < 0x4000){ // reading from PPU RAM address
        return ppu->read(address);
    } else if (address >= 0x4016 & address < 0x4018){ // reading form input IO
        return io->read(address);
    } else if (address >= 0x8000 & address <= 0xFFFD){ // reading from PROGRAM ROM address
        return mapper->prg_read(address);
    }

    else{
        return 0;
    }

}

void CPU::write(uint16_t address, uint8_t data){
    cycle(); // 1 cycle per write
    if (address >= 0x0000 & address < 0x2000 ){ // writing to RAM addresses
        ram.write(address, data);
    } else if (address >= 0x2000 & address < 0x4000){ // writing to PPU RAM addresses
        ppu->write(address, data);
    } else if (address >= 0x4016 & address < 0x4018) { // writing to input IO
        io->write(address, data);
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

/*  Add with Carry
    r_accumulator = r_accumulator + Mem
    flags: carry, negative, overflow, zero */
void CPU::ADC(std::function<uint16_t()> address){
    uint8_t data = read(address()); // get uint8_t to add to ACC
    uint8_t carry_bit = r_status_register & 1; // get carry bit
    uint16_t sum = r_accumulator + data + carry_bit;
    set_status_register(f_carry, sum > 0xFF); // check if there is overflow or underflow
    r_accumulator = sum;
    set_status_register(f_overflow, (r_accumulator ^ sum) & (data & sum) & 0x80); // check if there is overflow
    set_status_register(f_zero, r_accumulator == 0);
    set_status_register(f_negative, r_accumulator & 0x80); // check if ACC is negative
}

/*  Logical AND on accumulator and mem
    r_accumulator = r_accumulator & mem
    flags: zero, negative*/
void CPU::AND(std::function<uint16_t()> address){
    r_accumulator &= read(address());
    set_status_register(f_zero, r_accumulator == 0);
    set_status_register(f_negative, r_accumulator & 0x80);
}

/*  Arithmetic Shift Left
    r_accumulator or Mem
    flags: negative, zero, carry */
void CPU::ASL(std::function<uint16_t()> address){
    if (address == nullptr){ // accumulator
        set_status_register(CPU::f_carry, r_accumulator & 0x80); // set carry if 7th bit is 1 in ACC
        r_accumulator <<= 1;
        set_status_register(CPU::f_zero, r_accumulator == 0);
        set_status_register(CPU::f_negative, r_accumulator & 0x80);
    } else{ // address
        uint16_t write_address = address();
        uint8_t data = read(address());
        set_status_register(CPU::f_carry, data & 0x80); // set carry if 7th bit is 1 in data
        data << 1;
        set_status_register(CPU::f_zero, data == 0);
        set_status_register(CPU::f_negative, data & 0x80);
        write(write_address, data);
    }
    cycle();
}

// Branch on f_carry = 0
void CPU::BCC(std::function<uint16_t()> address){
    uint8_t f_carry = (CPU::r_status_register >> CPU::f_carry) & 1;
    if (!f_carry){
        boundary_check(r_program_counter, r_program_counter + address());
        r_program_counter = address();
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();
}

// Branch on f_carry = 1
void CPU::BCS(std::function<uint16_t()> address){
    uint8_t f_carry = (CPU::r_status_register >> CPU::f_carry) & 1;
    if (f_carry){
        boundary_check(r_program_counter, r_program_counter + address());
        r_program_counter = address();
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();

}

// Branch on f_zero = 1
void CPU::BEQ(std::function<uint16_t()> address){
    uint8_t f_zero = (CPU::r_status_register >> CPU::f_zero) & 1;
    if (f_zero){
        uint16_t jump_address = address();
        boundary_check(r_program_counter, r_program_counter + jump_address);
        r_program_counter = jump_address;
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();
}

// bits 7 and 6 of operand are transferred to bit 7 and 6 of SR (N,V);
// the zero-flag is set to the result of operand AND accumulator.
void CPU::BIT(std::function<uint16_t()> address){
    uint8_t test_bit = read(address());
    CPU::set_status_register(CPU::f_negative, (test_bit >> CPU::f_negative) & 1);
    CPU::set_status_register(CPU::f_overflow, (test_bit >> CPU::f_overflow) & 1);
    CPU::set_status_register(CPU::f_zero, (test_bit & r_accumulator) == 0);
}

// Branch on f_negative = 1
void CPU::BMI(std::function<uint16_t()> address){
    uint8_t f_negative = (CPU::r_status_register >> CPU::f_negative) & 1;
    if (f_negative){
        uint16_t jump_address = address();
        boundary_check(r_program_counter, r_program_counter + jump_address);
        r_program_counter = jump_address;
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();
}

// Branch on f_zero = 0
void CPU::BNE(std::function<uint16_t()> address){
    uint8_t f_zero = (CPU::r_status_register >> CPU::f_zero) & 1;
    if (!f_zero){
        uint16_t jump_address = address();
        boundary_check(r_program_counter, r_program_counter + jump_address);
        r_program_counter = jump_address;
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();
}

// Branch on f_negative = 0
void CPU::BPL(std::function<uint16_t()> address){
    uint8_t f_negative = (CPU::r_status_register >> CPU::f_negative) & 1;
    if (!f_negative){
        uint16_t jump_address = address();
        boundary_check(r_program_counter, r_program_counter + jump_address);
        r_program_counter = jump_address;
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();
}

/*  Break
    push PC+2 and Status Register to Stack*/
void CPU::BRK(){
    set_status_register(CPU::f_break_4, 1);
    CPU::push(r_program_counter + 2);
    CPU::push(r_status_register);
    uint8_t high_byte = read(0xFFFE);
    uint8_t low_byte = read(0xFFFF);
    r_program_counter = high_byte * 256  + low_byte;
    cycle(2);
}

// Branch on f_overflow = 0
void CPU::BVC(std::function<uint16_t()> address){
    uint8_t f_overflow = (CPU::r_status_register >> CPU::f_overflow) & 1;
    if (!f_overflow){
        uint16_t jump_address = address();
        boundary_check(r_program_counter, r_program_counter + jump_address);
        r_program_counter = jump_address;
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();
}

// Branch on f_overflow = 1
void CPU::BVS(std::function<uint16_t()> address){
    uint8_t f_overflow = (CPU::r_status_register >> CPU::f_overflow) & 1;
    if (f_overflow){
        uint16_t jump_address = address();
        boundary_check(r_program_counter, r_program_counter + jump_address);
        r_program_counter = jump_address;
    }else{
        // if branch does not occur the next byte will be relative address
        // PC needs to move on after it
        r_program_counter++;
    }
    cycle();
}

// set f_carry = 0
void CPU::CLC(){
    set_status_register(f_carry, 0);
    cycle();
}

// Set f_decimal_mode = 0
void CPU::CLD(){
    set_status_register(f_decimal_mode, 0);
    cycle();
}

// Set f_interrupt_disabled = 0
void CPU::CLI(){
    set_status_register(f_interrupt_disabled, 0);
    cycle();
}

// Set f_overflow = 0
void CPU::CLV(){
    set_status_register(f_overflow, 0);
    cycle();
}

/*  A - Mem
    flags: f_negative, f_carry, f_zero */
void CPU::CMP(std::function<uint16_t()> address){
    uint8_t data = read(address());
    set_status_register(f_negative, (r_accumulator - data) & 0x80);
    set_status_register(f_carry, r_accumulator < data);
    set_status_register(f_zero, r_accumulator == data);
}

/*  X - Mem
    flags: f_negative, f_carry, f_zero */
void CPU::CPX(std::function<uint16_t()> address){
    uint8_t data = read(address());
    set_status_register(f_negative, (r_index_x - data) & 0x80);
    set_status_register(f_carry, r_index_x >= data);
    set_status_register(f_zero, r_index_x == data);
}

/*  Y - Mem
    flags: f_negative, f_carry, f_zero */
void CPU::CPY(std::function<uint16_t()> address){
    uint8_t data = read(address());
    set_status_register(f_negative, (r_index_y - data) & 0x80);
    set_status_register(f_carry, r_index_y < data);
    set_status_register(f_zero, r_index_y == data);
}

/*  Decrement Mem by 1
    Mem = Mem - 1 */
void CPU::DEC(std::function<uint16_t()> address){
    uint16_t addr = address();
    uint8_t data = read(addr) - 1;
    set_status_register(f_negative, data & 0x80);
    set_status_register(f_zero, data == 0);
    write(addr, data);
    cycle();
}

/*  Decrement r_index_x by 1
    r_index_x = r_index_x - 1 */
void CPU::DEX(){
    r_index_x--;
    set_status_register(f_negative, r_index_x & 0x80);
    set_status_register(f_zero, r_index_x == 0);
    cycle();
}

/*  Decrement r_index_x by 1
    r_index_x = r_index_x - 1 */
void CPU::DEY(){
    r_index_y--;
    set_status_register(f_negative, r_index_y & 0x80);
    set_status_register(f_zero, r_index_y == 0);
    cycle();
}

/*  Exclusive OR of r_accumulator and Mem
    r_accumulator = r_accumulator XOR Mem*/
void CPU::EOR(std::function<uint16_t()> address){
    r_accumulator ^= read(address());
    set_status_register(f_negative, r_accumulator & 0x80);
    set_status_register(f_zero, r_accumulator == 0);
}

/*  increment Mem by 1
    Mem = Mem + 1
    flags: f_negative, f_zero */
void CPU::INC(std::function<uint16_t()> address){
    uint16_t addr = address();
    uint8_t data = read(addr) + 1;
    set_status_register(f_negative, data & 0x80);
    set_status_register(f_zero, data == 0);
    write(addr, data);
    cycle();
}

/*  Increment r_index_x by 1
    r_index_x = r_index_x + 1
    flags: f_negative, f_zero */
void CPU::INX(){
    r_index_x++;
    set_status_register(f_negative, r_index_x & 0x80);
    set_status_register(f_zero, r_index_x == 0);
    cycle();
}

/*  Increment r_index_y by 1
    r_index_y = r_index_y + 1
    flags: f_negative, f_zero */
void CPU::INY(){
    r_index_y++;
    set_status_register(f_negative, r_index_y & 0x80);
    set_status_register(f_zero, r_index_y == 0);
    cycle();
}

// Jump to Mem
void CPU::JMP(std::function<uint16_t()> address){
    r_program_counter = address() - 1;
}

/*  Jump to Mem and save return address to SP
    push PC + 2
    PC+1 -> PCL
    PC+2 -> PCH */
void CPU::JSR(std::function<uint16_t()> address){
    uint8_t low_byte = r_program_counter & 0xFF;
    uint8_t high_byte = r_program_counter & 0xFF00;
    push(high_byte);
    push(low_byte);
    r_program_counter = address() - 1;
    cycle();
}

/*  Load r_accumulator with Mem
    r_accumulator = Mem
    flags: f_zero, f_negative*/
void CPU::LDA(std::function<uint16_t()> address){
    r_accumulator = read(address());
    set_status_register(f_zero, r_accumulator == 0);
    set_status_register(f_negative, r_accumulator & 0x80);
}

/*  Load r_index_x with Mem
    r_index_x = Mem
    flags: f_zero, f_negative*/
void CPU::LDX(std::function<uint16_t()> address){
    r_index_x = read(address());
    set_status_register(f_zero, r_index_x == 0);
    set_status_register(f_negative, r_index_x & 0x80);
}

/*  Load r_index_y with Mem
    r_index_y = Mem
    flags: f_zero, f_negative*/
void CPU::LDY(std::function<uint16_t()> address){
    r_index_y = read(address());
    set_status_register(f_zero, r_index_y == 0);
    set_status_register(f_negative, r_index_y & 0x80);
}

/*  Logical Shift Right
    r_accumulator or Mem
    flags: f_negative, f_zero, f_carry*/
void CPU::LSR(std::function<uint16_t()> address){
    // accumulator
    if (address == nullptr){
        set_status_register(f_negative, 0);
        set_status_register(f_carry, (r_accumulator >> 1) & 1);
        r_accumulator >>= 1;
        set_status_register(f_zero, r_accumulator == 0);
    }else{
        uint16_t addr = address();
        uint8_t data = read(addr);
        set_status_register(f_negative, 0);
        set_status_register(f_carry, (data >> 1) & 1);
        data >>=     1;
        set_status_register(f_zero, data == 0);
        write(addr, data);
    }
    cycle();
}

/* cycle or move r_program_counter to next opcode*/
void CPU::NOP(std::function<uint16_t()> address){
    if (address != nullptr){
        address();
    }else{
        cycle();
    }
}

/*  Logical OR with r_accumulator and Mem
    r_accumulator = r_accumulator OR Mem
    flags: f_negative, f_zero */
void CPU::ORA(std::function<uint16_t()> address){
    r_accumulator |= read(address());
    set_status_register(f_negative, r_accumulator & 0x80);
    set_status_register(f_zero, r_accumulator == 0);
}

/*  Push r_accumulator to stack*/
void CPU::PHA(){
    push(r_accumulator);
    cycle();
}

/*  Push r_status_register to stack*/
void CPU::PHP(){
    set_status_register(f_break_4, 1);
    set_status_register(f_break_5, 1);
    push(r_status_register);
    cycle();
}

/*  Pull r_accumulator from stack*/
void CPU::PLA(){
    r_accumulator = pop();
    set_status_register(f_negative, r_accumulator & 0x80);
    set_status_register(f_zero, r_accumulator == 0);
    cycle(2);
}

/*  Pull r_status_register from stack*/
void CPU::PLP(){
    r_status_register = pop();
    set_status_register(f_break_4, 0);
    set_status_register(f_break_5, 1);
    cycle(2);
}

/*  Rotate bits left
    shift bits left
    bit 0 = f_carry
    f_carry = bit 7
    flags: f_negative, f_zero, f_carry*/
void CPU::ROL(std::function<uint16_t()> address){
    // accumulator
    if (address == nullptr){
        uint8_t bit_7 = r_accumulator & 0x80;
        r_accumulator = (r_accumulator << 1) | (r_status_register & f_carry);
        set_status_register(f_carry, bit_7);
        set_status_register(f_zero, r_accumulator == 0);
        set_status_register(f_negative, r_accumulator & 0x80);
    }else{
        uint16_t addr = address();
        uint8_t data = read(addr);
        uint8_t bit_7 = data & 0x80;
        data = (data << 1) | (r_status_register & f_carry);
        set_status_register(f_carry, bit_7);
        set_status_register(f_zero, data == 0);
        set_status_register(f_negative, data & 0x80);
        write(addr, data);
    }
    cycle();
}

/*  Rotate bits right
    shift bits right
    bit 7 = f_carry
    f_carry = bit 0
    flags: f_negative, f_zero, f_carry*/
void CPU::ROR(std::function<uint16_t()> address){
    // accumulator
    if (address == nullptr){
        uint8_t bit_0 = r_accumulator & 1;
        r_accumulator = (r_accumulator >> 1) | ((r_status_register & f_carry) << 7);
        set_status_register(f_carry, bit_0);
        set_status_register(f_zero, r_accumulator == 0);
        set_status_register(f_negative, r_accumulator & 0x80);
    }else{
        uint16_t addr = address();
        uint8_t data = read(addr);
        uint8_t bit_0 = data & 0x80;
        data = (data >> 1) | ((r_status_register & f_carry) << 7);
        set_status_register(f_carry, bit_0);
        set_status_register(f_zero, data == 0);
        set_status_register(f_negative, data & 0x80);
        write(addr, data);
    }
    cycle();
}

/*  Return from interrupt
 *  r_status_register = pop
 *  r_program_counter = pop*/
void CPU::RTI(){
	r_status_register = pop();
	set_status_register(f_break_4, 0);
	set_status_register(f_break_5, 1);
	uint8_t low_byte = pop();
    uint8_t hight_byte = pop();
	r_program_counter = hight_byte * 256 + low_byte;
	cycle(2);
}

/*  Return from Subroutine
 *  r_program_counter = pop*/
void CPU::RTS(){
    uint8_t low_byte = pop();
    uint8_t hight_byte = pop();
	r_program_counter = hight_byte * 256 + low_byte;
	cycle(3);
}

/*  Subtract from r_accumulator with borrow form f_carry
 *  flags: f_carry, f_overflow, f_negative, f_zero*/
void CPU::SBC(std::function<uint16_t()> address){
    uint8_t subtractor = read(address());
    uint16_t sum = r_accumulator + (r_status_register >> f_carry) - subtractor;
    set_status_register(f_carry, sum > 0xFF);
    set_status_register(f_overflow, (r_accumulator ^ sum) & (subtractor ^ sum) & 0x80);
    r_accumulator = sum;
    set_status_register(f_negative, r_accumulator && 0x80);
    set_status_register(f_zero, r_accumulator == 0);

}

// Set f_carry = 1
void CPU::SEC(){
    set_status_register(f_carry, 1);
    cycle();
}

// Set f_decimal_mode = 1
void CPU::SED(){
    set_status_register(f_decimal_mode, 1);
    cycle();
}

// Set f_interrupt_disabled = 1
void CPU::SEI(){
    set_status_register(f_interrupt_disabled, 1);
    cycle();
}

// store r_accumulator to Mem
void CPU::STA(std::function<uint16_t()> address){
    write(address(), r_accumulator);
}

// store r_index_x to Mem
void CPU::STX(std::function<uint16_t()> address){
    write(address(), r_index_x);
}

// store r_index_y to Mem
void CPU::STY(std::function<uint16_t()> address){
    write(address(), r_index_y);
}

// Transfer r_accumulator to r_index_x
void CPU::TAX(){
    r_index_x = r_accumulator;
    set_status_register(f_zero, r_index_x == 0);
    set_status_register(f_negative, r_index_x & 0x80);
    cycle();
}

// Transfer r_accumulator to r_index_y
void CPU::TAY(){
    r_index_y = r_accumulator;
    set_status_register(f_zero, r_index_y == 0);
    set_status_register(f_negative, r_index_y & 0x80);
    cycle();
}

// Transfer r_stack_pointer to r_index_x
void CPU::TSX(){
    r_index_x = r_stack_pointer;
    set_status_register(f_zero, r_index_x == 0);
    set_status_register(f_negative, r_index_x & 0x80);
    cycle();
}

// Transfer r_index_x to r_accumulator
void CPU::TXA(){
    r_accumulator = r_index_x;
    set_status_register(f_zero, r_accumulator == 0);
    set_status_register(f_negative, r_accumulator & 0x80);
    cycle();
}

// Transfer r_index_x to r_stack_pointer
void CPU::TXS(){
    r_stack_pointer = r_index_x;
    cycle();
}

// Transfer r_index_y to r_accumulator
void CPU::TYA(){
    r_accumulator = r_index_y;
    set_status_register(f_zero, r_accumulator == 0);
    set_status_register(f_negative, r_accumulator & 0x80);
    cycle();
}

//==Debug==
std::string CPU::debug(){
    std::string cpu_state = "--CPU Registers--\n";
    char temp_buff[4];
    sprintf(temp_buff, "%X", r_program_counter);
    cpu_state = cpu_state + "Program Counter: " + static_cast<std::string>(temp_buff) + '\n';
    sprintf(temp_buff, "%X", r_accumulator);
    cpu_state = cpu_state + "Accumulator: " + static_cast<std::string>(temp_buff) + '\n';
    sprintf(temp_buff, "%X", r_stack_pointer);
    cpu_state = cpu_state + "Stack Pointer: " + static_cast<std::string>(temp_buff) + '\n';
    sprintf(temp_buff, "%X", r_index_x);
    cpu_state = cpu_state + "X Register: " + static_cast<std::string>(temp_buff) + '\n';
    sprintf(temp_buff, "%X", r_index_y);
    cpu_state = cpu_state + "Y Register: " + static_cast<std::string>(temp_buff) + '\n';
    sprintf(temp_buff, "%X", r_status_register);
    cpu_state = cpu_state + "Status Register: " + static_cast<std::string>(temp_buff) + '\n';
    cpu_state = cpu_state + "Cycles: " + std::to_string(cycles);
    return cpu_state;
}