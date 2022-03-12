#ifndef ROM_HEADER
#define ROM_HEADER

#include "../util/typedefs.hpp"
#include <vector>

class ROM {
public:
    ROM();
    ~ROM() { };
    void read_rom(const char* rom_name);
    std::vector<Byte> rom_content;
private:
    // Byte[]

};


#endif