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
./build/chip8 <rom_path> [cycles_per_frame]
```

`cycles_per_frame` controls emulation speed (default: 10). Higher = faster. Tune this per game since CHIP-8 has no standard clock speed.

Example:
```bash
./build/chip8 roms/Tetris.ch8        # default speed
./build/chip8 roms/Tetris.ch8 12
./build/chip8 roms/Space_Invaders.ch8 20
./build/chip8 roms/Soccer.ch8 6
```

Available ROMs in the `roms/` directory:
- `roms/Tetris.ch8`
- `roms/Space_Invaders.ch8`
- `roms/Soccer.ch8`

## Controls

The CHIP-8 hex keypad is mapped to the keyboard as follows:

```
CHIP-8   Keyboard
-------  --------
1 2 3 C  1 2 3 4
4 5 6 D  Q W E R
7 8 9 E  A S D F
A 0 B F  Z X C V
```

### Tetris
| Key | Action        |
|-----|---------------|
| Q   | Rotate piece  |
| W   | Move left     |
| E   | Move right    |
| A   | Drop faster   |

### Space Invaders
| Key | Action     |
|-----|------------|
| Q   | Move left  |
| W   | Fire       |
| E   | Move right |

### Soccer
| Key | Action          |
|-----|-----------------|
| 1   | Player 1 up     |
| Q   | Player 1 down   |
| 4   | Player 2 up     |
| R   | Player 2 down   |