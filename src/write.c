#include <write.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/history.h>
#include <readline/readline.h>

#include <ANSIEscapeCodes.h>
#include <globals.h>
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

char DEFAULT_TEXT[MAX_PATH_LENGTH];

int cre() {
    const short inTodo = checkIfTodo(location) == 0;
    while (1) {
        if (inTodo) {
            printf("Do you want to create a new line? (y/N)\n");
            char c;
            scanf(" %c", &c);
            c = tolower(c);

            if (c == 'y') {
                char line[BUFFER_SIZE];
                while (1) {
                    while (getchar() != '\n')
                        ;
                    printf("Enter new line:\n");
                    if (fgets(line, sizeof(line), stdin) != NULL) {
                        const size_t len = strlen(line);
                        if (len > 0 && line[len - 1] == '\n') {
                            line[len - 1] = '\0';
                        }
                        if (strlen(line) > 0) {
                            break;
                        }
                    }
                }
                FILE *pF = fopen(location, "a");
                if (pF == NULL) {
                    return -1;
                }
                strcat(line, "\n");
                if (fputs(line, pF) == EOF) {
                    fclose(pF);
                    return -1;
                }
                strcpy(target, line);
                fclose(pF);
                return 1;
            }
            if (c == 'n' || c == 'e') {
                return 4;
            }
        } else {
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
                return 2;
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
                        return -1;
                    }
                    strcpy(target, name);
                }
                return 2;
            }
            if (c == 'e') {
                return 4;
            }

            printf("%c is not a valid option.\n", c);
        }
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

int del() {
    char c;
    const int inTodo = checkIfTodo(location) == 0;
    if (inTodo) {
        while (1) {
            printf("Do you really want to delete this line? (y/N) ");
            scanf(" %c", &c);
            c = tolower(c);

            if (c == 'y') {
                FILE *pF = fopen(location, "r");
                FILE *temp = fopen("tempfile1.txt", "w+");
                if (pF == NULL) {
                    printf("Error in opening todo at location %s. \n", location);
                    return -1;
                }
                char buf[BUFFER_SIZE];
                while (fgets(buf, BUFFER_SIZE, pF) != NULL) {
                    if (strcmp(buf, target) != 0) {
                        fputs(buf, temp);
                    }
                }
                rewind(temp);
                fclose(pF);
                pF = fopen(location, "w");
                if (pF == NULL) {
                    printf("Error in opening todo at location %s. \n", location);
                    return -1;
                }
                while ((c = fgetc(temp)) != EOF) {
                    fputc(c, pF);
                }
                fclose(pF);
                fclose(temp);
                remove("tempfile1.txt");
                newTarget = true;
                return 1;
            }
            if (c == 'n') {
                return 2;
            }

            printf("%c is not a valid option.\n", c);
        }
    } else {
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

                newTarget = true;
                return 1;
            }

            if (c == 'n') {
                return 2;
            }

            printf("%c is not a valid option.\n", c);
        }
    }
}

int ren() {
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
        target[FILENAME_MAX - 1] = '\0';
    }

    return result;
}

int prefill() {
    rl_insert_text(DEFAULT_TEXT);
    return 0;
}

int edit() {
    if (checkIfTodo(location) != 0) {
        return 1;
    }
    char newLocation[MAX_PATH_LENGTH];
    strcpy(newLocation, location);

    char *filename = strrchr(newLocation, PATH_DIVIDER);

#ifdef _WIN32
    char *c;
    while ((c = strchr(filename, '\\')) != NULL) {
        *c = '/';
    }
#endif

    system(CLEAN_COMMAND);
    printf("%s\n**** TODO ****\n%s", ANSI_CYAN, ANSI_RESET);
    printf("%s%s\t-- EDIT MODE\n%s\n", ANSI_GREEN, filename, ANSI_RESET);

    strcpy(DEFAULT_TEXT, target);
    if (DEFAULT_TEXT[strlen(DEFAULT_TEXT) - 1] == '\n') {
        DEFAULT_TEXT[strlen(DEFAULT_TEXT) - 1] = '\0';
    }

    rl_startup_hook = prefill;
    char *buf = readline(" ");
    char bufCopy[strlen(buf) + 1];

    strcpy(bufCopy, buf);

    // Remove newline character from the end if present
    buf[strcspn(buf, "\n")] = '\0';

    replaceLineInFile(location, target, buf);

    if (bufCopy[strlen(bufCopy) - 1] != '\n') {
        strcat(bufCopy, "\n");
    }
    strcpy(target, bufCopy);

    free(buf);
    return 0;
}
