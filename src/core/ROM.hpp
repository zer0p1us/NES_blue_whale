#ifndef ROM_HEADER
#define ROM_HEADER


#include <vector>
#include <stdint.h>

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