#include "chip8.h"
#include <SDL2/SDL.h>
#include <iostream>

struct AudioState {
    int phase;
    int sampleRate;
    int freq;
};

void audioCallback(void* userdata, Uint8* stream, int len) {
    auto* state = static_cast<AudioState*>(userdata);
    Sint16* samples = reinterpret_cast<Sint16*>(stream);
    int count = len / 2;
    int period = state->sampleRate / state->freq;
    for (int i = 0; i < count; ++i) {
        samples[i] = (state->phase < period / 2) ? 2000 : -2000;
        state->phase = (state->phase + 1) % period;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <rom_path> [cycles_per_frame]\n";
        return 1;
    }

    int cyclesPerFrame = (argc >= 3) ? std::atoi(argv[2]) : 10;
    if (cyclesPerFrame < 1) cyclesPerFrame = 1;

    chip8 chip8vm;
    chip8vm.initialize();
    chip8vm.loadGame(argv[1]);

    // audio and graphics initalization and configuration
    const int SCALE = 10; // 64*10=640, 32*10=320
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    AudioState audioState = { 0, 44100, 440 };
    SDL_AudioSpec want = {}, have;
    want.freq     = audioState.sampleRate;
    want.format   = AUDIO_S16SYS;
    want.channels = 1;
    want.samples  = 512;
    want.callback = audioCallback;
    want.userdata = &audioState;
    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    SDL_PauseAudioDevice(audioDevice, 1); // start silent

    SDL_Window*   window   = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture*  texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);


    // CHIP-8 key -> SDL scancode mapping
    // CHIP-8: 1 2 3 C      Keyboard: 1 2 3 4
    //         4 5 6 D                Q W E R
    //         7 8 9 E                A S D F
    //         A 0 B F                Z X C V
    static const SDL_Scancode keymap[16] = {
        SDL_SCANCODE_X,  // 0
        SDL_SCANCODE_1,  // 1
        SDL_SCANCODE_2,  // 2
        SDL_SCANCODE_3,  // 3
        SDL_SCANCODE_Q,  // 4
        SDL_SCANCODE_W,  // 5
        SDL_SCANCODE_E,  // 6
        SDL_SCANCODE_A,  // 7
        SDL_SCANCODE_S,  // 8
        SDL_SCANCODE_D,  // 9
        SDL_SCANCODE_Z,  // A
        SDL_SCANCODE_C,  // B
        SDL_SCANCODE_4,  // C
        SDL_SCANCODE_R,  // D
        SDL_SCANCODE_F,  // E
        SDL_SCANCODE_V,  // F
    };

    for (;;) {
        u32 startTime = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) goto cleanup;
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                u8 state = (event.type == SDL_KEYDOWN) ? 1 : 0;
                SDL_Scancode sc = event.key.keysym.scancode;
                for (int i = 0; i < 16; ++i) {
                    if (sc == keymap[i]) {
                        chip8vm.setKey(i, state);
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < cyclesPerFrame; ++i)
            chip8vm.emulateCycle();
        chip8vm.tickTimers();
        SDL_PauseAudioDevice(audioDevice, chip8vm.soundActive() ? 0 : 1);

        if (chip8vm.drawFlag) {
        u32 pixels[64 * 32];
        for (int i = 0; i < 64 * 32; ++i) {
            pixels[i] = chip8vm.gfx[i] ? 0xFFFFFFFF : 0x00000000;
        }
        SDL_UpdateTexture(texture, nullptr, pixels, 64 * sizeof(u32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        chip8vm.drawFlag = false;
    }

        u32 elapsedTime = SDL_GetTicks() - startTime;
       if (elapsedTime < 16.6) SDL_Delay(16.6 - elapsedTime);
    }

cleanup:
    SDL_CloseAudioDevice(audioDevice);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}