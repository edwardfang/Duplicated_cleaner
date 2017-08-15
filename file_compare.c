#include "cleaner.h"
#include <openssl/md5.h>
#include <fcntl.h> //file control options


int compare(const char *filepath)
{
    if(check_privilege(filepath)){
        verbose_msg("Ignore file: filename due to privilege limitation: %s", strerror(errno));
        //return -1;
    }
    struct stat buffer;
    int status;
    status = lstat(filepath, &buffer);
    debug_msg("Siza: %d, status=%d", (int)buffer.st_size, status);
    // test MD5
    getMD5(filepath);
    // maintain the info in a binary tree
    return 0;
}

// ignore the files that can not be read; test privilege
int check_privilege(const char* filepath){
    FILE* fd;
    if((fd = fopen(filepath, "rb"))==NULL){
        return 1;
    }else{
        fclose(fd);
        return 0;
    }
}


// the third comparision
int compare_file_blocks(file_fingerprint* file1,file_fingerprint file2 ){
    off_t file_size = file1->filesize;
    if(file_size>MIN_BLOCK_COMPARE_SIZE){
        return 0;
    }else{
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
    unsigned char *out = (unsigned char *)malloc(sizeof(char) * MD5_DIGEST_LENGTH);

    MD5_Init(&c);
    debug_msg("MD5_Init");
    int fd; // FILE* fd;
    fd = open(filepath, O_RDONLY); // or fd = fopen(filepath, "rb"); 
    bytes = read(fd, buf, 512);
    debug_msg("MD5_read");
    while (bytes > 0)
    {

        MD5_Update(&c, buf, bytes);
        debug_msg("MD5_update");
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