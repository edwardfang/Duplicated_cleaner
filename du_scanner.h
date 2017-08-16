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
#include "data_structure.h"

#define _GNU_SOURCE

#define __USE_GNU

#include <search.h>

#define MIN_BLOCK_COMPARE_SIZE 500
#define BLOCK_SIZE 100

extern long num_du_files;
extern int verbose_mod;
extern int debug_mod;
extern char *rootpath;
extern int rootpathlen;

int main(int argc, char *argv[]);

extern int verbose_msg(const char *fmt, ...);

extern int debug_msg(const char *fmt, ...);

extern int iterate_dir(char *dirpath);

extern int compare(const char *filepath);

unsigned char *getMD5(const char *filepath);

int check_privilege(const char *filepath);

int compare_file_blocks(list_node *file1, list_node *file2);

list_node *is_samefile_inlist(list *lst, list_node *newfile);

#endif