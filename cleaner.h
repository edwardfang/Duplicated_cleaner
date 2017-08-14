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

extern int compare(const char * filepath);

unsigned char* getMD5(const char *filepath);

// filefingerprint
// typedef struct{
//     char* filename;
//     off_t st_size;
//     mode_t st_mode;
// } file_fingerprint;

// struct filenode {
//     //struct
// };