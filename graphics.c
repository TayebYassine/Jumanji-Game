#include "graphics.h"
#include "globals.h"
#include "constants.h"
#include "audio.h"
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>

// Check if mouse is hovering over a button
bool isButtonHovered(MenuButton *btn, int mouseX, int mouseY) {
    return (mouseX >= btn->bounds.x &&
            mouseX <= btn->bounds.x + btn->bounds.w &&
            mouseY >= btn->bounds.y &&
            mouseY <= btn->bounds.y + btn->bounds.h);
}

// Update button hover states and play sound when hover changes
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

void renderFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int w = 0; w <= radius * 2; w++) {
        for (int h = 0; h <= radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
            }
        }
    }
}

void renderRoundedRect(SDL_Renderer *renderer, SDL_Rect *rect, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Clamp radius to half the smaller dimension
    int maxRadius = (rect->w < rect->h ? rect->w : rect->h) / 2;
    if (radius > maxRadius) radius = maxRadius;

    // Fill center rectangles
    SDL_Rect centerRect = {
        rect->x + radius,
        rect->y,
        rect->w - radius * 2,
        rect->h
    };
    SDL_RenderFillRect(renderer, &centerRect);

    SDL_Rect sideRect = {
        rect->x,
        rect->y + radius,
        rect->w,
        rect->h - radius * 2
    };
    SDL_RenderFillRect(renderer, &sideRect);

    // Fill corner circles
    renderFilledCircle(renderer, rect->x + radius, rect->y + radius, radius);
    renderFilledCircle(renderer, rect->x + rect->w - radius - 1, rect->y + radius, radius);
    renderFilledCircle(renderer, rect->x + radius, rect->y + rect->h - radius - 1, radius);
    renderFilledCircle(renderer, rect->x + rect->w - radius - 1, rect->y + rect->h - radius - 1, radius);
}

void renderRoundedBorder(SDL_Renderer *renderer, SDL_Rect *rect, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int maxRadius = (rect->w < rect->h ? rect->w : rect->h) / 2;
    if (radius > maxRadius) radius = maxRadius;

    // Draw straight line segments
    // Top and bottom
    for (int x = rect->x + radius; x < rect->x + rect->w - radius; x++) {
        SDL_RenderDrawPoint(renderer, x, rect->y);
        SDL_RenderDrawPoint(renderer, x, rect->y + rect->h - 1);
    }
    // Left and right
    for (int y = rect->y + radius; y < rect->y + rect->h - radius; y++) {
        SDL_RenderDrawPoint(renderer, rect->x, y);
        SDL_RenderDrawPoint(renderer, rect->x + rect->w - 1, y);
    }

    // Draw corner arcs (using Bresenham-like approach for cleaner lines)
    int cx[4] = {rect->x + radius, rect->x + rect->w - radius - 1,
                 rect->x + radius, rect->x + rect->w - radius - 1};
    int cy[4] = {rect->y + radius, rect->y + radius,
                 rect->y + rect->h - radius - 1, rect->y + rect->h - radius - 1};
    int startAngles[4] = {180, 270, 90, 0};

    for (int i = 0; i < 4; i++) {
        int x = radius;
        int y = 0;
        int err = 0;

        while (x >= y) {
            // Draw arc points based on quadrant
            switch(i) {
                case 0: // Top-left
                    SDL_RenderDrawPoint(renderer, cx[i] - x, cy[i] - y);
                    SDL_RenderDrawPoint(renderer, cx[i] - y, cy[i] - x);
                    break;
                case 1: // Top-right
                    SDL_RenderDrawPoint(renderer, cx[i] + x, cy[i] - y);
                    SDL_RenderDrawPoint(renderer, cx[i] + y, cy[i] - x);
                    break;
                case 2: // Bottom-left
                    SDL_RenderDrawPoint(renderer, cx[i] - x, cy[i] + y);
                    SDL_RenderDrawPoint(renderer, cx[i] - y, cy[i] + x);
                    break;
                case 3: // Bottom-right
                    SDL_RenderDrawPoint(renderer, cx[i] + x, cy[i] + y);
                    SDL_RenderDrawPoint(renderer, cx[i] + y, cy[i] + x);
                    break;
            }
            y++;
            err += 1 + 2*y;
            if (2*(err-x) + 1 > 0) {
                x--;
                err += 1 - 2*x;
            }
        }
    }
}

void renderButton(MenuButton *btn) {
    if (!gameRenderer) return;

    SDL_Color borderColor = btn->isHovered ? (SDL_Color){255, 215, 0, 255} : (SDL_Color){255, 255, 255, 255};
    SDL_Color fillColor = btn->isHovered ? (SDL_Color){150, 100, 50, 200} : (SDL_Color){0, 0, 0, 150};

    int cornerRadius = 8; // Adjust this for more/less rounding

    // Fill rounded rectangle
    renderRoundedRect(gameRenderer, &btn->bounds, cornerRadius, fillColor);

    // Draw rounded border
    renderRoundedBorder(gameRenderer, &btn->bounds, cornerRadius, borderColor);

    if (normalFont) {
        SDL_Color textColor = btn->isHovered ? colorGold : colorWhite;
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

// Initialize particle system
void initParticles() {
    if (particlesInitialized) return;

    for (int i = 0; i < 30; i++) {
        leaves[i].x = rand() % WINDOW_WIDTH;
        leaves[i].y = rand() % WINDOW_HEIGHT;
        leaves[i].speed = 0.5 + (rand() % 20) / 10.0;
        leaves[i].rotation = rand() % 360;
        leaves[i].size = 8 + rand() % 8;
        leaves[i].type = rand() % 2;
    }

    particlesInitialized = true;
}

// Draw animated falling particles
void drawParticles() {
    if (!gameRenderer) return;

    initParticles();

    for (int i = 0; i < 30; i++) {
        // Color based on type
        if (leaves[i].type == 0) {
            // Green jungle
            SDL_SetRenderDrawColor(gameRenderer, 34 + rand() % 50, 139 + rand() % 50, 34, 200);
        } else {
            // Gold
            SDL_SetRenderDrawColor(gameRenderer, 218, 165, 32, 180);
        }

        // Rotate leaves
        leaves[i].rotation += 2;
        float rad = leaves[i].rotation * 3.14159f / 180.0f;

        // Draw leaf (oval shape)
        for (int fy = -leaves[i].size / 2; fy < leaves[i].size / 2; fy++) {
            for (int fx = -leaves[i].size / 3; fx < leaves[i].size / 3; fx++) {
                // Rotation
                int rx = (int) (fx * cos(rad) - fy * sin(rad));
                int ry = (int) (fx * sin(rad) + fy * cos(rad));

                int px = (int) leaves[i].x + rx;
                int py = (int) leaves[i].y + ry;

                if (px >= 0 && px < WINDOW_WIDTH && py >= 0 && py < WINDOW_HEIGHT) {
                    SDL_RenderDrawPoint(gameRenderer, px, py);
                }
            }
        }

        // Falling movement with oscillation
        leaves[i].y += leaves[i].speed;
        leaves[i].x += sin(leaves[i].y / 50.0) * 1.5;

        // Reset if off screen
        if (leaves[i].y > WINDOW_HEIGHT + 20) {
            leaves[i].y = -20;
            leaves[i].x = rand() % WINDOW_WIDTH;
        }
        if (leaves[i].x < -20) leaves[i].x = WINDOW_WIDTH + 20;
        if (leaves[i].x > WINDOW_WIDTH + 20) leaves[i].x = -20;
    }
}

// Draw text with typing animation effect
void drawTypingText(const char *text, int x, int y, SDL_Color textColor, bool useLargeFont, Uint32 startTime,
                    int delayMs) {
    if (!normalFont || !gameRenderer || !text) return;

    char buffer[256];
    int len = strlen(text);

    // Calculate how many letters to show
    Uint32 elapsed = SDL_GetTicks() - startTime;
    int charsToShow = elapsed / delayMs;

    if (charsToShow > len) charsToShow = len;

    // Copy letters to show
    strncpy(buffer, text, charsToShow);
    buffer[charsToShow] = '\0';

    // Draw text
    renderText(buffer, x, y, textColor, useLargeFont);

    // Blinking cursor
    if (charsToShow < len && (elapsed / 500) % 2 == 0) {
        int textW, textH;
        TTF_Font *f = useLargeFont ? largeFont : normalFont;
        TTF_SizeText(f, buffer, &textW, &textH);

        SDL_Rect cursor = {x + textW + 5, y + 5, 3, textH - 10};
        SDL_SetRenderDrawColor(gameRenderer, textColor.r, textColor.g, textColor.b, 255);
        SDL_RenderFillRect(gameRenderer, &cursor);
    }
}
