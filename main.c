#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#include "types.h"
#include "constants.h"
#include "globals.h"
#include "graphics.h"
#include "audio.h"
#include "resources.h"
#include "player_data.h"
#include "menus.h"

// Initialize SDL and all subsystems
bool initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        return false;
    }

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    if (TTF_Init() == -1) {
        printf("TTF Error: %s\n", TTF_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mixer Error: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

// Create game window and renderer
bool createWindow() {
    gameWindow = SDL_CreateWindow("JUMANJI",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT,
                                  SDL_WINDOW_SHOWN);
    if (!gameWindow) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gameRenderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Main game loop
void runGameLoop() {
    bool isRunning = true;
    while (isRunning && activeState != STATE_QUIT) {
        refreshBackgroundMusic();
        SDL_RenderClear(gameRenderer);

        switch (activeState) {
            case STATE_MAIN_MENU:
                displayMainMenu();
                break;
            case STATE_OPTIONS_MENU:
                displayOptionsMenu();
                break;
            case STATE_SAVE_SELECTION:
                displaySaveMenu();
                break;
            case STATE_PLAYER_SELECTION:
                displayPlayerMenu();
                break;
            case STATE_AVATAR_SELECTION:
                displayAvatarMenu();
                break;
            case STATE_HIGH_SCORES:
                displayHighScoresMenu();
                break;
            case STATE_PUZZLE_GAME:
                displayPuzzleMenu();
                break;
            default:
                break;
        }

        SDL_RenderPresent(gameRenderer);
        SDL_Delay(FPS_DELAY);
    }
}

// Cleanup and shutdown
void cleanup() {
    cleanupResources();

    Mix_CloseAudio();
    IMG_Quit();

    if (gameRenderer) SDL_DestroyRenderer(gameRenderer);
    if (gameWindow) SDL_DestroyWindow(gameWindow);

    TTF_Quit();
    SDL_Quit();
}

// Main entry point
int main(int argc, char *argv[]) {
    // Initialize SDL subsystems
    if (!initializeSDL()) {
        return 1;
    }

    // Create window and renderer
    if (!createWindow()) {
        cleanup();
        return 1;
    }

    // Load resources
    if (!initializeFonts()) {
        printf("Failed to load fonts!\n");
        cleanup();
        return 1;
    }

    initializeImages();
    initializeSounds();
    Mix_VolumeMusic(musicVolume);

    // Initialize player data
    sauvegarder_joueurs("joueurs.bin");

    // Run main game loop
    runGameLoop();

    // Cleanup
    cleanup();

    return 0;
}