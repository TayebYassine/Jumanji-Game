#include "player_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Load default values for each player
void loadDefaultPlayers(Joueur *joueurs) {
    strcpy(joueurs[0].name, "DR. BRAVESTONES");
    joueurs[0].score = 99999;

    strcpy(joueurs[1].name, "RUBY ROUNDHOUSE");
    joueurs[1].score = 87500;

    strcpy(joueurs[2].name, "MOUSE FINBAR");
    joueurs[2].score = 75000;
}

// Compare scores for sorting (descending order)
int comparer_scores(const void *a, const void *b) {
    Joueur *joueurA = (Joueur *) a;
    Joueur *joueurB = (Joueur *) b;

    return joueurB->score - joueurA->score;
}

// Save player data to file
void sauvegarder_joueurs(char filename[]) {
    Joueur joueurs[NOMBRE_JOEURS];
    loadDefaultPlayers(joueurs);

    srand((unsigned int) time(NULL));

    for (int i = 0; i < NOMBRE_JOEURS; i++) {
        joueurs[i].score = rand() % 1000 + 1;
    }

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return;
    }

    int numPlayers = NOMBRE_JOEURS;
    size_t ecrit = fwrite(&numPlayers, sizeof(int), 1, file);
    if (ecrit != 1) {
        fclose(file);
        return;
    }

    ecrit = fwrite(joueurs, sizeof(Joueur), NOMBRE_JOEURS, file);
    if (ecrit != NOMBRE_JOEURS) {
        fclose(file);
        return;
    }

    fclose(file);
}

// Load player data from file (sorted by score)
int charger_joueurs_tries(char filename[], Joueur *joueurs) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        // Use centralized default player data
        loadDefaultPlayers(joueurs);
        return NOMBRE_JOEURS;
    }

    int numPlayers;
    if (fread(&numPlayers, sizeof(int), 1, file) != 1) {
        fclose(file);
        loadDefaultPlayers(joueurs);
        return NOMBRE_JOEURS;
    }

    if (numPlayers > NOMBRE_JOEURS) {
        numPlayers = NOMBRE_JOEURS;
    }

    if (fread(joueurs, sizeof(Joueur), numPlayers, file) != (size_t) numPlayers) {
        fclose(file);
        loadDefaultPlayers(joueurs);
        return NOMBRE_JOEURS;
    }

    fclose(file);

    // Sort by score
    qsort(joueurs, numPlayers, sizeof(Joueur), comparer_scores);

    return numPlayers;
}
