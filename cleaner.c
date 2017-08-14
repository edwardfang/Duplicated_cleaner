#include "cleaner.h"

int main(int argc, char* argv[]){
    /**
    ** main entry
    **/

    // the information to display when options goes wrong
    char * usage = "Usage: %s dirname [-v] [-d]\n"
    "where\n"
    "\t -v is used to displays more information about what the program is doing\n"
    "\t -d is used to displays the information for debugging\n";

    // get option info
    int opt, argnum = 1; // argv[0], thus default 1
    while ((opt = getopt(argc, argv, "vd")) != -1)
	{
		switch (opt)
		{
		case 'v':
            verbose_mod = 1;
            argnum++;
			break;
		case 'd':
            debug_mod = 1;
            argnum++;
			break;
        default: /* '?' */
            fprintf(stderr, "Invalid option exists.\n");
			fprintf(stderr, usage,	argv[0]);
			exit(EXIT_FAILURE);
		}
    }

    // check argument nums
    if(argc-argnum !=1){
        fprintf(stderr, "You must specify one and only one directory for scanning!\n");
        fprintf(stderr, usage,	argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;

}