#include <stdbool.h>

#include <print.h>
#include <utils.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <ncurses.h>
#endif

int main() {

    initDataDir();

    printDir();
    int command = getch();
    if (switchCommand(command) == 1) {
        return 0;
    }

    while (true) {
        command = getch();
        printDir();
        if (switchCommand(command) == 1) {
            break;
        }
    }

    return 0;
}
