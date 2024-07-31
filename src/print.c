#include <print.h>

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <defaultDataPath.h>
#include <utils.h>

void printTodo(const char *location) {
    FILE *pF = fopen(location, "r");
    if (pF == NULL) {
        printf("Error in opening todo at location %s. \n", location);
        return;
    }
    char buffer[255];
    while (fgets(buffer, 255, pF)) {
        printf(" %s", buffer);
    }
    fclose(pF);
}

void printDir(const char *location, char *target, bool *pNewTarget, char ***pSelectedDir, int *pCount) {
    // ANSI escape code to reset the formatting
    const char *reset = "\033[0m";
    // ANSI escape code for cyan text
    const char *cyan = "\x1b[34m";
    // ANSI escape code for green text
    const char *green = "\x1b[32m";

    system(CLEAN_COMMAND);

    printf("%s\n**** TODO ****\n%s", cyan, reset);

    printf(" %s%s%s\n", green, location + strlen(DEFAULT_DATA_PATH), reset);

    if (checkIfTodo(location) == 0) {
        printTodo(location);
        printf("\n");
        return;
    }

    DIR *dir = NULL;
    struct dirent *entry;

    dir = opendir(location);

    if (dir == NULL) {
        printf("Error in opening directory at location %s.\n", location);
        return;
    }

    int size = 10;
    *pSelectedDir = (char **)malloc(size * sizeof(char *));
    *pCount = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)) {  // exclude "." and ".."
            if (*pCount >= size) {
                size *= 2;
                *pSelectedDir = (char **)realloc(*pSelectedDir, size * sizeof(char *));
            }

            (*pSelectedDir)[*pCount] = (char *)malloc((strlen(entry->d_name) + 1) * sizeof(char));
            strcpy((*pSelectedDir)[*pCount], entry->d_name);

            // ANSI escape code for underlined text
            const char *underline = "\033[4m";

            if (*pNewTarget) {
                // Underline the first entry and update the target
                if (checkIfTodo(target) == 0) {
                    printf(" %s%s%s\n", underline, entry->d_name, reset);
                } else {
                    printf(" %s%s%s ->\n", underline, entry->d_name, reset);
                }
                strcpy(target, entry->d_name);
                *pNewTarget = false;
            } else {
                // Check if the entry name matches the target
                if (strcmp(target, entry->d_name) == 0) {
                    if (checkIfTodo(target) == 0) {
                        printf(" %s%s%s\n", underline, entry->d_name, reset);
                    } else {
                        printf(" %s%s%s ->\n", underline, entry->d_name, reset);
                    }
                } else {
                    printf(" %s\n", entry->d_name);
                }
            }

            (*pCount)++;
        }
    }

    if (*pCount == 0) {
        printf("This directory is empty. Enter 'h'/'?' to display possible commands.\n");
    }

    printf("\n");

    closedir(dir);
}

void freeSelectedDir(char **selectedDir, const int count) {
    for (int i = 0; i < count; i++) {
        free(selectedDir[i]);
    }
    free(selectedDir);
}

void printCommands() {
    printf("** COMMANDS **\n");
    printf("\t'h'/'?' - Display possible commands\n");
    printf("\t'i'/'Arrow Right' - Navigate into directory/todo\n");
    printf("\t'o'/'Arrow Left' - Navigate out of directory/todo\n");
    printf("\t'c' - Create new directory/todo\n");
    printf("\t'd' - Delete selected directory/todo\n");
    printf("\t'e' - Edit selected directory/todo\n");
    printf("\t'q' - Quit\n");
}
