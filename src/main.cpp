#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "Cartridge.hpp"

std::vector<bw::Byte> read_rom(const char* rom_name);

int main(int argc, char const *argv[]) {
    const unsigned int viewport_width = 256;
    const unsigned int viewport_height = 240;

    sf::WindowHandle bw_window_handler;
    sf::RenderWindow bw_window(sf::VideoMode(viewport_width, viewport_height), "Blue Whale");
    bw_window.setSize(sf::Vector2u(viewport_width, viewport_height));

    if (!argc) {
         std::cout << "no arguments received" << '\n';
    } else {
        std::vector<bw::Byte> rom_data = read_rom(argv[0]);
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