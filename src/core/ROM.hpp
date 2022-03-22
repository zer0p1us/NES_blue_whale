#ifndef ROM_HEADER
#define ROM_HEADER


#include <vector>
#include <stdint.h>

struct Header {
    // contains 16 bytes of data
    char nes[4]; // NES String followed by MS-DOS end-of-line char $1A
    uint8_t prg_in_16kb;
    uint8_t chr_in_8kb;
    uint8_t flag_6;
    uint8_t flag_7;
    uint8_t prg_ram_8kb;
    uint8_t flag_9;
    uint8_t flag_10;
    char zeros[5]; // padding
};
class ROM {
public:
    ROM();
    ~ROM() { };
    void read_rom(const char* rom_name);
    std::vector<uint8_t> rom_content;
private:
    std::vector<uint8_t> PRG_ROM;
    std::vector<uint8_t> CHR_ROM;

};


#endif