#ifndef NAVIGATE_H
#define NAVIGATE_H

#include <stdbool.h>

#define PATH_MAX_LENGTH 1024

#ifdef _WIN32
#define CLEAN_COMMAND "cls"
#else
#define CLEAN_COMMAND "clear"
#endif

void handleArrowUp(const char *location, char *target, bool *pNewTarget, char **selectedDir, int *pCount);
void handleArrowDown(const char *location, char *target, bool *pNewTarget, char **selectedDir, int *pCount);
void in(char *location, char *target, bool *pNewTarget);
void out(const char *location, char *target, bool *pNewTarget);

#endif