# CHIP-8 Interpreter

## Requirements
- g++ 17
- cmake
- ninja
- sdl2

## Build
```bash
cmake -S . -B build -G Ninja
cmake --build build
```

## Run
```bash
./build/chip8 <rom_path>
```

Example:
```bash
./build/chip8 roms/Tetris.ch8
```

Available ROMs in the `roms/` directory:
- `roms/Tetris.ch8`
- `roms/Space_Invaders.ch8`
- `roms/Soccer.ch8`