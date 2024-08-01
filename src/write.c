#include <write.h>

#include <ANSIEscapeCodes.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

#define MAX_PATH_LENGTH 1024

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define F_OK 0
#define access _access
#else
#include <dirent.h>
#include <unistd.h>
#endif

int cre(const char *location, char *target) {
    while (1) {
        printf("Do you want to create a %sD%sirectory or a %sT%sodo? (enter 'e' to exit)\n", ANSI_UNDERLINE, ANSI_RESET, ANSI_UNDERLINE, ANSI_RESET);
        char c;
        scanf(" %c", &c);
        c = tolower(c);

        if (c == 'd') {
            char name[260];
            printf("Enter directory name:\n");
            scanf("%259s", name);
            char newPath[MAX_PATH_LENGTH];
            strcpy(newPath, location);
            strcat(newPath, "/");
            strcat(newPath, name);
            if (access(newPath, F_OK) == 0) {
                printf("The directory %s already exists.", name);
            } else {
                mkdir(newPath);
                strcpy(target, name);
            }
            return 1;
        }

        if (c == 't') {
            char name[260];
            while (1) {
                printf("Enter TODO name:\n");
                scanf("%259s", name);
                if (strchr(name, '.') != NULL) {
                    printf("Name cannot contain dots.\n");
                    continue;
                }
                break;
            }
            char newPath[MAX_PATH_LENGTH];
            strcpy(newPath, location);
            strcat(newPath, "/");
            strcat(name, ".todo");
            strcat(newPath, name);
            if (access(newPath, F_OK) == 0) {
                printf("The TODO %s already exists.", name);
            } else {
                const FILE *pF = NULL;
                pF = fopen(newPath, "w");
                if (pF == NULL) {
                    printf("Error occurred while creating the TODO.");
                    free(newPath);
                    return -1;
                }
                strcpy(target, name);
            }
            return 2;
        }

        if (c == 'e') {
            return 3;
        }

        printf("%c is not a valid option.\n", c);
    }
}

int removeDirectory(const char *path) {
#ifdef _WIN32
    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*", path);

    WIN32_FIND_DATA findFileData;
    const HANDLE hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error in opening directory at location %s.\n", path);
        return -1;
    }

    do {
        char newPath[MAX_PATH_LENGTH];
        snprintf(newPath, sizeof(newPath), "%s\\%s", path, findFileData.cFileName);
        if (checkIfTodo(findFileData.cFileName) == 0) {
            remove(newPath);
        } else {
            if (removeDirectory(newPath) == -1) {
                return -1;
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
#else
    DIR *dir = NULL;
    struct dirent = *entry;

    dir = opendir(location);

    if (dir == NULL) {
        printf("Error in opening directory at location %s.\n", path);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char newPath[MAX_PATH_LENGTH];
        snprintf(newPath, sizeof(newPath), "%s/%s", location, entry->d_name);
        if (checkIfTodo(findFileData.cFileName) == 0) {
            remove(newPath);
        } else {
            if (removeDirectory(newPath) == -1) {
                return -1;
            }
        }
    }
#endif
    return 0;
}

int del(const char *location, const char *target, bool *pNewTarget) {
    char c;
    const int isTodo = checkIfTodo(target);
    while (1) {
        if (isTodo == 0) {
            printf("\n%s\nDo you really want to delete this TODO? (y/N) ", target);
        } else {
            printf("\n%s\nDo you really want to delete this directory? (y/N) ", target);
        }

        scanf(" %c", &c);
        c = tolower(c);

        if (c == 'y') {
            char fullPath[strlen(location) + strlen(target) + 1];
            strcpy(fullPath, location);
            strcat(fullPath, "/");
            strcat(fullPath, target);

            if (isTodo == 0) {
                if (remove(fullPath) != 0) {
                    perror("Error removing TODO.");
                    return -1;
                }
            } else {
                if (removeDirectory(fullPath) != 0) {
                    perror("Error removing directory.");
                    return -1;
                }
            }

            *pNewTarget = true;
            return 1;
        }

        if (c == 'n') {
            return 2;
        }

        printf("%c is not a valid option.\n", c);
    }
}

void edit(char *location) {
}
