#ifndef RESOURCES_H
#define RESOURCES_H

#include <stdbool.h>

// Resource loading functions
bool initializeImages();

bool initializeFonts();

void cleanupResources();

#endif // RESOURCES_H