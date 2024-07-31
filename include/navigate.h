#ifndef NAVIGATE_H
#define NAVIGATE_H

#include <stdbool.h>

void handleArrowUp(const char *location, char *target, bool *pNewTarget, char **selectedDir, int *pCount);
void handleArrowDown(const char *location, char *target, bool *pNewTarget, char **selectedDir, int *pCount);
void in(char *location, char *target, bool *pNewTarget);
void out(const char *location, char *target, bool *pNewTarget);

#endif