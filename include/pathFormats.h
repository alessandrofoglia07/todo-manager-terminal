#ifndef PATH_FORMAT

#ifdef _WIN32
#define PATH_FORMAT "%s\\%s"
#define PATH_DIVIDER '\\'
#else
#define PATH_FORMAT "%s/%s"
#define PATH_DIVIDER '/'
#endif

#endif
