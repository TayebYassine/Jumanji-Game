#include "globals.h"

// Graphics resources
SDL_Window *gameWindow = NULL;
SDL_Renderer *gameRenderer = NULL;
TTF_Font *normalFont = NULL;
TTF_Font *largeFont = NULL;

// Texture resources
SDL_Texture *mainBackground = NULL;
SDL_Texture *stoneBackground = NULL;
SDL_Texture *greenBackground = NULL;
SDL_Texture *rhinoBackground = NULL;
SDL_Texture *gameLogo = NULL;

// Audio resources
Mix_Music *mainTheme = NULL;
Mix_Music *optionsTheme = NULL;
Mix_Music *victoryTheme = NULL;
Mix_Chunk *hoverSound = NULL;
Mix_Chunk *clickSound = NULL;

// Game state variables
GameState activeState = STATE_MAIN_MENU;
int musicVolume = 64;
bool isFullscreenMode = false;
int chosenAvatar = 0;

// Hover tracking (fixed hover sound spam bug)
MenuButton *lastHoveredButton = NULL;

// Particle system
Particle leaves[30];
bool particlesInitialized = false;
Uint32 introStartTime = 0;
bool introInitialized = false;
