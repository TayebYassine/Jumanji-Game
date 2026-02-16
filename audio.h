#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

// Audio management functions
bool initializeSounds();

void refreshBackgroundMusic();

void triggerHoverSound();

void triggerClickSound();

void cleanupAudio();

#endif // AUDIO_H