#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "types.h"

// Graphics resources
extern SDL_Window *gameWindow;
extern SDL_Renderer *gameRenderer;
extern TTF_Font *normalFont;
extern TTF_Font *largeFont;

// Texture resources
extern SDL_Texture *mainBackground;
extern SDL_Texture *stoneBackground;
extern SDL_Texture *greenBackground;
extern SDL_Texture *rhinoBackground;
extern SDL_Texture *gameLogo;

// Audio resources
extern Mix_Music *mainTheme;
extern Mix_Music *optionsTheme;
extern Mix_Music *victoryTheme;
extern Mix_Chunk *hoverSound;
extern Mix_Chunk *clickSound;

// Game state variables
extern GameState activeState;
extern int musicVolume;
extern bool isFullscreenMode;
extern int chosenAvatar;

// Static variable for hover tracking (fixed hover sound spam bug)
extern MenuButton *lastHoveredButton;

#endif // GLOBALS_H