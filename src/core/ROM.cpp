#include "ROM.hpp"

#include <fstream>
#include <iostream>
#include <stdint.h>

#include "NROM.hpp"
#include "../debug.hpp"


void ROM:: read(const char* rom_name){
    // open file
    std::streampos rom_size;
    std::ifstream rom(rom_name, std::ios::binary);

    rom.read(reinterpret_cast<char*>(&header.nes), sizeof(char[4]));
    rom.read(reinterpret_cast<char*>(&header.prg_in_16kb), sizeof(uint8_t));
    rom.read(reinterpret_cast<char*>(&header.chr_in_8kb), sizeof(uint8_t));
    rom.read(reinterpret_cast<char*>(&header.flag_6), sizeof(uint8_t));
    rom.read(reinterpret_cast<char*>(&header.flag_7), sizeof(uint8_t));
    rom.read(reinterpret_cast<char*>(&header.prg_ram_8kb), sizeof(uint8_t));
    rom.read(reinterpret_cast<char*>(&header.flag_9), sizeof(uint8_t));
    rom.read(reinterpret_cast<char*>(&header.flag_10), sizeof(uint8_t));
    rom.read(reinterpret_cast<char*>(&header.zeros), sizeof(char[5]));

    mirror = header.flag_6 & 1;

    mapper_id = ((header.flag_6 & 0xF0) >> 4) | (header.flag_7 & 0xF0);

    // init vectors
    int prg_size = header.prg_in_16kb * 16384;
    int chr_size = header.chr_in_8kb * 8192;
    prg_mem.resize(prg_size);
    chr_mem.resize(chr_size);

    // load PRG_ROM
    rom.read((char*)prg_mem.data(), prg_size);

    // load CHR_ROM if exists
    if (header.chr_in_8kb > 0){
        rom.read((char*)chr_mem.data(), chr_size);
    } else { // or fill it with zeroes
        chr_mem = std::vector<uint8_t>(8192, 0);
    }
}

void ROM::print_header(){
    std::cout << "--Header--" << '\n';
    std::cout << "Signature: " << header.nes << '\n';
    std::cout << "PRG ROM size: " << (int)header.prg_in_16kb << " of 16kb banks" << '\n';
    std::cout << "CHR ROM size: " << (int)header.chr_in_8kb << " of 8kb banks" << '\n';
}

Mapper* ROM::create_mapper(){
    switch (mapper_id) {
    case 0:
        debug_out("NROM mapper detected");
        return new NROM(prg_mem, chr_mem, mirror);
        break;

    default:
        std::cout << "Mapper: " << mapper_id << " not implemented" << '\n';
        return NULL;
        break;
    }

}