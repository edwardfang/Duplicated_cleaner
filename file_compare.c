#include "cleaner.h"
#include <openssl/md5.h>
#include <fcntl.h> //file control options


#define MD5_DIGEST_LENGTH 16

typedef unsigned char uchar;

void *tree_root = NULL;

char *pathtrim(const char *filepath)
{
    char *subpath = (char *)malloc(sizeof(char) * (strlen(filepath) - rootpathlen)); // rootpath + subpath + '\0'
    strcpy(subpath, filepath + rootpathlen);
    debug_msg("Relative Path: %s", subpath);
    return subpath;
}

int compare(const char *filepath)
{
    char *subpath = pathtrim(filepath);
    verbose_msg("Comparing File: %s", filepath);
    // ignore if no read permission
    if (check_privilege(filepath))
    {
        verbose_msg("Ignore file: %s due to privilege limitation: %s", subpath, strerror(errno));
        return -1;
    }
    debug_msg("Successfully access.");
    struct stat buffer;
    int status;
    status = lstat(filepath, &buffer);
    if (status != 0)
    {
        verbose_msg("Fetch file status failed: $s: %s, Error code: %d", subpath, strerror(errno), errno);
    }
    // find the linked list with the same file size and the same file type
    // st_mode & S_IFMT extract the file type code from a mode value.
    list* ls = listheadnew(buffer.st_size, (unsigned int)buffer.st_mode & S_IFMT);
    debug_msg("Size: %d, type: %u,  status=%d", (int)ls->filesize, ls->filetype, status);
    // store the file info into the node
    //file_fingerprint *ffp;
    
    // reference man page: http://man7.org/linux/man-pages/man3/tsearch.3.html
    // key points to the item to be searched for. rootp points to a variable which points to the root of the tree.
    list ** lsp = (list **)tsearch((void *)ls, &tree_root, list_compare);
    // returns a pointer to the newly added item.
    if (lsp == NULL)
    {
        debug_msg("Append child failed");
        exit(1);
    }
    else
    {
        list * rls = * lsp;
        debug_msg("Listinfo Size: %d, type: %d", rls->filesize, rls->filetype);
        if(rls != ls){
            debug_msg("A list already exists!");
            // add the item to the existing list
        }
        // if(fin == ffp) {
        //     //Not found
        //     debug_msg("Same file not found currently");
        //     return 1;
        // }
    }
    /**
    // test block comparision
    if(compare_file_blocks(fin, ffp) != 0) {
        //Different file, return
        return 1;
    }
    // test MD5
    char path1[rootpathlen+strlen(ffp->filepath)+1];
    //printf("%s\n", rootpath);
    strcpy(path1, rootpath);
    //printf("seek\n");
    strcat(path1, ffp->filepath);
    ffp -> md5 = getMD5(path1);
    char path2[rootpathlen+strlen((*fin)->filepath)+1];
    strcpy(path2, rootpath);
    strcat(path2, (*fin)->filepath);
    (*fin) -> md5 = getMD5(path2);
    for(int i=0;i<MD5_DIGEST_LENGTH;i++) {
        if(ffp->md5[i] != fin->md5[i])
            return 1;
    }
    
    //Got the same file, output them
    printf("Duplicate file found:\n%s\n%s\n", ffp->filepath, fin->filepath);
    //uchar * md5 = getMD5(filepath);
    //debug_msg("MD5: %x", md5);
    //free(md5);
    // maintain the info in a binary tree
    **/
    free(subpath);
    return 0;
}

// ignore the files that can not be read; test privilege
int check_privilege(const char *filepath)
{
    debug_msg("Checking privileges.");
    FILE *fd;
    if ((fd = fopen(filepath, "rb")) == NULL)
    {
        // debug_msg("Return 1 Checking privilege.");
        return 1;
    }
    else
    {
        // debug_msg("Return 0 Checking privilege.");
        fclose(fd);
        return 0;
    }
}

// the third comparision
// 0 - same file
// 1 - different file
int compare_file_blocks(list_node *file1, list_node *file2)
{
    unsigned int file_size = (unsigned int)file1->filesize;
    char *subpath[2];
    if (verbose_mod)
    {
        subpath[0] = pathtrim(file1->filepath);
        subpath[1] = pathtrim(file2->filepath);
        verbose_msg("Block Checking: File name: %s\t%s", subpath[0], subpath[1]);
    }
    FILE *f1, *f2;
    char tmp1, tmp2;
    int i;
    f1 = fopen(file1->filepath, "rb");
    f2 = fopen(file2->filepath, "rb");
    if (file_size > MIN_BLOCK_COMPARE_SIZE)
    {
        // scan the beginning
        for (i = 0; i < BLOCK_SIZE; i++)
        {

            fread(&tmp2, 1, 1, f1);
            fread(&tmp2, 1, 1, f2);
            if (tmp1 != tmp2)
            {
                return 1;
            }
        }
        // scan the end
        fseek(f1, file_size - BLOCK_SIZE, SEEK_SET);
        fseek(f2, file_size - BLOCK_SIZE, SEEK_SET);
        for (i = 0; i < BLOCK_SIZE; i++)
        {
            fread(&tmp2, 1, 1, f1);
            fread(&tmp2, 1, 1, f2);
            if (tmp1 != tmp2)
            {
                return 1;
            }
        }
        // randomly scan the middle
        long offset = (rand() % (file_size - 2 * BLOCK_SIZE)) + 100;
        fseek(f1, offset, SEEK_SET);
        fseek(f2, offset, SEEK_SET);
        for (i = 0; i < BLOCK_SIZE; i++)
        {
            fread(&tmp2, 1, 1, f1);
            fread(&tmp2, 1, 1, f2);
            if (tmp1 != tmp2)
            {
                return 1;
            }
        }
    }
    else
    {
        debug_msg("File too small, check the whole");
        for (i = 0; i < file_size; i++)
        {
            fread(&tmp1, 1, 1, f1);
            fread(&tmp2, 1, 1, f2);
            if (tmp1 != tmp2)
            {
                return 1;
            }
        }
    }
    fclose(f1);
    fclose(f2);
    debug_msg("Block Checking finished");
    return 0;
}

// the final comparision
uchar *getMD5(const char *filepath)
{
    int n;
    MD5_CTX c;
    char buf[512];
    ssize_t bytes;
    uchar *out = (uchar *)malloc(sizeof(char) * (MD5_DIGEST_LENGTH + 1));

    MD5_Init(&c);
    //debug_msg("MD5_Init");
    int fd;                        // FILE* fd;
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