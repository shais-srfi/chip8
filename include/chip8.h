#pragma once
#include <cstdint>
#include <cstring>
#include <fstream>

using u8 = uint8_t;
using u16 = uint16_t;

class chip8 {
public:
    void initialize();
    void emulateCycle();
    void loadGame(std::string gameName);

    u8 memory[4096];    // chip8 memory (temporarily here)

private:
    u16 opcode;         // operation code
    u8 V[16];           // chip8 cpu registers
    u16 I;              // index register
    u16 pc;             // program counter
    u8 gfx[64 * 32];    // 2048 pixel screen
    u8 delayTimer;
    u8 soundTimer;
    u16 stack[16];
    u16 sp;             // stack pointer
    u8 key[16];         // keypad

    u8 chip8Fontset[80] =
    { 
     0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
     0x20, 0x60, 0x20, 0x20, 0x70, // 1
     0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
     0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
     0x90, 0x90, 0xF0, 0x10, 0x10, // 4
     0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
     0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
     0xF0, 0x10, 0x20, 0x40, 0x40, // 7
     0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
     0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
     0xF0, 0x90, 0xF0, 0x90, 0x90, // A
     0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
     0xF0, 0x80, 0x80, 0x80, 0xF0, // C
     0xE0, 0x90, 0x90, 0x90, 0xE0, // D
     0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
     0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
};

void chip8::initialize() {
    pc = 0x200; // this is where the program counter starts
    opcode = 0;
    I = 0;
    sp = 0;
    memset(gfx, 0, sizeof(gfx));
    memset(stack, 0, sizeof(stack));
    memset(V, 0, sizeof(V));
    memset(memory, 0, sizeof(memory));

    // loading in fontset starting from 0x50 to 0xA0
    for (int i = 0; i < 80; ++i) {
        memory[0x50 + i] = chip8Fontset[i];
    }

    delayTimer = 0;
    soundTimer = 0;
};

void chip8::loadGame(std::string name) {
    std::ifstream romFile(name, std::ios::binary);
    char byte;
    int i = 0;
    while (romFile.get(byte)) {
        memory[0x200 + i] = static_cast<u8>(byte);
        ++i;
    }
};