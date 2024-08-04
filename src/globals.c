#include <globals.h>

#include <stdbool.h>
#include <stdio.h>

char location[MAX_PATH_LENGTH] = DEFAULT_DATA_PATH;
char target[FILENAME_MAX];
bool newTarget = true;
char **selectedDir = NULL;
int count = 0;