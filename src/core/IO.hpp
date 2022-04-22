#ifndef IO_HEADER

#define IO_HEADER

#include "SDL2/SDL.h"

#include "BUS.hpp"
#include "../debug.hpp"

#include <stdint.h>

class IO : BUS {

    public:
        IO() = default;
        ~IO() { };
        void set_button(SDL_Keycode key, bool state);

        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);

    private:
        uint8_t joypad_0 = 0;
        uint8_t joypad_0_locked = 0;
        uint8_t joypad_1 = 0; // not implemented
        uint8_t joypad_1_locked = 0; // not implemented
        bool strobe; // if true will prevent
        bool single_player = true; // if true will keep second joypad desabled

        // button mapping for joypad 0
        int b_mapping_a_0 = SDLK_a;
        int b_mapping_b_0 = SDLK_b;
        int b_mapping_select_0 = SDLK_SPACE;
        int b_mapping_start_0 = SDLK_RETURN;
        int b_mapping_up_0 = SDLK_UP;
        int b_mapping_down_0 = SDLK_DOWN;
        int b_mapping_left_0 = SDLK_LEFT;
        int b_mapping_right_0 = SDLK_RIGHT;

        // button mapping for joypad 1
        // will be desabled as mapping another pair of keys on the same keyboard becomes combersome
        int b_mapping_a_1 = SDLK_j;
        int b_mapping_b_1 = SDLK_l;
        int b_mapping_select_1 = SDLK_SPACE;
        int b_mapping_start_1 = SDLK_RETURN;
        int b_mapping_up_1 = SDLK_UP;
        int b_mapping_down_1 = SDLK_DOWN;
        int b_mapping_left_1 = SDLK_LEFT;
        int b_mapping_right_1 = SDLK_RIGHT;

        enum keys{
            b_index_a = 0,
            b_index_b = 1,
            b_index_select = 2,
            b_index_start = 3,
            b_index_up = 4,
            b_index_down = 5,
            b_index_left = 6,
            b_index_right = 7,
        };




};

#endif