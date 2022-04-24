#ifndef PPU_HEADER
#define PPU_HEADER

#include <stdint.h>
#include <vector>

#include "BUS.hpp"
#include "Mapper.hpp"
#include "RAM.hpp"

class PPU : BUS {

private:

    Mapper* mapper;

    // Address: $2000
    union {
        struct{
            unsigned base_nametable_address: 2;
            unsigned vram_increment: 1;
            unsigned sprite_pattern_table_address: 1;
            unsigned background_pattern_table_address: 1;
            unsigned sprite_size: 1;
            unsigned ppu_master_slave_select: 1;
            unsigned generate_NMI: 1;
        }; uint8_t data;
    } r_control;

    // Address: $2001
    union {
        struct{
            unsigned gray_scale: 1; // 0: normal colour; 1: produce greyscale display;
            unsigned render_background_leftmost_8_pixel: 1; // 1: show; 0: hide;
            unsigned render_sprite_leftmost_8_pixel: 1; // 1: show; 0: hide;
            unsigned render_backround: 1; // 1: show; 0: hide;
            unsigned render_sprites: 1; // 1: show; 0: hide;
            unsigned enchance_red: 1;
            unsigned enchance_green: 1;
            unsigned enchance_blue: 1;
        }; uint8_t data;
    } r_mask;

    // Address: $2001
    union {
        struct{
            unsigned unused: 5;
            unsigned sprite_overflow: 1;
            unsigned sprite_0_hit: 1;
            unsigned vertical_black: 1; // 0: not in vblank 1: in vblank
        }; uint8_t data;
    } r_status;

    uint8_t r_status_temp = 0;

    // Address: $2003
    uint8_t OAM_address = 0;
    // Address: $2004
    uint8_t OAM_data = 0;
    // Address: $2005
    uint8_t ppu_scroll = 0;
    uint8_t ppu_read_buffer = 0;
    uint8_t ppu_read_buffer_temp = 0;



    struct sprite {
        uint8_t x;
        uint8_t y;
        uint8_t tile_index;
        uint8_t attribute;
        uint8_t id;
    };

    struct sprite_object_attribute_entity {
        uint8_t low;
        uint8_t high;
        uint8_t attribute;
        uint8_t counter;
        uint8_t id;
        bool horizontally_flip;
        bool vertically_flip;
        int shifted = 0;

        void shift() {
            if (shifted == 8) {
                return;
            }

            if (horizontally_flip) {
                low >>= 1;
                high >>= 1;
            } else {
                low <<= 1;
                high <<= 1;
            }

            shifted++;
        }
    };

    uint8_t write_toggle = 0;
    uint8_t data_buffer = 0;
    uint16_t address_buffer = 0;

    uint32_t palette[64] = {
        4283716692, 4278197876, 4278718608, 4281335944, 4282646628, 4284219440, 4283696128, 4282128384,
        4280297984, 4278729216, 4278206464, 4278205440, 4278202940, 4278190080, 4278190080, 4278190080,
        4288190104, 4278734020, 4281348844, 4284227300, 4287108272, 4288681060, 4288160288, 4286069760,
        4283718144, 4280840704, 4278746112, 4278220328, 4278216312, 4278190080, 4278190080, 4278190080,
        4293717740, 4283210476, 4286086380, 4289749740, 4293154028, 4293679284, 4293683812, 4292118560,
        4288719360, 4285842432, 4283224096, 4281912428, 4281906380, 4282137660, 4278190080, 4278190080,
        4293717740, 4289252588, 4290559212, 4292129516, 4293701356, 4293701332, 4293702832, 4293182608,
        4291613304, 4290043512, 4289258128, 4288209588, 4288730852, 4288717472, 4278190080, 4278190080
    };


    // pixel dot position
    int scanline = 0;
    int dot = 0;
    int pixel_index = 0;
    bool nmi_occurred = false;
    bool odd = false;

    // background
    uint8_t bg_palette[16] = { 0 };
    RAM vram;
    // uint8_t vram[2048] = { 0 };
    uint16_t t = 0;
    uint8_t x = 0;
    int w = 0;
    uint8_t ntbyte, attrbyte, patternlow, patternhigh;
    uint16_t bgShiftRegLo;
    uint16_t bgShiftRegHi;
    uint16_t attrShiftReg1;
    uint16_t attrShiftReg2;
    uint8_t quadrant_num;

    //Sprites
    uint8_t sprite_palette[16] = { 0 };
    uint16_t spritePatternLowAddr, spritePatternHighAddr;
    int primaryOAMCursor = 0;
    int secondaryOAMCursor = 0;
    sprite primaryOAM[64];
    sprite secondaryOAM[8];
    sprite tmpOAM;
    bool inRange = false;
    int inRangeCycles = 8;
    int spriteHeight = 8;
    std::vector<sprite_object_attribute_entity> sprite_OAE;
    sprite_object_attribute_entity out;

    uint8_t ppu_read(uint16_t address);
    void ppu_write(uint16_t address, uint8_t data);

    void increment_x();
    void increment_y();

    uint8_t read_OAM(int address);

    bool render_state() { return !r_mask.render_backround && !r_mask.render_sprites; }

    void set_pixel();

    void fetch_tiles();
    void x_inc();
    void y_inc();
    void horizontal_to_addr_buffer();
    void vertical_to_addr_buffer();
    void reloadShiftersAndShift();
    void dec_sprite_counter();
    void eval_sprite();
    uint16_t getSpritePatternAddress(const sprite&, bool);
    void copyOAM(uint8_t data, uint16_t address);
    bool inYRange(const sprite&);
    bool isUninit(const sprite&);

public:

    PPU(Mapper* p_mapper) : mapper(p_mapper) { }
    uint32_t frame_buffer[256*240] = { 0 };

    void step();

    bool renderFrame;
    bool NMI = false;

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    bool render_frame;
};


#endif