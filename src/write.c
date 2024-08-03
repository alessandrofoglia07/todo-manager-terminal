#include <write.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ANSIEscapeCodes.h>
#include <pathFormats.h>
#include <utils.h>

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
            char name[FILENAME_MAX];
            printf("Enter directory name:\n");
            scanf("%259s", name);
            char newPath[MAX_PATH_LENGTH];
            snprintf(newPath, MAX_PATH_LENGTH, PATH_FORMAT, location, name);
            if (access(newPath, F_OK) == 0) {
                printf("The directory %s already exists.", name);
            } else {
                mkdir(newPath);
                strcpy(target, name);
            }
            return 1;
        }

        if (c == 't') {
            char name[FILENAME_MAX];
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
            strcat(name, ".todo");
            snprintf(newPath, MAX_PATH_LENGTH, PATH_FORMAT, location, name);
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
    char searchPath[MAX_PATH_LENGTH];
    snprintf(searchPath, MAX_PATH_LENGTH, "%s\\*", path);

    WIN32_FIND_DATA findFileData;
    const HANDLE hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error in opening directory at location %s\n", path);
        return -1;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }
        char newPath[MAX_PATH_LENGTH];
        snprintf(newPath, MAX_PATH_LENGTH, PATH_FORMAT, path, findFileData.cFileName);
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (removeDirectory(newPath) == -1) {
                return -1;
            }
        } else {
            if (DeleteFile(newPath) == 0) {
                fprintf(stderr, "DeleteFile failed (%lu)\n", GetLastError());
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (RemoveDirectory(path) == 0) {
        fprintf(stderr, "RemoveDirectory failed (%lu)\n", GetLastError());
        return -1;
    }
#else
    DIR *dir = NULL;
    struct dirent *entry;

    dir = opendir(location);

    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char newPath[MAX_PATH_LENGTH];
        snprintf(newPath, MAX_PATH_LENGTH, PATH_FORMAT, path, entry->d_name);
        if (entry->d_type == DT_DIR) {
            if (removeDirectory(newPath) == -1) {
                return -1;
            }
        } else {
            if (remove(newPath) != 0) {
                perror("remove");
            }
        }
    }

    closedir(dir);

    if (rmdir(path) != 0) {
        perror("rmdir");
        return -1;
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
            char fullPath[MAX_PATH_LENGTH];
            snprintf(fullPath, MAX_PATH_LENGTH, PATH_FORMAT, location, target);

            if (isTodo == 0) {
                if (remove(fullPath) != 0) {
                    perror("Error removing TODO");
                    return -1;
                }
            } else {
                if (removeDirectory(fullPath) != 0) {
                    perror("Error removing directory");
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

int ren(const char *location, char *target) {
    char fullPath[MAX_PATH_LENGTH];
    snprintf(fullPath, MAX_PATH_LENGTH, PATH_FORMAT, location, target);

    const int isTodo = checkIfTodo(target) == 0;

    char newName[FILENAME_MAX];

    while (1) {
        printf("Enter new name for %s %s:\n", isTodo ? "TODO" : "directory", target);
        if (fgets(newName, sizeof(newName), stdin) == NULL) {
            return -1;
        }
        newName[strcspn(newName, "\n")] = 0;

        if (strchr(newName, '.') != NULL) {
            printf("Name cannot contain dots.\n");
            continue;
        }
        break;
    }
    if (isTodo) {
        strncat(newName, ".todo", FILENAME_MAX - strlen(newName) - 1);
    }

    char newFullPath[MAX_PATH_LENGTH];
    snprintf(newFullPath, MAX_PATH_LENGTH, PATH_FORMAT, location, newName);

    const int result = rename(fullPath, newFullPath);

    if (result == 0) {
        strcpy(target, newName);
        target[MAX_PATH_LENGTH - 1] = '\0';
    }

    return result;
}

// TODO
int edit(const char *location) {
    char *filename = strrchr(location, PATH_DIVIDER);
#ifdef _WIN32
    char *c;
    while ((c = strchr(filename, '\\')) != NULL) {
        *c = '/';
    }
#endif
    if (checkIfTodo(location) != 0) {
        return 1;
    }

    FILE *pF = fopen(location, "r");
    if (pF == NULL) {
        perror("Error opening file");
        return -1;
    }

    char *buf = (char *)malloc(BUFFER_SIZE);
    if (buf == NULL) {
        perror("Memory allocation error");
        fclose(pF);
        return -1;
    }

    const size_t bytesRead = fread(buf, 1, BUFFER_SIZE - 1, pF);
    buf[bytesRead] = '\0';

    system(CLEAN_COMMAND);
    printf("%s\n**** TODO ****\n%s", ANSI_CYAN, ANSI_RESET);
    printf("%s-- EDIT MODE --\n%s%s%s\n", ANSI_GREEN, filename, buf, ANSI_RESET);

    char input[BUFFER_SIZE];

    scanf("%s%s", buf, input);

    fclose(pF);
    return 0;
}
