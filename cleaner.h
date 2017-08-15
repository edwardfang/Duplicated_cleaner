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

#define MIN_BLOCK_COMPARE_SIZE 500 
#define BLOCK_SIZE 100

extern int verbose_mod;
extern int debug_mod;
extern char* rootpath;
extern int rootpathlen;

typedef struct{
    const char* filepath;
    off_t filesize;
    mode_t filetype;
    unsigned char * md5;
} file_fingerprint;
extern void* tree_root;


int main(int argc, char* argv[]);

extern int verbose_msg(const char * fmt, ...);

extern int debug_msg(const char * fmt, ...);

extern int iterate_dir(char* dirpath);

extern int compare(const char * filepath);

unsigned char* getMD5(const char *filepath);

int  check_privilege(const char* filepath);

int compare_file_blocks(file_fingerprint* file1,file_fingerprint *file2 );

file_fingerprint* ffpnew(const char* filepath,off_t filesize, mode_t filetype);

void ffpfree(file_fingerprint* ffp);

int ffp_compare (const void* ffp1, const void* ffp2);   

#endif