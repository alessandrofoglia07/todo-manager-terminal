#ifndef WRITE_H
#define WRITE_H
#include <stdbool.h>

int cre(const char *location, char *target);
int del(const char *location, const char *target, bool *pNewTarget);
void edit(char *location);

#endif