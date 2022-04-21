#include "IO.hpp"

uint8_t IO::read(uint16_t address){
    if (address == 0x4016){
        if (strobe) {
            return 0x40 | (joypad_0 & 1);
        }
        uint8_t temp_joypad = 0x80 | (joypad_0_locked & 1);
        joypad_0_locked >>= 1;
        return temp_joypad;

    }else if (address == 0x4017){
        if (strobe) {
            return 0x40 | (joypad_1 & 1);
        }
        uint8_t temp_joypad = 0x80 | (joypad_1_locked & 1);
        joypad_1_locked >>= 1;
        return temp_joypad;
    }
    return 0;
}


void IO::write(uint16_t address, uint8_t data){
    if (address == 0x4016){
        if (strobe && !(data & 0x01)) {
            joypad_0_locked = joypad_0;
        }
        strobe = data & 0x01;
    }else if (address == 0x4017){
        if (strobe && !(data & 0x1)) {
            joypad_1_locked = joypad_1;
        }
    }
}


void IO::set_button(SDL_Keycode key, bool state){

    // joy pad 0
    if (key == b_mapping_a_0){
        joypad_0 = (state) ? (joypad_0 | (1 << b_index_a)) : (joypad_0 & ~(1 << b_index_a));
    }else if (key == b_mapping_b_0){
        joypad_0 = (state) ? (joypad_0 | (1 << b_index_b)) : (joypad_0 & ~(1 << b_index_b));
    }else if (key == b_mapping_up_0){
        joypad_0 = (state) ? (joypad_0 | (1 << b_index_up)) : (joypad_0 & ~(1 << b_index_up));
    }else if (key == b_mapping_down_0){
        joypad_0 = (state) ? (joypad_0 | (1 << b_index_down)) : (joypad_0 & ~(1 << b_index_down));
    }else if (key == b_mapping_left_0){
        joypad_0 = (state) ? (joypad_0 | (1 << b_index_left)) : (joypad_0 & ~(1 << b_index_left));
    }else if (key == b_mapping_right_0){
        joypad_0 = (state) ? (joypad_0 | (1 << b_index_right)) : (joypad_0 & ~(1 << b_index_right));
    }

    // debug info on joypad 0
    char temp_hex[4];
    sprintf(temp_hex, "%X", joypad_0);
    debug_out("joypad 0: " + static_cast<std::string>(temp_hex) + '\n');

    // desables joypad_1
    if (single_player){
        return;
    }

    // joy pad 1
    if (key == b_mapping_a_1){
        joypad_0 = (state) ? (joypad_1 | (1 << b_index_a)) : (joypad_1 & ~(1 << b_index_a));
    }else if (key == b_mapping_b_1){
        joypad_0 = (state) ? (joypad_1 | (1 << b_index_b)) : (joypad_1 & ~(1 << b_index_b));
    }else if (key == b_mapping_up_1){
        joypad_0 = (state) ? (joypad_1 | (1 << b_index_up)) : (joypad_1 & ~(1 << b_index_up));
    }else if (key == b_mapping_down_1){
        joypad_0 = (state) ? (joypad_1 | (1 << b_index_down)) : (joypad_1 & ~(1 << b_index_down));
    }else if (key == b_mapping_left_1){
        joypad_0 = (state) ? (joypad_1 | (1 << b_index_left)) : (joypad_1 & ~(1 << b_index_left));
    }else if (key == b_mapping_right_1){
        joypad_0 = (state) ? (joypad_1 | (1 << b_index_right)) : (joypad_1 & ~(1 << b_index_right));
    }

    // debug info on joypad 1
    sprintf(temp_hex, "%X", joypad_1);
    debug_out("joypad 1: " + static_cast<std::string>(temp_hex) + '\n');
}