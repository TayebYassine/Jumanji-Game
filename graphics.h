#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "types.h"

// Button rendering and interaction
bool isButtonHovered(MenuButton *btn, int mouseX, int mouseY);

void renderButton(MenuButton * btn);
void updateButtonHover(MenuButton * buttons[], int count, int mouseX, int mouseY);

// Text rendering
void renderText(const char *message, int xPos, int yPos, SDL_Color textColor, bool useLargeFont);

// Logo rendering
void renderGameLogo(int x, int y, int width, int height);

#endif // GRAPHICS_H