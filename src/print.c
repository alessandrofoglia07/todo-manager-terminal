#include <print.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ANSIEscapeCodes.h>
#include <globals.h>
#include <utils.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

void printLine(const char *line, int *pSize) {
    if (count >= *pSize) {
        *pSize *= 2;
        selectedDir = (char **)realloc(selectedDir, *pSize * sizeof(char *));
    }

    selectedDir[count] = (char *)malloc((strlen(line) + 1) * sizeof(char));
    strcpy(selectedDir[count], line);

    if (newTarget) {
        strcpy(target, line);
        printf(" - %s%s%s", ANSI_UNDERLINE, line, ANSI_RESET);
        newTarget = false;
    } else {
        if (strcmp(target, line) == 0) {
            printf(" - %s%s%s", ANSI_UNDERLINE, line, ANSI_RESET);
        } else {
            printf(" - %s", line);
        }
    }
}

void printFileName(const char *entryName, int *pSize) {
    // Exclude "." and ".."
    if (!(strcmp(entryName, ".") == 0 || strcmp(entryName, "..") == 0)) {
        if (count >= *pSize) {
            *pSize *= 2;
            selectedDir = (char **)realloc(selectedDir, *pSize * sizeof(char *));
        }

        selectedDir[count] = (char *)malloc((strlen(entryName) + 1) * sizeof(char));
        strcpy(selectedDir[count], entryName);

        if (newTarget) {
            strcpy(target, entryName);
            if (checkIfTodo(target) == 0) {
                printf(" %s%s%s\n", ANSI_UNDERLINE, entryName, ANSI_RESET);
            } else {
                printf(" %s%s/%s ->\n", ANSI_UNDERLINE, entryName, ANSI_RESET);
            }
            newTarget = false;
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

        count++;
    }
}

void printDir() {
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
    selectedDir = (char **)malloc(size * sizeof(char *));
    count = 0;

    if (checkIfTodo(location) == 0) {
        FILE *pF = fopen(location, "r");
        if (pF == NULL) {
            printf(" Error in opening todo at location %s. \n", location);
            return;
        }
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, pF) != NULL) {
            printLine(buffer, &size);
        }
        fclose(pF);
        if (count == 0) {
            printf(" This TODO is empty. Enter 'h'/'?' to display avaliable commands.\n");
        }
        printf("\n\n");
        return;
    }

#ifdef _WIN32
    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*", location);  // Add wildcard for searching

    WIN32_FIND_DATA findFileData;
    const HANDLE hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf(" Error in opening directory at location %s.\n", location);
        return;
    }

    do {
        printFileName(findFileData.cFileName, &size);
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
        printFileName(entry->d_name, &size);
    }

    closedir(dir);
#endif

    if (count == 0) {
        if (strcmp(DEFAULT_DATA_PATH, location)) {
            printf(" Welcome to the main directory. Enter 'h'/'?' to display avaliable commands.\n");
        } else {
            printf(" This directory is empty. Enter 'h'/'?' to display avaliable commands.\n");
        }
    }

    printf("\n");
}

void printCommands() {
    const int isTodo = checkIfTodo(location) == 0;
    printf("** COMMANDS **\n");
    printf("  'h'/'?' - Display available commands\n");
    if (isTodo) {
        printf("  'o'/'Arrow Left' - Navigate out of todo\n");
        printf("  'c' - Create new line\n");
        printf("  'e' - Edit selected line\n");
        printf("  'd' - Delete selected line\n");
    } else {
        printf("  'i'/'Arrow Right' - Navigate into directory/todo\n");
        if (strcmp(location, DEFAULT_DATA_PATH) != 0) {
            printf("  'o'/'Arrow Left' - Navigate out of directory\n");
        }
        printf("  'c' - Create new directory/todo\n");
        printf("  'r' - Rename selected directory/todo\n");
        printf("  'd' - Delete selected directory/todo\n");
    }
    printf("  'q' - Quit\n");
}
