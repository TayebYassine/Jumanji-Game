#include "resources.h"
#include "globals.h"
#include "audio.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

// Load all image resources
bool initializeImages() {
    mainBackground = IMG_LoadTexture(gameRenderer, "images/bg_main.jpeg");
    stoneBackground = IMG_LoadTexture(gameRenderer, "images/bg_stone.jpeg");
    goldBackground = IMG_LoadTexture(gameRenderer, "images/bg_gold.jpeg");
    rhinoBackground = IMG_LoadTexture(gameRenderer, "images/bg_rhino.jpeg");

    gameLogo = IMG_LoadTexture(gameRenderer, "images/logo.png");
    if (!gameLogo) {
        gameLogo = IMG_LoadTexture(gameRenderer, "images/logo.jpg");
    }

    if (!gameLogo) printf("Error: logo.png not found!\n");
    if (!mainBackground) printf("Error: bg_main.jpeg not found!\n");
    if (!stoneBackground) printf("Error: bg_stone.jpeg not found!\n");
    if (!goldBackground) printf("Error: bg_gold.jpeg not found!\n");
    if (!rhinoBackground) printf("Error: bg_rhino.jpeg not found!\n");

    return true;
}

// Load font resources
bool initializeFonts() {
    normalFont = TTF_OpenFont("font/JetBrainsMono.ttf", 24);
    largeFont = TTF_OpenFont("font/JetBrainsMono.ttf", 48);

    if (!normalFont) {
        normalFont = TTF_OpenFont("font/JetBrainsMono.ttf", 24);
        largeFont = TTF_OpenFont("font/JetBrainsMono.ttf", 48);
    }

    if (!normalFont) {
        printf("Could not load fonts!\n");
        return false;
    }

    return true;
}

// Cleanup all resources
void cleanupResources() {
    // Cleanup textures
    if (mainBackground) SDL_DestroyTexture(mainBackground);
    if (stoneBackground) SDL_DestroyTexture(stoneBackground);
    if (goldBackground) SDL_DestroyTexture(goldBackground);
    if (rhinoBackground) SDL_DestroyTexture(rhinoBackground);
    if (gameLogo) SDL_DestroyTexture(gameLogo);

    // Cleanup fonts
    if (normalFont) TTF_CloseFont(normalFont);
    if (largeFont) TTF_CloseFont(largeFont);

    // Cleanup audio
    cleanupAudio();

    // Reset pointers
    mainBackground = NULL;
    stoneBackground = NULL;
    goldBackground = NULL;
    rhinoBackground = NULL;
    gameLogo = NULL;
    normalFont = NULL;
    largeFont = NULL;
}
