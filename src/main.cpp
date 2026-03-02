#include "chip8.h"
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <rom_path>\n";
        return 1;
    }

    chip8 chip8vm;
    chip8vm.initialize();
    chip8vm.loadGame(argv[1]);

    return 0;
}