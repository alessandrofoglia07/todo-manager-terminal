#ifndef PRINT_H
#define PRINT_H

#include <stdbool.h>

void printDir(const char *location, char *target, bool *pNewTarget, char ***pSelectedDir, int *pCount);
void freeSelectedDir(char **selectedDir, int count);
void printCommands();

#endif