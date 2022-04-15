---
title: "NES (Nintendo Entertainment System) emulator or custom programming language"
author: Ayon
---

<head>
<style>
body {
    height: 100%;
    background-image: url(blob-scene-haikei.svg);
    background-size:100% 100%;
    -o-background-size: 100% 100%;
    -webkit-background-size: 100% 100%;
    background-size:cover;
}
</style>
</head>

The current plan is to build a fully functional NES emulator, however due to the complexity and my lack of knowledge in emulation of hardware there will be a fallback option of functional programming language, as I have previous experiences in building custom languages I will reimplement the specification of an old language of my creation meant for beginner programmers.

The final decision will likely be made within a week of this proposal once I've had enough time to get a proper grip of the scope of the NES project.

if a fallback plan is not acceptable then I will simply take the custom programming language. **do let me know if that's the case, a comment or feedback on the dropbox will be enough**


```classDiagram
class CPU{
    uint16_t r_program_counter
    uint8_t r_stack_pointer
    uint8_t r_accumulator
    uint8_t r_index_x
    uint8_t r_index_y
    uint8_t r_status_register

    void step()
    void reset()
    void cycle(uint8_t)

    uint8_t fetch()
    void execute(uint8_t)

    void debug()
}
```