#include <utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <globals.h>
#include <navigate.h>
#include <print.h>
#include <write.h>

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

void initDataDir() {
    const int dataDirExists = access(DEFAULT_DATA_PATH, F_OK) == 0;
    if (dataDirExists) {
        return;
    }
    mkdir(DEFAULT_DATA_PATH);
}

void replaceLineInFile(const char *filename, char *oldLine, char *newLine) {

    oldLine[strcspn(oldLine, "\n")] = '\0';

    if (newLine[0] == '\n') {
        newLine++;
    }
    newLine[strcspn(newLine, "\n")] = 0;

    FILE *pF = fopen(filename, "r");
    if (pF == NULL) {
        perror("Error opening file");
        return;
    }

    FILE *tmpFile = NULL;

#ifdef _WIN32
    const char tempFilename[] = "tempfile.txt";
    tmpFile = fopen(tempFilename, "w+");
#else
    tmpFile = tmpfile();
#endif

    if (tmpFile == NULL) {
        perror("Error creating temporary file");
        fclose(pF);
        return;
    }

    char buf[BUFFER_SIZE];

    while (fgets(buf, BUFFER_SIZE, pF) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';

        if (strcmp(buf, oldLine) == 0) {
            fprintf(tmpFile, "%s\n", newLine);
        } else {
            fprintf(tmpFile, "%s\n", buf);
        }
    }

    fclose(pF);

    pF = fopen(filename, "w");
    if (pF == NULL) {
        perror("Error opening file for writing");
        fclose(tmpFile);
        return;
    }

    rewind(tmpFile);

    int c;
    while ((c = fgetc(tmpFile)) != EOF) {
        fputc(c, pF);
    }

    fclose(tmpFile);
    fclose(pF);
#ifdef _WIN32
    DeleteFile(tempFilename);
#endif
}

/**
 * @return 0 if str ends in ".todo", 1 otherwise
*/
int checkIfTodo(const char *str) {
    const char *todoSuffix = ".todo";
    return strncmp(str + strlen(str) - strlen(todoSuffix), todoSuffix, strlen(todoSuffix));
}

int switchCommand(int command) {
    // special key detect
    if (command == 0 || command == 224) {
        command = getch();
        switch (command) {
            case 72:
                handleArrowUp();
                printDir();
                break;
            case 80:
                handleArrowDown();
                printDir();
                break;
            case 75:  // left arrow
                out();
                printDir();
                break;
            case 77:  // right arrow
                if (checkIfTodo(location) == 0) {
                    break;
                }
                char temp[sizeof(location) + sizeof(char) + sizeof(target)];
                strcpy(temp, location);
                in();
                if (strcmp(temp, location) == 0)
                    break;
                printDir();
                break;
        }
    } else if (command == 'h' || command == '?') {
        printCommands();
    } else if (command == 'c') {
        const int result = cre();
        if (result == 1) {
            printDir();
            printf("New line has been successfully added.");
        } else if (result == 2) {
            printDir();
            printf("The directory has been successfully created.");
        } else if (result == 3) {
            printDir();
            printf("The TODO has been successfully created.");
        } else if (result == 4) {
            printDir();
        }
    } else if (command == 'd') {
        const int result = del();
        if (result == 1) {
            printDir();
            printf("Deletion successful.");
        } else if (result == 2) {
            printDir();
        }
    } else if (command == 'r') {
        if (checkIfTodo(location) == 0) {
            return 0;
        }
        if (ren() == 0) {
            printDir();
            printf("File renamed successfully.");
            return 0;
        }
        printDir();
    } else if (command == 'e') {
        const int result = edit();
        if (result == 0) {
            printDir();
            printf("TODO edited successfully.");
        } else if (result == 1) {
            printDir();
            printf("Cannot use this command while in a directory.");
        }
    } else if (command == 'i') {
        if (checkIfTodo(location) == 0) {
            return 0;
        }
        char temp[strlen(location) + strlen(target) + 1];
        strcpy(temp, location);
        in();
        if (strcmp(temp, location) != 0) {
            printDir();
        }
    } else if (command == 'o') {
        out();
        printDir();
    } else if (command == 'q') {
        printf("Quitting...");
        for (int i = 0; i < count; i++) {
            free(selectedDir[i]);
        }
        free(selectedDir);
        return 1;
    };
    return 0;
}
