#ifndef UTILS_H
#define UTILS_H
#ifdef _WIN32
#define CLEAN_COMMAND "cls"
#include <conio.h>
#else
#define CLEAN_COMMAND "clear"
#include <ncurses.h>
#endif

#include <stdbool.h>

int checkIfTodo(const char *str);

int switchCommand(int command, char *location, char *target, bool *pNewTarget, char ***pSelectedDir, int *pCount);

#endif  //UTILS_H
