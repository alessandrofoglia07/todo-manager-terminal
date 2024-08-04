#include <stdbool.h>
#include <stdio.h>

#include <defaultDataPath.h>
#include <print.h>
#include <utils.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <ncurses.h>
#endif

int main() {
    char location[MAX_PATH_LENGTH] = DEFAULT_DATA_PATH;

    char **selectedDir = NULL;
    int count = 0;

    bool newTarget = true;
    char target[FILENAME_MAX];

    printDir(location, target, &newTarget, &selectedDir, &count);
    int command = getch();
    switchCommand(command, location, target, &newTarget, &selectedDir, &count);

    while (true) {
        command = getch();
        printDir(location, target, &newTarget, &selectedDir, &count);
        if (switchCommand(command, location, target, &newTarget, &selectedDir, &count) == 1) {
            break;
        }
    }

    return 0;
}
