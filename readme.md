# NES Blue Whale

Blue Whale is an NES emulator, written in C++ using

## Features


**Components**

- [x] CPU
- [ ] PPU
- [ ] pAPU
- [x] I/O


**Supported ROM type**

- [x] NROM (No MMCs)
- [ ] MMC1
- [ ] MMC2

### Building from source

#### Windows

In order to build the project you will need cmake and a C++ compiler, the project has been developed in windows with MinGW(GCC) so that's the only setup I can be sure of.

create a 'build' folder and navigate to it and run the following command.

```cmake
cmake .. -G "MinGW Makefiles" -D "CMAKE_BUILD_TYPE=Release"
```

To build run the following command inthe `build` folder.

```bash
cmake --build .
```
