#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Game states
typedef enum {
    STATE_MAIN_MENU,
    STATE_OPTIONS_MENU,
    STATE_SAVE_SELECTION,
    STATE_PLAYER_SELECTION,
    STATE_AVATAR_SELECTION,
    STATE_HIGH_SCORES,
    STATE_PUZZLE_GAME,
    STATE_QUIT
} GameState;

// Button structure
typedef struct {
    SDL_Rect bounds;
    char label[50];
    bool isHovered;
} MenuButton;

// Player/Score structure
typedef struct {
    char name[64];
    int score;
} Joueur;

#endif // TYPES_H