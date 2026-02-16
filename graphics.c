#include "graphics.h"
#include "globals.h"
#include "constants.h"
#include "audio.h"
#include <SDL2/SDL_ttf.h>

// Check if mouse is hovering over a button
bool isButtonHovered(MenuButton *btn, int mouseX, int mouseY) {
    return (mouseX >= btn->bounds.x &&
            mouseX <= btn->bounds.x + btn->bounds.w &&
            mouseY >= btn->bounds.y &&
            mouseY <= btn->bounds.y + btn->bounds.h);
}

// Update button hover states and play sound when hover changes
// FIX: Moved hover sound logic here to prevent spam
void updateButtonHover(MenuButton *buttons[], int count, int mouseX, int mouseY) {
    bool anyHovered = false;
    MenuButton *currentlyHovered = NULL;

    for (int i = 0; i < count; i++) {
        buttons[i]->isHovered = isButtonHovered(buttons[i], mouseX, mouseY);
        if (buttons[i]->isHovered) {
            anyHovered = true;
            currentlyHovered = buttons[i];
        }
    }

    // Play hover sound only when we start hovering a NEW button
    if (currentlyHovered != NULL && currentlyHovered != lastHoveredButton) {
        triggerHoverSound();
    }

    // Update the last hovered button
    lastHoveredButton = anyHovered ? currentlyHovered : NULL;
}

// Render a button with hover effects
void renderButton(MenuButton *btn) {
    if (!gameRenderer) return;

    SDL_Color borderColor = btn->isHovered ? (SDL_Color){255, 215, 0, 255} : (SDL_Color){255, 255, 255, 255};

    SDL_Color fillColor = btn->isHovered ? (SDL_Color){150, 100, 50, 200} : (SDL_Color){0, 0, 0, 150};

    SDL_SetRenderDrawColor(gameRenderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    SDL_RenderFillRect(gameRenderer, &btn->bounds);

    SDL_SetRenderDrawColor(gameRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(gameRenderer, &btn->bounds);

    if (normalFont) {
        SDL_Color textColor = btn->isHovered ? colorGreen : colorWhite;
        SDL_Surface *textSurface = TTF_RenderText_Blended(normalFont, btn->label, textColor);

        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gameRenderer, textSurface);
            if (textTexture) {
                SDL_Rect textPosition = {
                    btn->bounds.x + (btn->bounds.w - textSurface->w) / 2,
                    btn->bounds.y + (btn->bounds.h - textSurface->h) / 2,
                    textSurface->w,
                    textSurface->h
                };

                SDL_RenderCopy(gameRenderer, textTexture, NULL, &textPosition);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}

// Render text at specified position
void renderText(const char *message, int xPos, int yPos, SDL_Color textColor, bool useLargeFont) {
    if (!normalFont || !gameRenderer || !message) return;

    TTF_Font *selectedFont = (useLargeFont && largeFont) ? largeFont : normalFont;
    SDL_Surface *textSurface = TTF_RenderText_Blended(selectedFont, message, textColor);

    if (textSurface) {
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gameRenderer, textSurface);
        if (textTexture) {
            SDL_Rect position = {xPos, yPos, textSurface->w, textSurface->h};
            SDL_RenderCopy(gameRenderer, textTexture, NULL, &position);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

// Render game logo with circular background
void renderGameLogo(int x, int y, int width, int height) {
    if (!gameRenderer) return;

    // Draw green circle background
    SDL_SetRenderDrawColor(gameRenderer, 34, 139, 34, 255);
    int centerX = x + width / 2;
    int centerY = y + height / 2;
    int circleRadius = width / 2 + 10;

    for (int dy = -circleRadius; dy <= circleRadius; dy++) {
        for (int dx = -circleRadius; dx <= circleRadius; dx++) {
            if (dx * dx + dy * dy <= circleRadius * circleRadius) {
                SDL_RenderDrawPoint(gameRenderer, centerX + dx, centerY + dy);
            }
        }
    }

    // Render logo texture
    if (gameLogo) {
        SDL_Rect logoRect = {x, y, width, height};
        SDL_RenderCopy(gameRenderer, gameLogo, NULL, &logoRect);
    }
}
