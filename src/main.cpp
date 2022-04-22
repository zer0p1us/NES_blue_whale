// to dereference SDL2 main from Blue Whale main
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "core/CPU.hpp"
#include "core/ROM.hpp"
#include "core/PPU.hpp"
#include "debug.hpp"

// based on 1.79Mhz
// indicates the time in ns to sleep for to keep original speed
const long clock_speed = 10000/179;

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

    if (argc < 2){
        std::cout << "Err: No NES ROM given!" << '\n';
        return 1;
    }

    ROM rom;
    rom.read(argv[1]);
    #ifdef DEBUG
        rom.print_header();
    #endif
    Mapper* mapper = rom.create_mapper();
    PPU ppu(mapper);



    CPU cpu(mapper, &ppu);
    cpu.reset();


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

        #ifdef DEBUG
            if (std::tolower(std::cin.get()) == 'q'){
                // closing program
                SDL_DestroyWindow(window);
                return 0;
            }
        #endif

        // pause to keep speed semilar to nes speeds
        std::this_thread::sleep_for(std::chrono::nanoseconds(clock_speed));
    }

    // SDL_Delay(3000);

    SDL_DestroyWindow(window);


    return 0;
}
