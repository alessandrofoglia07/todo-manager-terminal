#include <navigate.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <defaultDataPath.h>
#include <print.h>

#ifdef _WIN32
#define CLEAN_COMMAND "cls"
#else
#define CLEAN_COMMAND "clear"
#endif

void handleArrowUp(const char *location, char *target, bool *pNewTarget, char **selectedDir, int *pCount) {
    for (int i = 0; i < *pCount; i++) {
        if (strcmp(selectedDir[i], target) == 0) {
            if (i > 0) {
                strcpy(target, selectedDir[i - 1]);
            } else {
                strcpy(target, selectedDir[*pCount - 1]);
            }
            *pNewTarget = false;
            break;
        }
    }
    printDir(location, target, pNewTarget, &selectedDir, pCount);
}

void handleArrowDown(const char *location, char *target, bool *pNewTarget, char **selectedDir, int *pCount) {
    for (int i = 0; i < *pCount; i++) {
        if (strcmp(selectedDir[i], target) == 0) {
            if (i + 1 < *pCount) {
                strcpy(target, selectedDir[i + 1]);
            } else {
                strcpy(target, selectedDir[0]);
            }
            *pNewTarget = false;
            break;
        }
    }
    printDir(location, target, pNewTarget, &selectedDir, pCount);
}

void in(char *location, char *target, bool *pNewTarget) {
    if (strlen(target) < 1)
        return;
    const size_t newSize = strlen(location) + strlen(target) + 2;
    char *temp = (char *)malloc(newSize);
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strcpy(temp, location);
    strcat(temp, "/");
    strcat(temp, target);
    strcpy(location, temp);
    strcpy(target, "");
    *pNewTarget = true;
    free(temp);
}

void out(const char *location, char *target, bool *pNewTarget) {
    if (strlen(location) - strlen(DEFAULT_DATA_PATH) == 0)
        return;
    char *lastBackslash = strrchr(location, '/');
    if (lastBackslash != NULL) {
        strcpy(target, lastBackslash + 1);
        *lastBackslash = '\0';
        *pNewTarget = false;
    }
}
