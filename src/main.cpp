#include "chip8.h"
#include <SDL2/SDL.h>
#include <iostream>

chip8 chip8vm;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <rom_path>\n";
        return 1;
    }

    chip8vm.initialize();
    chip8vm.loadGame(argv[1]);

    // Print memory starting at 0x200 where ROM data is loaded
    std::cout << "Memory dump from 0x200:\n";
    for (int i = 0; i < 64; ++i) {
        if (i % 16 == 0)
            std::cout << std::hex << "0x" << (0x200 + i) << ": ";
        std::cout << std::hex << std::uppercase
                  << (static_cast<int>(chip8vm.memory[0x200 + i]) < 16 ? "0" : "")
                  << static_cast<int>(chip8vm.memory[0x200 + i]) << " ";
        if (i % 16 == 15)
            std::cout << "\n";
    }
    std::cout << std::dec << std::endl;
}