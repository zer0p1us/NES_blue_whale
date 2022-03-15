#include "NROM.hpp"


uint8_t NROM::read(uint16_t addr){
    if (addr < 0x8000) {
        return 0;
    }

    addr = (addr - 0x8000) % prg_rom.size();
    return prg_rom[addr];
}
