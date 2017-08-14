#include "cleaner.h"

int verbose_mod = 0;
int debug_mod = 0;

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

    // processing of the root path
    char * rootpath = argv[--optind];
    int rootpathlen = strlen(rootpath);
    char ch;
    while((ch = rootpath[rootpathlen-1])== '/'){
        rootpath[rootpathlen-1] = 0;
        rootpathlen--;
    }
    debug_msg(rootpath);

    // validate the directory
    DIR *dir;
    if ((dir = opendir (rootpath)) == NULL) {
        perror ("Cannot open the dir");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    // begin to find duplicated files
    iterate_dir(rootpath);

    return 0;

}

int verbose_msg(const char * fmt, ...){
    if(verbose_mod){
        int result;
        va_list args;
        va_start(args, fmt);
        fputs("[INFO]", stderr);
        result = vfprintf(stderr,fmt,args);
        fputs("\n", stderr);
        va_end(args);
        return result; //retrun the length of string
    }else{
        return -1;
    }
    
}

int debug_msg(const char * fmt, ...){
    if(debug_mod){
        int result;
        va_list args;
        va_start(args, fmt);
        fputs("[DEBUG]", stderr);
        result = vfprintf(stderr,fmt,args);
        fputs("\n", stderr);
        va_end(args);
        return result; //retrun the length of string
    }else{
        return -1;
    }
}