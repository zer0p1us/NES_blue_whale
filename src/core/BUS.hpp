#ifndef BUS_HEADER
#define BUS_HEADER

#include <stdint.h>

class BUS {
    public:
        virtual ~BUS() {}
        virtual uint8_t read(uint16_t addr) = 0;
        virtual void write(uint16_t addr, uint8_t data) = 0;

};


#endif