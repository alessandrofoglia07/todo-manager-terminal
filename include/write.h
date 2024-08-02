#ifndef WRITE_H
#define WRITE_H
#include <stdbool.h>

int cre(const char *location, char *target);
int del(const char *location, const char *target, bool *pNewTarget);
int ren(const char *location, char *target);
void edit(char *location);

#endif