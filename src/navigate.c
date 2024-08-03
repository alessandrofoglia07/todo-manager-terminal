#include <navigate.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <defaultDataPath.h>
#include <pathFormats.h>
#include <print.h>

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
    snprintf(location, PATH_MAX_LENGTH, PATH_FORMAT, location, target);
    strcpy(target, "");
    *pNewTarget = true;
}

void out(const char *location, char *target, bool *pNewTarget) {
    if (strlen(location) - strlen(DEFAULT_DATA_PATH) == 0)
        return;
    char *lastDivider = strrchr(location, PATH_DIVIDER);
    if (lastDivider != NULL) {
        strcpy(target, lastDivider + 1);
        *lastDivider = '\0';
        *pNewTarget = false;
    }
}
