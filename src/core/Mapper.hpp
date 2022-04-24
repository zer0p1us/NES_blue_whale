#ifndef MAPPER_HEADER
#define MAPPER_HEADER

#include <vector>
#include <stdint.h>

class Mapper {
    public:
        Mapper(std::vector<uint8_t> &prg_rom, std::vector<uint8_t> &chr_rom, int mirroring): prg_rom(prg_rom), chr_rom(chr_rom), mirroring(mirroring) { }
        virtual uint8_t prg_read(uint16_t addr) = 0; // zero is required for purely virtual functions
        virtual void prg_write(uint16_t addr, uint8_t data) = 0; // zero is required for purely virtual functions

        virtual uint8_t chr_read(uint16_t addr) { return chr_rom[addr]; }
        virtual void chr_write(uint16_t addr, uint8_t data) { chr_rom[addr] = data; }
        int get_mirroring() { return mirroring; }



    protected:
        int mirroring;
        std::vector<uint8_t> prg_rom;
        std::vector<uint8_t> chr_rom;


};

#endif