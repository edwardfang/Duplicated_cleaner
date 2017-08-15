#include "cleaner.h"
#include <openssl/md5.h>
#include <fcntl.h> //file control options
#include <search.h>

#define MD5_DIGEST_LENGTH  16

typedef unsigned char uchar;

file_fingerprint* tree_root = NULL;

char* pathtrim(const char* filepath){
    char * subpath = (char *) malloc(sizeof(char)*(strlen(filepath)-rootpathlen)); // rootpath + subpath + '\0'
    strcpy(subpath, filepath+rootpathlen);
    debug_msg("Relative Path: %s",subpath);
    return subpath;
}


int compare(const char *filepath)
{
    char * subpath = pathtrim(filepath);
    verbose_msg("Comparing File: %s",filepath);
    // ignore if no read permission
    if(check_privilege(filepath)){
        verbose_msg("Ignore file: %s due to privilege limitation: %s",subpath, strerror(errno));
        return -1;
    }
    debug_msg("Successfully access.");
    struct stat buffer;
    int status;
    status = lstat(filepath, &buffer);
    if(status!=0){
        verbose_msg("Fetch file status failed: $s: %s, Error code: %d",subpath, strerror(errno),errno);
    }
    file_fingerprint *ffp;
    ffp = ffpnew(subpath, buffer.st_size, buffer.st_mode);
    debug_msg("Size: %d, type: %d,  status=%d", (int)ffp->filesize, (int)ffp->filetype, status);
    // !!! not finished yet
    file_fingerprint *fin;
    if((fin = (file_fingerprint*)tsearch((void*)ffp,(void**)&tree_root,ffp_compare))==NULL){
        debug_msg("Append child failed");
        exit(1);
    } else {
        //fin = (file_fingerprint*)((uintptr_t)fin-32);
        printf("%d %d %d\n", ffp, fin, (uintptr_t)fin - (uintptr_t)ffp);
        debug_msg("Fin Size: %d, type: %d path: %s", fin->filesize, fin->filetype, fin->subpath);
        if(fin == ffp) {
            //Not found
            debug_msg("Same file not found currently");
            return 1;
        }
    }
    // test block comparision
    if(compare_file_blocks(fin, ffp) != 0) {
        //Different file, return
        return 1;
    }
    // test MD5
    char path1[rootpathlen+strlen(ffp->subpath)+1];
    //printf("%s\n", rootpath);
    strcpy(path1, rootpath);
    //printf("seek\n");
    strcat(path1, ffp->subpath);
    ffp -> md5 = getMD5(path1);
    char path2[rootpathlen+strlen(fin->subpath)+1];
    strcpy(path2, rootpath);
    strcat(path2, fin->subpath);
    fin -> md5 = getMD5(path2);
    for(int i=0;i<MD5_DIGEST_LENGTH;i++) {
        if(ffp->md5[i] != fin->md5[i])
            return 1;
    }
    
    //Got the same file, output them
    printf("Duplicate file found:\n%s\n%s\n", ffp->subpath, fin->subpath);
    //uchar * md5 = getMD5(filepath);
    //debug_msg("MD5: %x", md5);
    //free(md5);
    // maintain the info in a binary tree
    return 0;
}

// ignore the files that can not be read; test privilege
int check_privilege(const char* filepath){
    debug_msg("Checking privileges.");
    FILE* fd;
    if((fd = fopen(filepath, "rb"))==NULL){
        //debug_msg("Return 1 Checking privilege.");        
        return 1;
    }else{
        //debug_msg("Return 0 Checking privilege.");
        fclose(fd);
        return 0;
    }
}


// the third comparision
// 0 - same file
// 1 - different file
int compare_file_blocks(file_fingerprint *file1, file_fingerprint *file2 ){
    unsigned int file_size = (unsigned int) file1->filesize;
    debug_msg("file size: %d %d", file_size, file2 -> filesize);
    FILE *f1, *f2;
    if(file_size>MIN_BLOCK_COMPARE_SIZE){
        debug_msg("File too large, quit comparing block");
        return 0;
    }else{
        // char * tmp1 = (char *)malloc(sizeof(char)* file_size);
        // char * tmp2 = (char *)malloc(sizeof(char)* file_size);
        //fread(tmp1,(size_t) file_size,)
        char path1[rootpathlen+strlen(file1->subpath)+1];
        strcpy(path1, rootpath);
        strcat(path1, file1->subpath);
        f1 = fopen(path1, "rb");
        char path2[rootpathlen+strlen(file2->subpath)+1];
        strcpy(path2, rootpath);
        strcat(path2, file2->subpath);
        f2 = fopen(path2, "rb");
        long offset = rand() % (file1->filesize - MIN_BLOCK_COMPARE_SIZE);
        fseek(f1, offset, SEEK_SET);
        fseek(f1, offset, SEEK_CUR);
        int i=0;
        while(i<MIN_BLOCK_COMPARE_SIZE && fgetc(f1) == fgetc(f1)) i++;
        fclose(f1);
        fclose(f2);
        debug_msg("compare blocks finished");
        return i < MIN_BLOCK_COMPARE_SIZE;
    }

}


// the final comparision
uchar *getMD5(const char *filepath)
{
    int n;
    MD5_CTX c;
    char buf[512];
    ssize_t bytes;
    uchar *out = (uchar *)malloc(sizeof(char) * (MD5_DIGEST_LENGTH+1));

    MD5_Init(&c);
    //debug_msg("MD5_Init");
    int fd; // FILE* fd;
    fd = open(filepath, O_RDONLY); // or fd = fopen(filepath, "rb"); 
    bytes = read(fd, buf, 512);
    //debug_msg("MD5_read");
    while (bytes > 0)
    {

        MD5_Update(&c, buf, bytes);
        //debug_msg("MD5_update");
        bytes = read(fd, buf, 512);
    }

    MD5_Final(out, &c);
    debug_msg("MD5_Final");
    if (debug_mod)
    {
        fprintf(stderr, "[DEBUG]");
        for (n = 0; n < MD5_DIGEST_LENGTH; n++)
            fprintf(stderr, "%02x", out[n]);
        fprintf(stderr, "\n");
    }

    return out;
}