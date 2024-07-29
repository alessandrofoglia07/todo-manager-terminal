#include <stdio.h>

#include "../include/navigate.h"
#include "../include/print.h"
#include "../include/write.h"

#define DEFAULT_DATA_PATH "../data"

int main() {
    char command;
    char location[255] = DEFAULT_DATA_PATH;

    printf("**** TODO ****\n");

    while (command != 'q') {
        scanf(" %c", &command);
        printDirs(location);
        switch (command) {
            case 'h':
            case '?':  // help
                printCommands();
                break;
            case 'c':  // create
                cre(location);
                break;
            case 'd':  // delete
                del(location);
                break;
            case 'e':  // edit
                edit(location);
                break;
            case 'i':  // enter
                in(location);
                break;
            case 'o':  // exit
                out(location);
                break;
            case 'q':  // quit
                break;
            default:
                printf("%c is not a valid command. Enter '?' to display valid commands.\n");
                break;
        }
    }

    printf("Quitting...");

    return 0;
}
