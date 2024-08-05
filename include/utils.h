#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN32
#include <conio.h>
#else
#include <ncurses.h>
#endif

void initDataDir();
void replaceLineInFile(const char *filename, char *oldLine, char *newLine);
int checkIfTodo(const char *str);
int switchCommand(int command);

#endif  //UTILS_H
