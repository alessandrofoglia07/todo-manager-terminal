#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

int checkIfTodo(const char* str);

bool switchCommand(int command, char *location, char *target, bool *newTarget, char ***selectedDir, int *count);

#endif  //UTILS_H
