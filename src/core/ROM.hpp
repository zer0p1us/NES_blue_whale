#ifndef ROM_HEADER
#define ROM_HEADER

#include <vector>
#include <stdint.h>

#include "Mapper.hpp"

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
    ROM() { }
    ~ROM() { }
    void read(const char* rom_name);
    std::vector<uint8_t> rom_content;
    // std::vector<uint8_t getPrg(uint8_t addr) { return prg_mem.at(addr); };
    // std::vector<uint8_t> getChr(uint8_t addr) { return chr_mem.at(addr); };
    void print_header();
    Mapper* create_mapper();
private:
    std::vector<uint8_t> prg_mem;
    std::vector<uint8_t> chr_mem;
    Header header;
    int mirror;
    uint8_t mapper_id;
};


#endif