#include "cleaner.h"

file_fingerprint* ffpnew(char* subpath,off_t filesize, mode_t filetype){
    file_fingerprint* ffp = (file_fingerprint *)malloc(sizeof(file_fingerprint));
    ffp->subpath = subpath;
    ffp->filetype = filetype;
    ffp->filesize = filesize;
    return ffp;
}

void ffpfree(file_fingerprint* ffp){
    free(ffp->subpath);
    if(ffp->md5){
        free(ffp->md5);
    }
}

int ffp_compare (const file_fingerprint* ffp1, const file_fingerprint* ffp2){
    if(ffp1->filesize>ffp2->filesize){
        return 1;
    }else if(ffp1->filesize<ffp2->filesize){
        return -1;
    }else{
        return 0;
    }
}

