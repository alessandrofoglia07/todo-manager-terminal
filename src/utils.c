#include "../include/utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/navigate.h"
#include "../include/print.h"
#include "../include/write.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <ncurses.h>
#endif

/**
 * @return 0 if str is "todo", 1 otherwise
*/
int checkIfTodo(const char *str) {
    const char *todoSuffix = ".todo";
    return strncmp(str + strlen(str) - strlen(todoSuffix), todoSuffix, strlen(todoSuffix));
}

/**
 * @return 1 if "quit" is selected, otherwise 0
 */
bool switchCommand(int command, char *location, char *target, bool *newTarget, char ***selectedDir, int *count) {
    // special key detect
    if (command == 0 || command == 224) {
        command = getch();
        switch (command) {
            case 72:
                handleArrowUp(location, target, newTarget, *selectedDir, count);
                break;
            case 80:
                handleArrowDown(location, target, newTarget, *selectedDir, count);
                break;
            case 75:  // left arrow
                out(location);
                break;
            case 77:  // right arrow
                in(location, target, newTarget);
                printDir(location, target, newTarget, selectedDir, count);
                break;
        }
    } else if (command == 'h' || command == '?') {
        printCommands();
    } else if (command == 'c') {
        cre(location);
    } else if (command == 'd') {
        del(location);
    } else if (command == 'e') {
        edit(location);
    } else if (command == 'i') {
        in(location, target, newTarget);
        printDir(location, target, newTarget, selectedDir, count);
    } else if (command == 'o') {
        out(location);
    } else if (command == 'q') {
        printf("Quitting...");
        freeSelectedDir(*selectedDir, *count);
        return 1;
    } else {
        printf("%c is not a valid command. Enter '?' to display valid commands.\n", command);
    }
    return 0;
}
