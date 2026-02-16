#include "menus.h"
#include "globals.h"
#include "constants.h"
#include "types.h"
#include "graphics.h"
#include "audio.h"
#include "player_data.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

// Handle main menu events
static void handleMainMenuEvents(MenuButton * buttons[], int buttonCount) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            activeState = STATE_QUIT;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (buttons[0]->isHovered) {
                triggerClickSound();
                activeState = STATE_SAVE_SELECTION;
            } else if (buttons[1]->isHovered) {
                triggerClickSound();
                activeState = STATE_OPTIONS_MENU;
            } else if (buttons[2]->isHovered) {
                triggerClickSound();
                activeState = STATE_HIGH_SCORES;
            } else if (buttons[3]->isHovered) {
                triggerClickSound();
            } else if (buttons[4]->isHovered) {
                activeState = STATE_QUIT;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                activeState = STATE_QUIT;
            }
        }
    }
}

// Main menu display
void displayMainMenu() {
    // Background
    if (mainBackground) {
        SDL_RenderCopy(gameRenderer, mainBackground, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(gameRenderer, 34, 139, 34, 255);
        SDL_RenderClear(gameRenderer);
    }

    // Logo and title
    int logoXPos = 50;
    int logoYPos = 30;
    int logoSize = 200;

    renderGameLogo(logoXPos, logoYPos, logoSize, logoSize);
    renderText("JUMANJI", logoXPos + logoSize + 30, logoYPos + 50, colorGreen, true);
    renderText("BIENVENUE DANS LA JUNGLE", logoXPos + logoSize + 30, logoYPos + 110, colorWhite, false);

    // Create buttons
    int btnX = 80;
    int btnY = 280;
    int btnSpacing = 80;

    MenuButton playButton = {{btnX, btnY, 220, 60}, "JOUER", false};
    MenuButton optionsButton = {{btnX, btnY + btnSpacing, 220, 60}, "OPTIONS", false};
    MenuButton scoresButton = {{btnX, btnY + btnSpacing * 2, 220, 60}, "MEILLEURS SCORES", false};
    MenuButton storyButton = {{btnX, btnY + btnSpacing * 3, 220, 60}, "HISTOIRE", false};
    MenuButton quitButton = {{750, 650, 200, 60}, "QUITTER", false};

    MenuButton *allButtons[] = {&playButton, &optionsButton, &scoresButton, &storyButton, &quitButton};

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // FIX: Use centralized hover update function to prevent sound spam
    updateButtonHover(allButtons, 5, mouseX, mouseY);

    // Render all buttons
    for (int i = 0; i < 5; i++) {
        renderButton(allButtons[i]);
    }

    handleMainMenuEvents(allButtons, 5);
}

// Options menu display
void displayOptionsMenu() {
    if (stoneBackground) {
        SDL_RenderCopy(gameRenderer, stoneBackground, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(gameRenderer, 101, 67, 33, 255);
        SDL_RenderClear(gameRenderer);
    }

    renderText("OPTIONS", 400, 50, colorGreen, true);

    // Volume slider
    int sliderX = 300;
    int sliderY = 200;
    int sliderWidth = 400;

    SDL_Rect sliderBackground = {sliderX, sliderY, sliderWidth, 20};
    SDL_SetRenderDrawColor(gameRenderer, 100, 100, 100, 255);
    SDL_RenderFillRect(gameRenderer, &sliderBackground);

    int filledWidth = (musicVolume * sliderWidth) / 128;
    SDL_Rect sliderFill = {sliderX, sliderY, filledWidth, 20};
    SDL_SetRenderDrawColor(gameRenderer, 0, 255, 0, 255);
    SDL_RenderFillRect(gameRenderer, &sliderFill);

    char volumeLabel[32];
    sprintf(volumeLabel, "Volume: %d%%", (musicVolume * 100) / 128);
    renderText(volumeLabel, sliderX, sliderY - 40, colorWhite, false);

    // Fullscreen button
    MenuButton fullscreenButton = {
        {300, 300, 400, 60},
        isFullscreenMode ? "PLEIN ECRAN: OUI" : "PLEIN ECRAN: NON", false
    };
    MenuButton backButton = {{800, 650, 150, 50}, "RETOUR", false};

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MenuButton *buttons[] = {&fullscreenButton, &backButton};
    updateButtonHover(buttons, 2, mouseX, mouseY);

    renderButton(&fullscreenButton);
    renderButton(&backButton);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            activeState = STATE_QUIT;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (fullscreenButton.isHovered) {
                isFullscreenMode = !isFullscreenMode;
                SDL_SetWindowFullscreen(gameWindow, isFullscreenMode ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            } else if (backButton.isHovered) {
                activeState = STATE_MAIN_MENU;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                activeState = STATE_MAIN_MENU;
            } else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_RIGHT) {
                if (musicVolume < 128) {
                    musicVolume += 5;
                    Mix_VolumeMusic(musicVolume);
                }
            } else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_LEFT) {
                if (musicVolume > 0) {
                    musicVolume -= 5;
                    Mix_VolumeMusic(musicVolume);
                }
            }
        }
    }
}

// Save selection menu
void displaySaveMenu() {
    if (stoneBackground) {
        SDL_RenderCopy(gameRenderer, stoneBackground, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(gameRenderer, 101, 67, 33, 255);
        SDL_RenderClear(gameRenderer);
    }

    renderText("CHOISISSEZ UNE SAUVEGARDE", 250, 50, colorGreen, true);

    MenuButton slot1Button = {{300, 200, 400, 80}, "PARTIE 1 - NOUVELLE PARTIE", false};
    MenuButton slot2Button = {{300, 320, 400, 80}, "PARTIE 2 - NOUVELLE PARTIE", false};
    MenuButton slot3Button = {{300, 440, 400, 80}, "PARTIE 3 - NOUVELLE PARTIE", false};
    MenuButton backButton = {{800, 650, 150, 50}, "RETOUR", false};

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MenuButton *buttons[] = {&slot1Button, &slot2Button, &slot3Button, &backButton};
    updateButtonHover(buttons, 4, mouseX, mouseY);

    renderButton(&slot1Button);
    renderButton(&slot2Button);
    renderButton(&slot3Button);
    renderButton(&backButton);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            activeState = STATE_QUIT;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (slot1Button.isHovered || slot2Button.isHovered || slot3Button.isHovered) {
                activeState = STATE_PLAYER_SELECTION;
            } else if (backButton.isHovered) {
                activeState = STATE_MAIN_MENU;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                activeState = STATE_MAIN_MENU;
            }
        }
    }
}

// Player selection menu
void displayPlayerMenu() {
    if (stoneBackground) {
        SDL_RenderCopy(gameRenderer, stoneBackground, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(gameRenderer, 101, 67, 33, 255);
        SDL_RenderClear(gameRenderer);
    }

    renderText("NOMBRE DE JOUEURS", 350, 50, colorGreen, true);

    MenuButton singlePlayerButton = {{300, 250, 400, 100}, "1 JOUEUR", false};
    MenuButton multiPlayerButton = {{300, 400, 400, 100}, "MULTI-JOUEURS", false};
    MenuButton backButton = {{800, 650, 150, 50}, "RETOUR", false};

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MenuButton *buttons[] = {&singlePlayerButton, &multiPlayerButton, &backButton};
    updateButtonHover(buttons, 3, mouseX, mouseY);

    renderButton(&singlePlayerButton);
    renderButton(&multiPlayerButton);
    renderButton(&backButton);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            activeState = STATE_QUIT;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (singlePlayerButton.isHovered || multiPlayerButton.isHovered) {
                activeState = STATE_AVATAR_SELECTION;
            } else if (backButton.isHovered) {
                activeState = STATE_SAVE_SELECTION;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                activeState = STATE_SAVE_SELECTION;
            }
        }
    }
}

// Avatar selection menu
void displayAvatarMenu() {
    if (stoneBackground) {
        SDL_RenderCopy(gameRenderer, stoneBackground, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(gameRenderer, 101, 67, 33, 255);
        SDL_RenderClear(gameRenderer);
    }

    renderText("CHOISISSEZ VOTRE AVATAR", 300, 50, colorGreen, true);

    MenuButton braveButton = {{150, 150, 280, 100}, "DR. BRAVESTONES", false};
    MenuButton shellyButton = {{550, 150, 280, 100}, "PROF. SHELLY", false};
    MenuButton mouseButton = {{150, 300, 280, 100}, "MOUSE FINBAR", false};
    MenuButton rubyButton = {{550, 300, 280, 100}, "RUBY ROUNDHOUSE", false};
    MenuButton confirmButton = {{450, 500, 150, 60}, "VALIDER", false};
    MenuButton backButton = {{800, 650, 150, 50}, "RETOUR", false};

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MenuButton *buttons[] = {&braveButton, &shellyButton, &mouseButton, &rubyButton, &confirmButton, &backButton};
    updateButtonHover(buttons, 6, mouseX, mouseY);

    // Draw golden border for selected avatar
    SDL_SetRenderDrawColor(gameRenderer, 255, 215, 0, 255);
    if (chosenAvatar == 1) {
        SDL_Rect highlight = {
            braveButton.bounds.x - 5, braveButton.bounds.y - 5,
            braveButton.bounds.w + 10, braveButton.bounds.h + 10
        };
        SDL_RenderDrawRect(gameRenderer, &highlight);
    } else if (chosenAvatar == 2) {
        SDL_Rect highlight = {
            shellyButton.bounds.x - 5, shellyButton.bounds.y - 5,
            shellyButton.bounds.w + 10, shellyButton.bounds.h + 10
        };
        SDL_RenderDrawRect(gameRenderer, &highlight);
    } else if (chosenAvatar == 3) {
        SDL_Rect highlight = {
            mouseButton.bounds.x - 5, mouseButton.bounds.y - 5,
            mouseButton.bounds.w + 10, mouseButton.bounds.h + 10
        };
        SDL_RenderDrawRect(gameRenderer, &highlight);
    } else if (chosenAvatar == 4) {
        SDL_Rect highlight = {
            rubyButton.bounds.x - 5, rubyButton.bounds.y - 5,
            rubyButton.bounds.w + 10, rubyButton.bounds.h + 10
        };
        SDL_RenderDrawRect(gameRenderer, &highlight);
    }

    renderButton(&braveButton);
    renderButton(&shellyButton);
    renderButton(&mouseButton);
    renderButton(&rubyButton);
    renderButton(&confirmButton);
    renderButton(&backButton);

    renderText("Force, Vitesse", 180, 260, colorWhite, false);
    renderText("Cartographie", 590, 260, colorWhite, false);
    renderText("Zoologie", 200, 410, colorWhite, false);
    renderText("Combat", 620, 410, colorWhite, false);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            activeState = STATE_QUIT;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (braveButton.isHovered) chosenAvatar = 1;
            else if (shellyButton.isHovered) chosenAvatar = 2;
            else if (mouseButton.isHovered) chosenAvatar = 3;
            else if (rubyButton.isHovered) chosenAvatar = 4;
            else if (confirmButton.isHovered && chosenAvatar != 0) {
                activeState = STATE_HIGH_SCORES;
            } else if (backButton.isHovered) {
                activeState = STATE_PLAYER_SELECTION;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_RETURN && chosenAvatar != 0) {
                activeState = STATE_HIGH_SCORES;
            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                activeState = STATE_PLAYER_SELECTION;
            }
        }
    }
}

// High scores menu
void displayHighScoresMenu() {
    if (greenBackground) {
        SDL_RenderCopy(gameRenderer, greenBackground, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(gameRenderer, 218, 165, 32, 255);
        SDL_RenderClear(gameRenderer);
    }

    renderText("MEILLEURS SCORES", 350, 50, colorBlack, true);

    Joueur joueurs[NOMBRE_JOEURS];
    int numPlayers = charger_joueurs_tries("joueurs.bin", joueurs);

    // FIX: Use centralized default loading
    if (numPlayers == 0) {
        loadDefaultPlayers(joueurs);
        numPlayers = NOMBRE_JOEURS;
    }

    for (int rank = 0; rank < numPlayers; rank++) {
        int yPosition = 200 + rank * 150;

        SDL_Rect starRect = {330, yPosition, 80, 80};
        SDL_SetRenderDrawColor(gameRenderer, 255, 215, 0, 255);
        SDL_RenderFillRect(gameRenderer, &starRect);

        char rankText[4];
        sprintf(rankText, "#%d", rank + 1);
        renderText(rankText, 335, yPosition + 25, colorBlack, true);
        renderText(joueurs[rank].name, 420, yPosition + 10, colorWhite, false);

        char scoreText[16];
        sprintf(scoreText, "%d", joueurs[rank].score);
        renderText(scoreText, 420, yPosition + 50, colorBlack, false);
    }

    MenuButton quitButton = {{300, 650, 150, 50}, "QUITTER", false};
    MenuButton backButton = {{550, 650, 150, 50}, "RETOUR", false};

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MenuButton *buttons[] = {&quitButton, &backButton};
    updateButtonHover(buttons, 2, mouseX, mouseY);

    renderButton(&quitButton);
    renderButton(&backButton);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            activeState = STATE_QUIT;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (quitButton.isHovered) {
                activeState = STATE_QUIT;
            } else if (backButton.isHovered) {
                activeState = STATE_MAIN_MENU;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_e) {
                activeState = STATE_PUZZLE_GAME;
            } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                activeState = STATE_MAIN_MENU;
            }
        }
    }
}

// Puzzle/challenges menu
void displayPuzzleMenu() {
    if (rhinoBackground) {
        SDL_RenderCopy(gameRenderer, rhinoBackground, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(gameRenderer, 105, 105, 105, 255);
        SDL_RenderClear(gameRenderer);
    }

    renderText("DEFIS JUMANJI", 400, 150, colorGreen, true);

    MenuButton quizButton = {{300, 300, 200, 80}, "QUIZ", false};
    MenuButton puzzleButton = {{550, 300, 200, 80}, "PUZZLE", false};
    MenuButton backButton = {{800, 650, 150, 50}, "RETOUR", false};

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    MenuButton *buttons[] = {&quizButton, &puzzleButton, &backButton};
    updateButtonHover(buttons, 3, mouseX, mouseY);

    renderButton(&quizButton);
    renderButton(&puzzleButton);
    renderButton(&backButton);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            activeState = STATE_QUIT;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (quizButton.isHovered) {
                printf("Quiz avec musique suspense!\n");
            } else if (puzzleButton.isHovered) {
                printf("Puzzle!\n");
            } else if (backButton.isHovered) {
                activeState = STATE_HIGH_SCORES;
            }
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                activeState = STATE_HIGH_SCORES;
            }
        }
    }
}