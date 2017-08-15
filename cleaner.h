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
#include <errno.h>

#endif

#define MIN_BLOCK_COMPARE_SIZE 150 
#define BLOCK_SIZE 50

extern int verbose_mod;
extern int debug_mod;
extern char* rootpath;
extern int rootpathlen;

typedef struct{
    char* filename;
    off_t filesize;
    mode_t st_mode;
    unsigned char * md5;
} file_fingerprint;

file_fingerprint* tree_root;

int main(int argc, char* argv[]);

extern int verbose_msg(const char * fmt, ...);

extern int debug_msg(const char * fmt, ...);

extern int iterate_dir(char* dirpath);

extern int compare(const char * filepath);

unsigned char* getMD5(const char *filepath);

int  check_privilege(const char* filepath);

// struct filenode {
//     //struct
// };