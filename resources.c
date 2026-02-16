#include "resources.h"
#include "globals.h"
#include "audio.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

// Load all image resources
bool initializeImages() {
    mainBackground = IMG_LoadTexture(gameRenderer, "images/bg_main.jpg");
    stoneBackground = IMG_LoadTexture(gameRenderer, "images/bg_stone.jpg");
    greenBackground = IMG_LoadTexture(gameRenderer, "images/bg_green.jpg");
    rhinoBackground = IMG_LoadTexture(gameRenderer, "images/bg_rhino.jpg");

    gameLogo = IMG_LoadTexture(gameRenderer, "images/logo.png");
    if (!gameLogo) {
        gameLogo = IMG_LoadTexture(gameRenderer, "images/logo.jpg");
    }

    if (gameLogo) {
        printf("Logo charge avec succes!\n");
    } else {
        printf("Logo non trouve (images/logo.png ou logo.jpg)\n");
    }

    return true;
}

// Load font resources
bool initializeFonts() {
    normalFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    largeFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 48);

    if (!normalFont) {
        normalFont = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf", 24);
        largeFont = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf", 48);
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
    if (greenBackground) SDL_DestroyTexture(greenBackground);
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
    greenBackground = NULL;
    rhinoBackground = NULL;
    gameLogo = NULL;
    normalFont = NULL;
    largeFont = NULL;
}