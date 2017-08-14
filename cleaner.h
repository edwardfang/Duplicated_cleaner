#ifndef CLEANER_H
#define CLEANER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#endif

extern int verbose_mod;
extern int debug_mod;

int main(int argc, char* argv[]);

extern int verbose_msg(const char* msg);

extern int debug_msg(const char* msg);

extern int iterate_dir(char* dirpath);

// no use
typedef struct{
    char* filename;
    size_t filesize;
    
}file_fingerprint;