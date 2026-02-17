#ifndef MENUS_H
#define MENUS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

// Menu display functions
void displayMainMenu();

void displayOptionsMenu();

void displaySaveSlotsMenu();

void displaySaveMenu();

void displayPlayerMenu();

void displayAvatarMenu();

void displayHighScoresMenu();

void displayPuzzleMenu();

void displayIntroAnimation();

void displayQuizMenu();

void resetQuiz();

typedef struct {
    char question[256];
    char reponses[3][100];
    int bonneReponse;
} QuestionQuiz;

static QuestionQuiz questions[] = {
    {"Quel est le talent de Ruby Roundhouse?",
     {"Force et vitesse", "Cartographie", "Danse et combat"}, 2},
    {"Quelle est la specialite du Professeur Shelly?",
     {"Zoologie", "Paleontologie", "Medecine"}, 1},
    {"Quel pouvoir a Dr. Bravestone?",
     {"Vitesse et courage", "Invisibilite", "Telepathie"}, 0},
    {"Que controle Mouse Finbar?",
     {"Le temps", "Les animaux", "La meteo"}, 1}
};

static int currentQuestion = 0;
static int scoreQuiz = 0;
static bool quizMusicStarted = false;
static Mix_Music* suspenseMusic = NULL;

#endif // MENUS_H
