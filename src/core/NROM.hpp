#ifndef NROM_HEADER
#define NROM_HEADER

#include "Mapper.hpp"

class NROM : public Mapper{

    public:
        NROM(std::vector<uint8_t> &prg_rom, std::vector<uint8_t> &chr_rom, int mirroring) : Mapper(prg_rom, chr_rom, mirroring) { }

        uint8_t read(uint16_t address) override;
        void write(uint16_t address, uint8_t data) override { }; // NROM cannot be written to


};

#endif