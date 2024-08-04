#include <navigate.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <globals.h>
#include <utils.h>

void handleArrowUp() {
    for (int i = 0; i < count; i++) {
        if (strcmp(selectedDir[i], target) == 0) {
            if (i > 0) {
                strcpy(target, selectedDir[i - 1]);
            } else {
                strcpy(target, selectedDir[count - 1]);
            }
            newTarget = false;
            break;
        }
    }
}

void handleArrowDown() {
    for (int i = 0; i < count; i++) {
        if (strcmp(selectedDir[i], target) == 0) {
            if (i + 1 < count) {
                strcpy(target, selectedDir[i + 1]);
            } else {
                strcpy(target, selectedDir[0]);
            }
            newTarget = false;
            break;
        }
    }
}

void in() {
    if (checkIfTodo(location) == 0)
        return;
    snprintf(location, MAX_PATH_LENGTH, PATH_FORMAT, location, target);
    strcpy(target, "");
    newTarget = true;
}

void out() {
    if (strlen(location) - strlen(DEFAULT_DATA_PATH) == 0)
        return;
    char *lastDivider = strrchr(location, PATH_DIVIDER);
    if (lastDivider != NULL) {
        strcpy(target, lastDivider + 1);
        *lastDivider = '\0';
        newTarget = false;
    }
}
