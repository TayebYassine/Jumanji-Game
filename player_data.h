#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "types.h"
#include "constants.h"

// Player data management
void loadDefaultPlayers(Joueur * joueurs);

int comparer_scores(const void *a, const void *b);

void sauvegarder_joueurs(const char *filename);

int charger_joueurs_tries(const char *filename, Joueur *joueurs);

#endif // PLAYER_DATA_H