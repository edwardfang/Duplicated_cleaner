#include "cleaner.h"


int compare(const char * filepath){
    struct stat buffer;
    int status;
    status = lstat(filepath, &buffer);
    debug_msg("Siza: %d, status=%d", (int )buffer.st_size, status);
    return 0;
}