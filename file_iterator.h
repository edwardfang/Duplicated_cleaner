#ifndef FILE_ITERATOR_HEAD
#define FILE_ITERATOR_HEAD

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif


int iterate_dir(DIR * directory);

// no use
typedef struct{
    char* filename;
    size_t filesize;
    
}file_fingerprint;