#include "cleaner.h"

verbose_mod = 0;
debug_mod = 0;

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
    int opt, argnum = 0; // argv[0], thus default 1
    while (optind < argc) {
        if ((opt = getopt(argc, argv, "vd")) != -1)
        {
            switch (opt)
            {
            case 'v':
                verbose_mod = 1;
                break;
            case 'd':
                debug_mod = 1;
                break;
            default: /* '?' */
                fprintf(stderr, "Invalid option exists.\n");
                fprintf(stderr, usage,	argv[0]);
                exit(EXIT_FAILURE);
            }
        }else{
            //debug_msg("one more arg.\n");
            argnum ++;
            optind ++;
        }
    }
    //fprintf(stderr,"%d num\n", argnum);
    // check argument nums
    if(argnum>1){
        fprintf(stderr, "You must specify one and only one directory for scanning!\n");
        fprintf(stderr, usage,	argv[0]);
        exit(EXIT_FAILURE);
    }

    char* scanrootdir = argv[--optind];

    // validate the directory
    DIR *dir;
    if ((dir = opendir (scanrootdir)) == NULL) {
        perror ("Cannot open the dir");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    // begin to find duplicated files
    iterate_dir(scanrootdir);

    return 0;

}

int verbose_msg(const char * msg){
    if(verbose_mod){
        fputs(msg,stderr);
        return 0;
    }else{
        return 1;
    }
    
}

int debug_msg(const char * msg){
    if(debug_mod){
        fputs(msg,stderr);
        return 0;
    }else{
        return 1;
    }
}