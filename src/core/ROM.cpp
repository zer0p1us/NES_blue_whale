#include "ROM.hpp"

#include <fstream>

ROM::ROM(){

}

void ROM::read_rom(const char* rom_name){
    // open file
    std::streampos rom_size;
    std::ifstream rom(rom_name, std::ios::binary);

    // get the size
    rom.seekg(0, std::ios::end);
    rom_size = rom.tellg();
    rom.seekg(0, std::ios::beg);

    // read data
    std::vector<uint8_t> rom_data(rom_size);
    rom.read((char*) &rom_data[0], rom_size);
    ROM::rom_content = rom_data;
}