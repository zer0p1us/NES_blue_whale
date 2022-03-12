#ifndef PPU_HEADER
#define PPU_HEADER



class PPU {

public:

    PPU(/* args */) = default;
    ~PPU() { };

    uint32_t buffer[256*240] = { 0 };

    bool renderFrame;


private:
    /* data */

};


#endif