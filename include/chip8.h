#pragma once
#include <cstdint>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <ctime>

using u8 = uint8_t;
using u16 = uint16_t;

class chip8 {
public:
    void initialize();
    void emulateCycle();
    void loadGame(std::string gameName);
    void setKey(u8 k, u8 state);

private:
    // mutable data
    u16 opcode;         // operation code
    u8 memory[4096];    // chip8 memory
    u8 V[16];           // chip8 cpu registers
    bool drawFlag;
    u16 I;              // index register
    u16 pc;             // program counter
    u8 gfx[64 * 32];    // 2048 pixel screen
    u8 delayTimer;
    u8 soundTimer;
    u16 stack[16];
    u16 sp;             // stack pointer
    u8 key[16];         // keypad

    // chip 8 font set data
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

    // dispatch tables
    using opcodeHandler = void(chip8::*)(u16);
    static opcodeHandler table[16];
    static opcodeHandler table8[15];

    void opNull(u16 opcode) { return; };
    void op0(u16 opcode);
    void op1(u16 opcode);
    void op2(u16 opcode);
    void op3(u16 opcode);
    void op4(u16 opcode);
    void op5(u16 opcode);
    void op6(u16 opcode);
    void op7(u16 opcode);
    void op8(u16 opcode);
    void op80(u16 opcode);
    void op81(u16 opcode);
    void op82(u16 opcode);
    void op83(u16 opcode);
    void op84(u16 opcode);
    void op85(u16 opcode);
    void op86(u16 opcode);
    void op87(u16 opcode);
    void op8E(u16 opcode);
    void op9(u16 opcode);
    void opA(u16 opcode);
    void opB(u16 opcode);
    void opC(u16 opcode);
    void opD(u16 opcode);
    void opE(u16 opcode);
    void opF(u16 opcode);

    // helper functions
    void fetchOpcode();
    void executeOperation();
    void getKeyPress();
};