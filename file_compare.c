#include "cleaner.h"
#include <openssl/md5.h>
#include <fcntl.h> //file control options
#include <search.h>

#define MD5_DIGEST_LENGTH  16


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
    file_fingerprint* ffp;
    ffp = ffpnew(subpath, buffer.st_size, buffer.st_mode);
    debug_msg("Siza: %d, type: %d,  status=%d", (int)ffp->filesize, (int)ffp->filetype, status);
    // !!! not finished yet
    if(tsearch(ffp,tree_root,&ffp_compare)!=NULL){
        debug_msg("Tsearch not null");
    }
    // test block comparision
    //compare_file_blocks("./build/cleaner.o", "./build/file_iterator.o");
    // test MD5
    //unsigned char * md5 = getMD5(filepath);
    //free(md5);
    // maintain the info in a binary tree
    return 0;
}

// ignore the files that can not be read; test privilege
int check_privilege(const char* filepath){
    debug_msg("Checking privilege.");
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
int compare_file_blocks(file_fingerprint* file1,file_fingerprint file2 ){
    unsigned int file_size = (unsigned int) file1->filesize;
    debug_msg("file size: %u", file_size);
    FILE* fd[2];
    if(file_size>MIN_BLOCK_COMPARE_SIZE){
        return 0;
    }else{
        char * tmp1 = (char *)malloc(sizeof(char)* file_size);
        char * tmp2 = (char *)malloc(sizeof(char)* file_size);
        //fread(tmp1,(size_t) file_size,)
        return 0;
    }

}


// the final comparision
unsigned char *getMD5(const char *filepath)
{
    int n;
    MD5_CTX c;
    char buf[512];
    ssize_t bytes;
    unsigned char *out = (unsigned char *)malloc(sizeof(char) * (MD5_DIGEST_LENGTH+1));

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