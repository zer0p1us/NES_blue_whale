#ifndef MAPPER_HEADER
#define MAPPER_HEADER

#include <vector>

class Mapper {
    public:
        Mapper(std::vector<uint8_t> &prg_rom, std::vector<uint8_t> &chr_rom, int mirroring): prg_rom(prg_rom), chr_rom(chr_rom), mirroring(mirroring) { }
        virtual uint8_t read(uint16_t addr);
        virtual void write(uint16_t addr, uint8_t data);


    protected:
        int mirroring;
        std::vector<uint8_t> prg_rom;
        std::vector<uint8_t> chr_rom;


};

#endif