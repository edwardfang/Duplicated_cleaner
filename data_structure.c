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

int ffp_compare (const void* fp1, const void* fp2){
    // printf("cmp\n");
    file_fingerprint * ffp1 = (file_fingerprint*)fp1;
    file_fingerprint * ffp2 = (file_fingerprint*)fp2;
    if(ffp1->filesize>ffp2->filesize){
        // printf("cmp\n");
        return 1;
    }else if(ffp1->filesize<ffp2->filesize){
        // printf("cmp\n");
        return -1;
    }else{
        // printf("cmp\n");
        return 0;
    }
}

