#include <utils.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <navigate.h>
#include <print.h>
#include <stdlib.h>
#include <windows.h>
#include <write.h>

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

int switchCommand(int command, char *location, char *target, bool *pNewTarget, char ***pSelectedDir, int *pCount) {
    // special key detect
    if (command == 0 || command == 224) {
        command = getch();
        switch (command) {
            case 72:
                handleArrowUp(location, target, pNewTarget, *pSelectedDir, pCount);
                break;
            case 80:
                handleArrowDown(location, target, pNewTarget, *pSelectedDir, pCount);
                break;
            case 75:  // left arrow
                out(location, target, pNewTarget);
                printDir(location, target, pNewTarget, pSelectedDir, pCount);
                break;
            case 77:  // right arrow
                if (checkIfTodo(location) == 0) {
                    break;
                }
                char temp[sizeof(location) + sizeof(char) + sizeof(target)];
                strcpy(temp, location);
                in(location, target, pNewTarget);
                if (strcmp(temp, location) == 0)
                    break;
                printDir(location, target, pNewTarget, pSelectedDir, pCount);
                break;
        }
    } else if (command == 'h' || command == '?') {
        printCommands();
    } else if (command == 'c') {
        const int result = cre(location, target);
        if (result == 1) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
            printf("The directory has been successfully created.");
        } else if (result == 2) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
            printf("The TODO has been successfully created.");
        } else if (result == 3) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
        }
    } else if (command == 'd') {
        const int result = del(location, target, pNewTarget);
        if (result == 1) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
            printf("Deletion successful.");
        } else if (result == 2) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
        }
    } else if (command == 'r') {
        if (checkIfTodo(location) == 0) {
            return 0;
        }
        if (ren(location, target) == 0) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
            printf("File renamed successfully.");
            return 0;
        }
        printDir(location, target, pNewTarget, pSelectedDir, pCount);
    } else if (command == 'e') {
        const int result = edit(location, target);
        if (result == 0) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
            printf("TODO edited successfully.");
        } else if (result == 1) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
            printf("Cannot use this command while in a directory.");
        }
    } else if (command == 'i') {
        if (checkIfTodo(location) == 0) {
            return 0;
        }
        char temp[sizeof(location) + sizeof(char) + sizeof(target)];
        strcpy(temp, location);
        in(location, target, pNewTarget);
        if (strcmp(temp, location) != 0) {
            printDir(location, target, pNewTarget, pSelectedDir, pCount);
        }
    } else if (command == 'o') {
        out(location, target, pNewTarget);
        printDir(location, target, pNewTarget, pSelectedDir, pCount);
    } else if (command == 'q') {
        printf("Quitting...");
        freeSelectedDir(*pSelectedDir, *pCount);
        return 1;
    };
    return 0;
}
