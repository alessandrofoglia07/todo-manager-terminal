#include <print.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ANSIEscapeCodes.h>
#include <defaultDataPath.h>
#include <utils.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

void printLine(const char *line, char *target, bool *pNewTarget, char ***pSelectedDir, int *pCount, int *pSize) {
    if (*pCount >= *pSize) {
        *pSize *= 2;
        *pSelectedDir = (char **)realloc(*pSelectedDir, *pSize * sizeof(char *));
    }

    (*pSelectedDir)[*pCount] = (char *)malloc((strlen(line) + 1) * sizeof(char));
    strcpy((*pSelectedDir)[*pCount], line);

    if (*pNewTarget) {
        strcpy(target, line);
        printf(" - %s%s%s", ANSI_UNDERLINE, line, ANSI_RESET);
        *pNewTarget = false;
    } else {
        if (strcmp(target, line) == 0) {
            printf(" - %s%s%s", ANSI_UNDERLINE, line, ANSI_RESET);
        } else {
            printf(" - %s", line);
        }
    }

    (*pCount)++;
}

void printFileName(const char *entryName, char *target, bool *pNewTarget, char ***pSelectedDir, int *pCount, int *pSize) {
    // Exclude "." and ".."
    if (!(strcmp(entryName, ".") == 0 || strcmp(entryName, "..") == 0)) {
        if (*pCount >= *pSize) {
            *pSize *= 2;
            *pSelectedDir = (char **)realloc(*pSelectedDir, *pSize * sizeof(char *));
        }

        (*pSelectedDir)[*pCount] = (char *)malloc((strlen(entryName) + 1) * sizeof(char));
        strcpy((*pSelectedDir)[*pCount], entryName);

        if (*pNewTarget) {
            strcpy(target, entryName);
            if (checkIfTodo(target) == 0) {
                printf(" %s%s%s\n", ANSI_UNDERLINE, entryName, ANSI_RESET);
            } else {
                printf(" %s%s/%s ->\n", ANSI_UNDERLINE, entryName, ANSI_RESET);
            }
            *pNewTarget = false;
        } else {
            if (strcmp(target, entryName) == 0) {
                if (checkIfTodo(target) == 0) {
                    printf(" %s%s%s\n", ANSI_UNDERLINE, entryName, ANSI_RESET);
                } else {
                    printf(" %s%s/%s ->\n", ANSI_UNDERLINE, entryName, ANSI_RESET);
                }
            } else {
                if (checkIfTodo(entryName) == 0) {
                    printf(" %s\n", entryName);
                } else {
                    printf(" %s/\n", entryName);
                }
            }
        }

        (*pCount)++;
    }
}

void printDir(const char *location, char *target, bool *pNewTarget, char ***pSelectedDir, int *pCount) {

    system(CLEAN_COMMAND);

    printf("%s\n**** TODO ****\n%s", ANSI_CYAN, ANSI_RESET);

    char locationToPrint[MAX_PATH_LENGTH];
    strcpy(locationToPrint, location + strlen(DEFAULT_DATA_PATH));

#ifdef _WIN32
    // if in windows, replace all "\" (windows default filepath divider) with "/" (more intuitive)
    char *c;
    while ((c = strchr(locationToPrint, '\\')) != NULL) {
        *c = '/';
    }
#endif

    printf(" %s%s%s\n", ANSI_GREEN, locationToPrint, ANSI_RESET);

    int size = 10;
    *pSelectedDir = (char **)malloc(size * sizeof(char *));
    *pCount = 0;

    if (checkIfTodo(location) == 0) {
        FILE *pF = fopen(location, "r");
        if (pF == NULL) {
            printf("Error in opening todo at location %s. \n", location);
            return;
        }
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, pF) != NULL) {
            printLine(buffer, target, pNewTarget, pSelectedDir, pCount, &size);
        }
        fclose(pF);
        printf("\n\n");
        return;
    }

#ifdef _WIN32
    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*", location);  // Add wildcard for searching

    WIN32_FIND_DATA findFileData;
    const HANDLE hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error in opening directory at location %s.\n", location);
        return;
    }

    do {
        printFileName(findFileData.cFileName, target, pNewTarget, pSelectedDir, pCount, &size);
    } while (FindNextFile(hFind, &findFileData) != 0);
#else
    DIR *dir = NULL;
    struct dirent *entry;

    dir = opendir(location);

    if (dir == NULL) {
        printf("Error in opening directory at location %s.\n", location);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        printFileName(entry->d_name, target, pNewTarget, pSelectedDir, pCount, &size);
    }

    closedir(dir);
#endif

    if (*pCount == 0) {
        printf("This directory is empty. Enter 'h'/'?' to display possible commands.\n");
    }

    printf("\n");
}

void freeSelectedDir(char **selectedDir, const int count) {
    for (int i = 0; i < count; i++) {
        free(selectedDir[i]);
    }
    free(selectedDir);
}

void printCommands() {
    printf("** COMMANDS **\n");
    printf("  'h'/'?' - Display possible commands\n");
    printf("  'i'/'Arrow Right' - Navigate into directory/todo\n");
    printf("  'o'/'Arrow Left' - Navigate out of directory/todo\n");
    printf("  'c' - Create new directory/todo\n");
    printf("  'd' - Delete selected directory/todo\n");
    printf("  'r' - Rename selected directory/todo\n");
    printf("  'e' - Edit todo line\n");
    printf("  'q' - Quit\n");
}
