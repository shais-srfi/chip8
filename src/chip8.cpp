#include "chip8.h"

void chip8::initialize() {
    pc = 0x200; // this is where the program counter starts
    opcode = 0;
    I = 0;
    sp = 0;
    memset(gfx, 0, sizeof(gfx));
    memset(stack, 0, sizeof(stack));
    memset(V, 0, sizeof(V));
    memset(memory, 0, sizeof(memory));
    memset(key, 0, sizeof(key));
    drawFlag = false;


    // loading in fontset starting from 0x50 to 0xA0
    for (int i = 0; i < 80; ++i) {
        memory[0x50 + i] = chip8Fontset[i];
    }

    delayTimer = 0;
    soundTimer = 0;
    srand(time(0));
};

void chip8::loadGame(std::string name) {
    pc = 0x200;
    std::ifstream romFile(name, std::ios::binary);
    char byte;
    int i = 0;
    while (romFile.get(byte) && i < (4096 - 0x200)) {
        memory[0x200 + i] = static_cast<u8>(byte);
        ++i;
    }
};

void chip8::emulateCycle() {
    executeOperation();
}

void chip8::tickTimers() {
    if(delayTimer > 0) --delayTimer;
    if(soundTimer > 0) --soundTimer;
}

void chip8::setKey(u8 k, u8 state) {
    key[k] = state;
}

void chip8::getKeyPress() {
    u8 x = (opcode & 0x0F00) >> 8;
    for (u8 i = 0; i < 16; ++i) {
        if (key[i] != 0) {
            V[x] = i;
            return;
        }
    }
    pc -= 2;
}

chip8::opcodeHandler chip8::table8[15] = {
    &chip8::op80, &chip8::op81, &chip8::op82, &chip8::op83,
    &chip8::op84, &chip8::op85, &chip8::op86, &chip8::op87,
    &chip8::opNull, &chip8::opNull, &chip8::opNull,
    &chip8::opNull, &chip8::opNull, &chip8::opNull,
    &chip8::op8E
};

chip8::opcodeHandler chip8::table[16] = {
    &chip8::op0, &chip8::op1, &chip8::op2, &chip8::op3,
    &chip8::op4, &chip8::op5, &chip8::op6, &chip8::op7,
    &chip8::op8, &chip8::op9, &chip8::opA, &chip8::opB,
    &chip8::opC, &chip8::opD, &chip8::opE, &chip8::opF
};

void chip8::fetchOpcode() {
    opcode = memory[pc] << 8 | memory[pc + 1];
    pc += 2;
};

void chip8::executeOperation() {
    fetchOpcode();
    (this->*table[(opcode & 0xF000) >> 12])(opcode);
}

///  opcode implementations ///

void chip8::op0(u16 oc) {
    switch (oc & 0x00FF) {
    case 0xE0:
        memset(gfx, 0, sizeof(gfx));
        drawFlag = true;
        break;
    case 0xEE:
        pc = stack[--sp];
        break;
    }
};

void chip8::op1(u16 oc) {
    pc = oc & 0x0FFF;
};

void chip8::op2(u16 oc) {
    stack[sp++] = pc;
    pc = oc & 0x0FFF;
};

void chip8::op3(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 NN = oc & 0x00FF;
    if (V[x] == NN) pc += 2;
}

void chip8::op4(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 NN = oc & 0x00FF;
    if (V[x] != NN) pc += 2;
}

void chip8::op5(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    if (V[x] == V[y]) pc += 2;
}

void chip8::op6(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 NN = oc & 0x00FF;
    V[x] = NN;
}

void chip8::op7(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 NN = oc & 0x00FF;
    V[x] += NN;
}

void chip8::op8(u16 oc) {
    (this->*table8[oc & 0x000F])(oc);
}

void chip8::op80(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    V[x] = V[y];
}

void chip8::op81(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    V[x] |= V[y];
}

void chip8::op82(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    V[x] &= V[y];
}

void chip8::op83(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    V[x] ^= V[y];
}

void chip8::op84(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    V[0xF] = (V[x] + V[y] > 255) ? 1 : 0;
    V[x] += V[y];
}

void chip8::op85(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    V[0xF] = (V[x] > V[y]) ? 1 : 0;
    V[x] -= V[y];
}

void chip8::op86(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    V[0xF] = V[x] & 0x1;
    V[x] >>= 1;
}

void chip8::op87(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    V[0xF] = (V[y] > V[x]) ? 1 : 0;
    V[x] = V[y] - V[x];
}

void chip8::op8E(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    V[0xF] = (V[x] >> 7) & 0x1;
    V[x] <<= 1;
}

void chip8::op9(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    if (V[x] != V[y]) pc+=2;
}

void chip8::opA(u16 oc) {
    u16 NNN = (oc & 0x0FFF);
    I = NNN;
}

void chip8::opB(u16 oc) {
    u16 NNN = (oc & 0x0FFF);
    pc = V[0] + NNN;
}

void chip8::opC(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 NN = (oc & 0x00FF);
    V[x] = (rand() % 256) & NN;
}

void chip8::opD(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    u8 y = (oc & 0x00F0) >> 4;
    u8 N = (oc & 0x000F);

    V[0xF] = 0;
    for (int row = 0; row < N; ++row) {
        u8 spriteByte = memory[I + row];
        for (int col = 0; col < 8; ++col) {
            if (spriteByte & (0x80 >> col)) {
                u8 px = (V[x] + col) % 64;
                u8 py = (V[y] + row) % 32;
                if (gfx[py * 64 + px]) V[0xF] = 1;
                gfx[py * 64 + px] ^= 1;
            }
        }
    }
    drawFlag = true;
}

void chip8::opE(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    switch (oc & 0x00FF) {
        case 0x009E:
            if (key[V[x]] != 0) pc += 2;
            break;
        case 0x00A1:
            if (key[V[x]] == 0) pc += 2;
            break;
    }
}

void chip8::opF(u16 oc) {
    u8 x = (oc & 0x0F00) >> 8;
    switch (oc & 0x00FF) {
        case 0x0007:
            V[x] = delayTimer;
            break;
        case 0x000A:
            getKeyPress();
            break;
        case 0x0015:
            delayTimer = V[x];
            break;
        case 0x0018:
            soundTimer = V[x];
            break;
        case 0x001E:
            I += V[x];
            break;
        case 0x0029:
            I = 0x50 + (V[x] * 5);
            break;
        case 0x0033:
            memory[I] = V[x] / 100;
            memory[I + 1] = (V[x] / 10) % 10;
            memory[I + 2] = V[x] % 10;
            break;
        case 0x0055:
            for (int i = 0; i <= x; ++i) {
                memory[I + i] = V[i];
            }
            break;
        case 0x0065:
            for (int i = 0; i <= x; ++i) {
                V[i] = memory[I + i];
            }
            break;
    }
}
