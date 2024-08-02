#include <utils.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <navigate.h>
#include <print.h>
#include <write.h>

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
        if (ren(location, target) == 0) {
            printf("File renamed successfully.");
        }
        printDir(location, target, pNewTarget, pSelectedDir, pCount);

    } else if (command == 'e') {
        edit(location);
    } else if (command == 'i') {
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
    } else {
        printf("%c is not a valid command. Enter '?' to display valid commands.\n", command);
    }
    return 0;
}
