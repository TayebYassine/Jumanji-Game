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
    clickSound = Mix_LoadWAV("sounds/click.mp3");

    if (mainTheme) {
        Mix_PlayMusic(mainTheme, -1);
    } else {
        printf("Error: menu_music.mp3 not found!");
    }

    if (!optionsTheme) printf("Error: options_music.mp3 not found!");
    if (!victoryTheme) printf("Error: victory_music.mp3 not found!");
    if (!hoverSound) printf("Error: hover.wav not found!");
    if (!clickSound) printf("Error: click.mp3 not found!");

    return true;
}

// Update background music based on current game state
void refreshBackgroundMusic() {
    static GameState previousState = -1;
    static Mix_Music *currentlyPlaying = NULL;

    if (activeState == previousState) return;
    previousState = activeState;

    // Determine which music should be playing for the current state
    Mix_Music *targetMusic = NULL;

    if (activeState == STATE_MAIN_MENU ||
        activeState == STATE_SAVE_SELECTION ||
        activeState == STATE_PLAYER_SELECTION ||
        activeState == STATE_AVATAR_SELECTION) {
        targetMusic = mainTheme;
    } else if (activeState == STATE_OPTIONS_MENU) {
        targetMusic = optionsTheme ? optionsTheme : mainTheme;
    } else if (activeState == STATE_HIGH_SCORES) {
        targetMusic = victoryTheme ? victoryTheme : mainTheme;
    } else if (activeState == STATE_PUZZLE_GAME) {
        targetMusic = mainTheme;
    }

    // Only change music if we're switching to a different track
    if (targetMusic != currentlyPlaying) {
        Mix_HaltMusic();
        if (targetMusic) {
            Mix_PlayMusic(targetMusic, -1);
            currentlyPlaying = targetMusic;
        } else {
            currentlyPlaying = NULL;
        }
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
    if (clickSound) Mix_FreeChunk(clickSound);

    mainTheme = NULL;
    optionsTheme = NULL;
    victoryTheme = NULL;
    clickSound = NULL;
}
