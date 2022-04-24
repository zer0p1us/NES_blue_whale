#include "PPU.hpp"
#include "Mapper.hpp"

void PPU::step(){
    if ((scanline >= 0 && scanline <= 239) || scanline == 261) { //visible scanline, pre-render scanline
        if (scanline == 261) {
            //clear vbl flag and sprite overflow
            if (dot == 2) {
                pixel_index = 0;
                r_status.data &= ~0x80;
                r_status.data &= ~0x20;
                r_status.data &= ~64;
            }

            //copy vertical bits
            if (dot >= 280 && dot <= 304) {
                vertical_to_addr_buffer();
            }
        }

         //skip dot on odd frame
         /*if (odd && !isRenderingDisabled() && dot == 339 && scanline == 261) {
             scanline = 0;
             dot = 0;
             tick();
         }*/

        if (scanline >= 0 && scanline <= 239) {
            eval_sprite();
        }

        if (dot == 257) {
            horizontal_to_addr_buffer();
        }

        //main hook: fetch tiles, emit pixel, shift
        if ((dot >= 1 && dot <= 257) || (dot >= 321 && dot <= 337)) {
            //reload shift registers and shift
            if ((dot >= 2 && dot <= 257) || (dot >= 322 && dot <= 337)) {
                reloadShiftersAndShift();
            }

            //if on visible scanlines and dots
            //eval sprites
            //emit pixels
            if (scanline >= 0 && scanline <= 239) {
                if (dot >= 2 && dot <= 257) {
                    if (scanline > 0) {
                        dec_sprite_counter();
                    }

                    set_pixel();
                }
            }

            //fetch nt, at, pattern low - high
            fetch_tiles();
        }
    } else if (scanline >= 240 && scanline <= 260) { //post-render, vblank
        if (scanline == 240 && dot == 0) {
            render_frame = true;
        }

        if (scanline == 241 && dot == 1) {
            //set vbl flag
            r_status.data |= 0x80;

            //flag for nmi
            if (r_control.data & 0x80) {
                nmi_occurred = true;
            }
        }
    }

    if (dot == 340) {
        scanline = (scanline+1) % 262;
        if (scanline == 0) {
            odd = !odd;
        }
        dot = 0;
    } else {
        dot++;
    }
}

void PPU::set_pixel(){
    if (render_state()) {
        pixel_index++;
        return;
    }

    // background
    uint16_t fine_select = 0x8000 >> x;
    uint16_t pixel_1 = (bgShiftRegLo & fine_select) << x;
    uint16_t pixel_2 = (bgShiftRegHi & fine_select) << x;
    uint16_t pixel_3 = (attrShiftReg1 & fine_select) << x;
    uint16_t pixel_4 = (attrShiftReg2 & fine_select) << x;
    uint8_t background_bit_12 = (pixel_2 >> 14) | (pixel_1 >> 15);

    //Sprites
    uint8_t sprite_pixel_1 = 0;
    uint8_t sprite_pixel_2 = 0;
    uint8_t sprite_pixel_3 = 0;
    uint8_t sprite_pixel_4 = 0;
    uint8_t sprite_bit_12 = 0;
    uint8_t palette_index = 0 | (pixel_4 >> 12) | (pixel_3 >> 13) | (pixel_2 >> 14) | (pixel_1 >> 15);
    uint8_t spritepalette_index = 0;
    bool showSprite = false;
    bool spriteFound = false;

    for (auto& sprite : sprite_OAE) {
        if (sprite.counter == 0 && sprite.shifted != 8) {
            if (spriteFound) {
                sprite.shift();
                continue;
            }

            sprite_pixel_1 = sprite.horizontally_flip ? ((sprite.low & 1) << 7) : sprite.low & 128;
            sprite_pixel_2 = sprite.horizontally_flip ? ((sprite.high & 1) << 7) : sprite.high & 128;
            sprite_pixel_3 = sprite.attribute & 1;
            sprite_pixel_4 = sprite.attribute & 2;
            sprite_bit_12 = (sprite_pixel_2 >> 6) | (sprite_pixel_1 >> 7);

            //sprite zero hit
            if (!r_status.sprite_0_hit && sprite_bit_12 && background_bit_12 && sprite.id == 0 && r_mask.render_sprites && r_mask.render_backround && dot < 256) {
                r_status.data |= 64;
            }

            if (sprite_bit_12) {
                showSprite = ((background_bit_12 && !(sprite.attribute & 32)) || !background_bit_12) && r_mask.render_sprites;
                spritepalette_index = 0x10 | (sprite_pixel_4 << 2) | (sprite_pixel_3 << 2) | sprite_bit_12;
                spriteFound = true;
            }

            sprite.shift();
        }
    }

    //When bg rendering is off
    if (!r_mask.render_backround) {
        palette_index = 0;
    }

    uint8_t pindex = ppu_read(0x3F00 | (showSprite ? spritepalette_index : palette_index)) % 64;
    //Handling grayscale mode
    uint8_t p = r_mask.gray_scale ? (pindex & 0x30) : pindex;

    //Dark border rect to hide seam of scroll, and other glitches that may occur
    if (dot <= 9 || dot >= 249 || scanline <= 7 || scanline >= 232) {
        showSprite = false;
        p = 13;
    }

    frame_buffer[pixel_index++] = palette[p];
}


uint8_t PPU::read(uint16_t address){
    address %= 8;

    if (address == 0) {
        return r_control.data;
    } else if (address == 1) {
        return r_mask.data;
    } else if (address == 2) {
        r_status_temp = r_status.data;
        r_status.data &= ~0x80;
        w = 0;
        return r_status_temp;
    } else if (address == 3) {
        return OAM_address;
    } else if (address == 4) {
        return read_OAM(OAM_address);
    } else if (address == 7) {
        ppu_read_buffer = ppu_read_buffer_temp;

        if (address_buffer >= 0x3F00 && address_buffer <= 0x3FFF) {
            ppu_read_buffer_temp = ppu_read(address_buffer - 0x1000);
            ppu_read_buffer = r_mask.gray_scale ? (ppu_read(address_buffer) & 0x30) : ppu_read(address_buffer);
        } else {
            ppu_read_buffer_temp = ppu_read(address_buffer);
        }

        address_buffer += r_control.vram_increment ? 32 : 1;
        address_buffer%=16384;
        return ppu_read_buffer;
    }

    return 0;
}


void PPU::write(uint16_t address, uint8_t data){
    address %= 8;

    if (address == 0) {
        t = (t & 0xF3FF) | (((uint16_t) data & 0x03) << 10);
        spriteHeight = (data & 0x20) ? 16 : 8;
        r_control.data = data;
    } else if (address == 1) {
        r_mask.data = data;
    } else if (address == 2) {
        data &= ~128;
        r_status.data &= 128;
        r_status.data |= data;
    } else if (address == 3) {
        OAM_address = data;
    } else if (address == 4 && (scanline > 239 && scanline != 241)) {
        copyOAM(data, OAM_address++);
    } else if (address == 5) {
        if (w == 0) {
            t &= 0x7FE0;
            t |= ((uint16_t) data) >> 3;
            x = data & 7;
            w = 1;
        } else {
            t &= ~0x73E0;
            t |= ((uint16_t) data & 0x07) << 12;
            t |= ((uint16_t) data & 0xF8) << 2;
            w = 0;
        }

        ppu_scroll = data;
    } else if (address == 6) {
        if (w == 0) {
            t &= 255;
            t |= ((uint16_t)data & 0x3F) << 8;
            w = 1;
        } else {
            t &= 0xFF00;
            t |= data;
            address_buffer = t;

            w = 0;
        }

    } else if (address == 7) {
        ppu_write(address_buffer, data);
        address_buffer += r_control.vram_increment ? 32 : 1;
        address_buffer %= 16384;
    }
}



uint8_t PPU::ppu_read(uint16_t address){
    switch (address) {
        case 0x0000 ... 0x1FFF:
            return mapper->chr_read(address);
            break;
        case 0x2000 ... 0x2FFF:
            //Horizontal
            if (mapper->get_mirroring() == 0) {
                if (address >= 0x2400 && address < 0x2800) {
                    address -= 0x400;
                }

                if (address >= 0x2800 && address < 0x2c00) {
                    address -= 0x400;
                }

                if (address >= 0x2c00 && address < 0x3000) {
                    address -= 0x800;
                }
            //Vertical
            } else if (mapper->get_mirroring() == 1) {
                if (address >= 0x2800 && address < 0x3000) {
                    address -= 0x800;
                }
            //One-screen mirroring, lower-bank (MMC1)
            } else if (mapper->get_mirroring() == 2) {
                address &= ~0xC00;
            //One-screen mirroring, upper-bank (MMC1)
            } else {
                address = (address & ~0xC00) + 0x400;
            }

            return vram.read(address - 0x2000);
            break;
        case 0x3F00 ... 0x3F0F:
            if (address == 0x3F04 || address == 0x3F08 || address == 0x3F0C) {
               address = 0x3F00;
            }

            return bg_palette[address - 0x3F00];
            break;
        case 0x3F10 ... 0x3F1F:
            if (address == 0x3F10 || address == 0x3F14 || address == 0x3F18 || address == 0x3F1C) {
                return ppu_read(address & 0x3F0F);
            } else {
                return sprite_palette[address - 0x3F10];
            }

            return 1;
            break;
        case 0x3000 ... 0x3EFF:
            return ppu_read(address - 0x1000);
            break;
        default:
            return 1;
            break;
    }
}


void PPU::ppu_write(uint16_t address, uint8_t data){

    switch (address) {
        case 0x0000 ... 0x1FFF:
            mapper->chr_write(address, data);
            break;
        case 0x2000 ... 0x2FFF:
            //Horizontal
            if (mapper->get_mirroring() == 0) {
                if (address >= 0x2400 && address < 0x2800) {
                    address -= 0x400;
                }

                if (address >= 0x2800 && address < 0x2c00) {
                    address -= 0x400;
                }

                if (address >= 0x2c00 && address < 0x3000) {
                    address -= 0x800;
                }
            //Vertical
            } else if (mapper->get_mirroring() == 1) {
                if (address >= 0x2800 && address < 0x3000) {
                    address -= 0x800;
                }
            //One-screen mirroring, lower-bank (MMC1)
            } else if (mapper->get_mirroring() == 2) {
                address &= ~0xC00;
            //One-screen mirroring, upper-bank (MMC1)
            } else {
                address = (address & ~0xC00) + 0x400;
            }

            vram.write(address - 0x2000, data);
            break;
        case 0x3F00 ... 0x3F0F:
            bg_palette[address - 0x3F00] = data;
            break;
        case 0x3F10 ... 0x3F1F:
            if (address == 0x3F10 || address == 0x3F14 || address == 0x3F18 || address == 0x3F1C) {
                bg_palette[(address & 0x3F0F) - 0x3F00] = data;
            } else if (address >= 0x3F11 && address <= 0x3F1F) {
                sprite_palette[address - 0x3F10] = data;
            }

            break;
        case 0x3000 ... 0x3EFF:
            ppu_write(address - 0x1000, data);
            break;
        default:
            break;
    }
}



void PPU::fetch_tiles() {
    if (render_state()) {
        return;
    }

    int cycle = dot % 8 ;

    //Fetch nametable byte
    if (cycle == 1) {
        ntbyte = ppu_read(0x2000 | (address_buffer & 0x0FFF));
    //Fetch attribute byte, also calculate which quadrant of the attribute byte is active
    } else if (cycle == 3) {
        attrbyte = ppu_read(0x23C0 | (address_buffer & 0x0C00) | ((address_buffer >> 4) & 0x38) | ((address_buffer >> 2) & 0x07));
        quadrant_num = (((address_buffer & 2) >> 1) | ((address_buffer & 64) >> 5)) * 2;
    //Get low order bits of background tile
    } else if (cycle == 5) {
        uint16_t patterAddr =
        ((uint16_t) r_control.background_pattern_table_address << 12) +
        ((uint16_t) ntbyte << 4) +
        ((address_buffer & 0x7000) >> 12);
        patternlow = ppu_read(patterAddr);
    //Get high order bits of background tile
    } else if (cycle == 7) {
        uint16_t patterAddr =
        ((uint16_t) r_control.background_pattern_table_address << 12) +
         ((uint16_t) ntbyte << 4) +
         ((address_buffer & 0x7000) >> 12) + 8;
        patternhigh = ppu_read(patterAddr);
    //Change columns, change rows
    } else if (cycle == 0) {
        if (dot == 256) {
            y_inc();
        }

        x_inc();
    }
}

void PPU::horizontal_to_addr_buffer(){
    if (render_state()) return;

    address_buffer = (address_buffer & ~0x41F) | (t & 0x41F);
}

void PPU::vertical_to_addr_buffer(){
    if (render_state()) return;

    address_buffer = (address_buffer & ~0x41F) | (t & 0x41F);
}

void PPU::dec_sprite_counter(){
    if (render_state()) return;


    for (auto& sprite : sprite_OAE) {
        if (sprite.counter > 0) {
            sprite.counter--;
        }
    }
}

void PPU::x_inc(){
    if ((address_buffer & 0x001F) == 31) {
        address_buffer &= ~0x001F;
        address_buffer ^= 0x0400;
    } else {
        address_buffer += 1;
    }
}

void PPU::y_inc(){
    if ((address_buffer & 0x7000) != 0x7000) {
        address_buffer += 0x1000;
    } else {
        address_buffer &= ~0x7000;
        int y = (address_buffer & 0x03E0) >> 5;

        if (y == 29) {
            y = 0;
            address_buffer ^= 0x0800;
        } else if (y == 31) {
            y = 0;
        } else {
            y += 1;
        }

        address_buffer = (address_buffer & ~0x03E0) | (y << 5);
    }
}

void PPU::reloadShiftersAndShift(){
    if (render_state()) {
        return;
    }

    bgShiftRegLo <<= 1;
    bgShiftRegHi <<= 1;
    attrShiftReg1 <<= 1;
    attrShiftReg2 <<= 1;

    if (dot % 8 == 1) {
        uint8_t attr_bits1 = (attrbyte >> quadrant_num) & 1;
        uint8_t attr_bits2 = (attrbyte >> quadrant_num) & 2;
        attrShiftReg1 |= attr_bits1 ? 255 : 0;
        attrShiftReg2 |= attr_bits2 ? 255 : 0;
        bgShiftRegLo |= patternlow;
        bgShiftRegHi |= patternhigh;
    }
}

void PPU::copyOAM(uint8_t data, uint16_t address){
    int oamSelect = address / 4;
    int property = address % 4;

    if (property == 0) {
        primaryOAM[oamSelect].y = data;
    } else if (property == 1) {
        primaryOAM[oamSelect].tile_index = data;
    } else if (property == 2) {
        primaryOAM[oamSelect].attribute = data;
    } else {
        primaryOAM[oamSelect].x = data;
    }
}

void PPU::eval_sprite(){
    //clear secondary OAM
    if (dot >= 1 && dot <= 64) {
        if (dot == 1) {
            secondaryOAMCursor = 0;
        }

        secondaryOAM[secondaryOAMCursor].attribute = 0xFF;
        secondaryOAM[secondaryOAMCursor].tile_index = 0xFF;
        secondaryOAM[secondaryOAMCursor].x = 0xFF;
        secondaryOAM[secondaryOAMCursor].y = 0xFF;

        if (dot % 8 == 0) {
            secondaryOAMCursor++;
        }
    }

    //sprite eval
    if (dot >= 65 && dot <= 256) {
        //Init
        if (dot == 65) {
            secondaryOAMCursor = 0;
            primaryOAMCursor = 0;
        }

        if (secondaryOAMCursor == 8) {
            //r_status |= 0x20;
            return;
        }

        if (primaryOAMCursor == 64) {
            return;
        }

        //odd cycle read
        if ((dot % 2) == 1) {
            tmpOAM = primaryOAM[primaryOAMCursor];

            if (inYRange(tmpOAM)) {
                inRangeCycles--;
                inRange = true;
            }
        //even cycle write
        } else {
            //tmpOAM is in range, write it to secondaryOAM
            if (inRange) {
                inRangeCycles--;

                //copying tmpOAM in range is 8 cycles, 2 cycles otherwise
                if (inRangeCycles == 0) {
                    primaryOAMCursor++;
                    secondaryOAMCursor++;
                    inRangeCycles = 8;
                    inRange = false;
                } else {
                    tmpOAM.id = primaryOAMCursor;
                    secondaryOAM[secondaryOAMCursor] = tmpOAM;
                }
            } else {
                primaryOAMCursor++;
            }
        }
    }

    //sprite fetches
    if (dot >= 257 && dot <= 320) {
        if (dot == 257) {
            secondaryOAMCursor = 0;
            sprite_OAE.clear();
        }

        sprite sprite = secondaryOAM[secondaryOAMCursor];

        int cycle = (dot-1) % 8;

        switch (cycle) {
            case 0 ... 1:
                if (!isUninit(sprite)) {
                    out = sprite_object_attribute_entity();
                }

                break;

            case 2:
                if (!isUninit(sprite)) {
                    out.attribute = sprite.attribute;
                    out.horizontally_flip = sprite.attribute & 64;
                    out.vertically_flip = sprite.attribute & 128;
                    out.id = sprite.id;
                }
                break;

            case 3:
                if (!isUninit(sprite)) {
                    out.counter = sprite.x;
                }
                break;

            case 4:
                if (!isUninit(sprite)) {
                    spritePatternLowAddr = getSpritePatternAddress(sprite, out.vertically_flip);
                    out.low = ppu_read(spritePatternLowAddr);
                }
                break;

            case 5:
                break;

            case 6:
                if (!isUninit(sprite)) {
                    spritePatternHighAddr = spritePatternLowAddr + 8;
                    out.high = ppu_read(spritePatternHighAddr);
                }
                break;

            case 7:
                if (!isUninit(sprite)) {
                    sprite_OAE.push_back(out);
                }

                secondaryOAMCursor++;
                break;

            default:
                break;
        }
    }
}

bool PPU::isUninit(const sprite& sprite) {
    return ((sprite.attribute == 0xFF) && (sprite.tile_index == 0xFF) && (sprite.x == 0xFF) && (sprite.y == 0xFF)) || ((sprite.x == 0) && (sprite.y == 0) && (sprite.attribute == 0) && (sprite.tile_index == 0));
}

bool PPU::inYRange(const sprite& oam) {
    return !isUninit(oam) && ((scanline >= oam.y) && (scanline < (oam.y + spriteHeight)));
}


uint16_t PPU::getSpritePatternAddress(const sprite &sprite, bool flipVertically) {
    uint16_t address = 0;

    int fineOffset = scanline - sprite.y;

    if (flipVertically) {
        fineOffset = spriteHeight - 1 - fineOffset;
    }

    //By adding 8 to fineOffset we skip the high order bits
    if (spriteHeight == 16 && fineOffset >= 8) {
        fineOffset += 8;
    }

    if (spriteHeight == 8) {
        address = ((uint16_t) r_control.sprite_pattern_table_address << 12) |
        ((uint16_t) sprite.tile_index << 4) |
        fineOffset;
    } else {
        address = (((uint16_t) sprite.tile_index & 1) << 12) |
        ((uint16_t) ((sprite.tile_index & ~1) << 4)) |
        fineOffset;
    }

    return address;
}


uint8_t PPU::read_OAM(int index) {
    int oamSelect = index / 4;
    int property = index % 4;

    if (property == 0) {
        return primaryOAM[oamSelect].y;
    } else if (property == 1) {
        return primaryOAM[oamSelect].tile_index;
    } else if (property == 2) {
        return primaryOAM[oamSelect].attribute;
    } else {
        return primaryOAM[oamSelect].x;
    }
}