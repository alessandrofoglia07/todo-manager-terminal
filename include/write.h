#ifndef WRITE_H
#define WRITE_H

#include <stdbool.h>

#define MAX_PATH_LENGTH 1024
#define BUFFER_SIZE 1024

int cre(const char *location, char *target);
int del(const char *location, const char *target, bool *pNewTarget);
int ren(const char *location, char *target);
int edit(const char *location, char *target);

#endif