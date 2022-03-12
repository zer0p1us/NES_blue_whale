// to dereference SDL2 main from Blue Whale main
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include <iostream>
#include "util/typedefs.hpp"

#include "core/CPU.hpp"
#include "core/ROM.hpp"
#include "core/PPU.hpp"

int main(int argc, char const *argv[]) {

    std::string window_title = "NES Blue Whale";
    const unsigned int viewport_width = 256;
    const unsigned int viewport_height = 240;
    bool headless_mode = false;

    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "SDL2 failed to initialise;" << '\n';
    }

    SDL_Window* window = SDL_CreateWindow(
        window_title.c_str(),              // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        512,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_SHOWN                   // flags - see below
    );

    if (window == NULL) {
        std::cout << "Could not create window: " << SDL_GetError() << '\n';
        return 1;
    }

    bool is_running = true;

    SDL_Event event;
    // create a renderer with hardware acceleration
    // also present respect the vertical sync
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | ((headless_mode) ? 0 : SDL_RENDERER_PRESENTVSYNC));


    CPU cpu;
    ROM rom;
    PPU ppu;

    if (argc > 1) {
        rom.read_rom(argv[1]);
    }

    if (rom.rom_content[3] == 0x1A){
        std::cout << "iNES format rom detected, loading rom..." << '\n';
        std::cout << "there are " << int(rom.rom_content[4]) << " backs of 16KB of PRG-ROM"  << '\n';
        std::cout << "there are " << int(rom.rom_content[5]) << " backs of 8KB of VROM"  << '\n';

    }else{
        std::cout << "this format is not supported, this emulator only supprot iNES roms" << '\n';
    }

    SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 256, 240);

    while (is_running){
        cpu.step();

        while (SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;

            default:
                break;
            }
        }


        //Draw frame
        ppu.renderFrame = false;
        SDL_RenderSetScale(renderer, 2, 2);
        SDL_UpdateTexture(texture, NULL, ppu.buffer, 256 * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // SDL_Delay(3000);

    SDL_DestroyWindow(window);


    return 0;
}
