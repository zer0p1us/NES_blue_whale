#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "CPU.hpp"

std::vector<bw::Byte> read_rom(const char* rom_name);

int main(int argc, char const *argv[]) {
    const unsigned int viewport_width = 256;
    const unsigned int viewport_height = 240;

    sf::WindowHandle bw_window_handler;
    sf::RenderWindow bw_window(sf::VideoMode(viewport_width, viewport_height), "Blue Whale");
    bw_window.setSize(sf::Vector2u(viewport_width, viewport_height));

    std::vector<bw::Byte> rom_data;
    if (!argc) {
        std::cout << "no arguments received" << '\n';
        return 0;
    } else {
        rom_data = read_rom(argv[1]);
        }

    if (rom_data[3] == 0x1A){
        std::cout << "iNES format rom detected, loading rom..." << '\n';
        std::cout << "there are " << int(rom_data[4]) << " backs of 16KB of PRG-ROM"  << '\n';
        std::cout << "there are " << int(rom_data[5]) << " backs of 8KB of VROM"  << '\n';

    }else{
        std::cout << "this format is not supported, this emulator only supprot iNES roms" << '\n';
    }

    while (bw_window.isOpen()){
        sf::Event event;
        while (bw_window.pollEvent(event)){
            if (event.type == sf::Event::Closed) bw_window.close();
        }


        bw_window.clear();
        // bw_window.draw();
        bw_window.display();
    }

    return 0;
}

std::vector<bw::Byte> read_rom(const char* rom_name){
    // open file
    std::streampos rom_size;
    std::ifstream rom(rom_name, std::ios::binary);

    // get the size
    rom.seekg(0, std::ios::end);
    rom_size = rom.tellg();
    rom.seekg(0, std::ios::beg);

    // read data
    std::vector<bw::Byte> rom_data(rom_size);
    rom.read((char*) &rom_data[0], rom_size);
    return rom_data;
}