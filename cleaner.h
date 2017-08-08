#ifndef
#define CLEANER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif


static int verbose_mod = 0;
static int debug_mod = 0;

int main(int argc, char*[] argv);

extern int verbose_msg(char* msg);

extern int debug_msg(char* msg);