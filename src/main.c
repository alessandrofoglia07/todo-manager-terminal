#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define CLEAN_COMMAND "cls"
#include <conio.h>
#else
#define CLEAN_COMMAND "clear"
#include <ncurses.h>
#endif

#include "../include/print.h"
#include "../include/utils.h"

#define DEFAULT_DATA_PATH "./data"

int main() {
    char location[255] = DEFAULT_DATA_PATH;

    char **selectedDir = NULL;
    int count = 0;

    bool newTarget = true;
    char target[260];  // 260 is the default max length for a filename

    system(CLEAN_COMMAND);
    printf("%s\n**** TODO ****\n%s", "\x1b[34m", "\033[0m");
    printDir(location, target, &newTarget, &selectedDir, &count);
    printf("\n");
    int command = getch();
    if (switchCommand(command, location, target, &newTarget, &selectedDir, &count) == 1) {
        return 0;
    }

    while (true) {
        command = getch();
        system(CLEAN_COMMAND);
        printf("%s\n**** TODO ****\n%s", "\x1b[34m", "\033[0m");
        printDir(location, target, &newTarget, &selectedDir, &count);
        printf("\n");
        if (switchCommand(command, location, target, &newTarget, &selectedDir, &count) == 1) {
            break;
        }
    }

    return 0;
}
