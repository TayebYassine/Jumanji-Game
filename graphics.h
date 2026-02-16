#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "types.h"

// Button rendering and interaction
bool isButtonHovered(MenuButton *btn, int mouseX, int mouseY);

void renderButton(MenuButton *btn);

void updateButtonHover(MenuButton *buttons[], int count, int mouseX, int mouseY);

// Text rendering
void renderText(const char *message, int xPos, int yPos, SDL_Color textColor, bool useLargeFont);

// Logo rendering
void renderGameLogo(int x, int y, int width, int height);

// Animation functions
void drawTypingText(const char *text, int x, int y, SDL_Color textColor, bool useLargeFont, Uint32 startTime,
                    int delayMs);

void initParticles();

void drawParticles();

#endif // GRAPHICS_H
