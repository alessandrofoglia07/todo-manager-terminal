#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#define DEFAULT_DATA_PATH ".\\data"
#define PATH_FORMAT "%s\\%s"
#define PATH_DIVIDER '\\'
#define CLEAN_COMMAND "cls"
#else
#define DEFAULT_DATA_PATH "./data"
#define PATH_FORMAT "%s/%s"
#define PATH_DIVIDER '/'
#define CLEAN_COMMAND "clear"
#endif

#define MAX_PATH_LENGTH 1024

extern char location[MAX_PATH_LENGTH];
extern char target[FILENAME_MAX];
extern bool newTarget;
extern char **selectedDir;
extern int count;

#endif  //GLOBALS_H
