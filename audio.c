#include "audio.h"
#include "globals.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

// Load all audio resources
bool initializeSounds() {
    mainTheme = Mix_LoadMUS("sounds/menu_music.mp3");
    optionsTheme = Mix_LoadMUS("sounds/options_music.mp3");
    victoryTheme = Mix_LoadMUS("sounds/victory_music.mp3");
    hoverSound = Mix_LoadWAV("sounds/hover.wav");
    clickSound = Mix_LoadWAV("sounds/click.wav");

    if (!mainTheme) {
        printf("menu_music.mp3 non trouve\n");
    } else {
        Mix_PlayMusic(mainTheme, -1);
    }

    return true;
}

// Update background music based on current game state
void refreshBackgroundMusic() {
    static GameState previousState = -1;

    if (activeState == previousState) return;
    previousState = activeState;

    Mix_HaltMusic();

    if (activeState == STATE_MAIN_MENU ||
        activeState == STATE_SAVE_SELECTION ||
        activeState == STATE_PLAYER_SELECTION ||
        activeState == STATE_AVATAR_SELECTION) {
        if (mainTheme) Mix_PlayMusic(mainTheme, -1);
    } else if (activeState == STATE_OPTIONS_MENU) {
        if (optionsTheme) {
            Mix_PlayMusic(optionsTheme, -1);
        } else if (mainTheme) {
            Mix_PlayMusic(mainTheme, -1);
        }
    } else if (activeState == STATE_HIGH_SCORES) {
        if (victoryTheme) Mix_PlayMusic(victoryTheme, -1);
    }
}

// Play hover sound effect
void triggerHoverSound() {
    if (hoverSound) Mix_PlayChannel(-1, hoverSound, 0);
}

// Play click sound effect
void triggerClickSound() {
    if (clickSound) Mix_PlayChannel(-1, clickSound, 0);
}

// Cleanup audio resources
void cleanupAudio() {
    if (mainTheme) Mix_FreeMusic(mainTheme);
    if (optionsTheme) Mix_FreeMusic(optionsTheme);
    if (victoryTheme) Mix_FreeMusic(victoryTheme);
    if (hoverSound) Mix_FreeChunk(hoverSound);
    if (clickSound) Mix_FreeChunk(clickSound);

    mainTheme = NULL;
    optionsTheme = NULL;
    victoryTheme = NULL;
    hoverSound = NULL;
    clickSound = NULL;
}