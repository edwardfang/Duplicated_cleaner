#ifndef CLEANER_H
#define CLEANER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdarg.h>

#endif

#define MAX_MSG_LEN 1000

extern int verbose_mod;
extern int debug_mod;
extern char* rootpath;
extern int rootpathlen;

int main(int argc, char* argv[]);

extern int verbose_msg(const char * fmt, ...);

extern int debug_msg(const char * fmt, ...);

extern int iterate_dir(char* dirpath);

extern int compare(char * filepath);

// no use
typedef struct{
    char* filename;
    size_t filesize;
    
}file_fingerprint;