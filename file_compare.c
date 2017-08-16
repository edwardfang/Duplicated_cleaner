#include "cleaner.h"
#include <openssl/md5.h>
#include <fcntl.h> //file control options

#define MD5_DIGEST_LENGTH 16

typedef unsigned char uchar;

char *pathtrim(const char *filepath)
{
    char *subpath = (char *)malloc(sizeof(char) * (strlen(filepath) - rootpathlen + 1)); // rootpath + subpath + '\0'
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
    list *ls_search = list_new(buffer.st_size, (unsigned int)buffer.st_mode & S_IFMT);
    debug_msg("Size: %d, type: %u,  status=%d", (int)ls_search->filesize, ls_search->filetype, status);
    // store the file info into the node
    list_node *newfile = node_new(filepath, buffer.st_size);
    // reference man page: http://man7.org/linux/man-pages/man3/tsearch.3.html
    // key points to the item to be searched for. rootp points to a variable which points to the root of the tree.
    list **lsp = (list **)tsearch((void *)ls_search, &tree_root, list_compare);
    // returns a pointer to the newly added item.
    list_node *finded_same; //0 true 1 false
    if (lsp == NULL)
    {
        debug_msg("Append child failed");
        exit(1);
    }
    else
    {
        list *rls = *lsp;
        debug_msg("Listinfo Size: %d, type: %d", rls->filesize, rls->filetype);
        if (rls != ls_search)
        {
            // list exists
            debug_msg("A list already exists! Search the item.");
            // wheather the same file is in the list
            finded_same = is_samefile_inlist(rls, newfile);
            debug_msg("is in %d", finded_same->filepath);
            // add the item to the existing list
            if (finded_same != NULL)
            {
                free(newfile);
                char *tmp_subpath = pathtrim(finded_same->filepath);
                printf("%s\t%s\n", tmp_subpath, subpath);
                free(tmp_subpath);
            }
            else
            {
                list_additem(rls, newfile);
            }
            free(ls_search);
        }
        else
        {
            // list not exist
            list_additem(rls, newfile);
            debug_msg("Created a new list!");
        }
    }
    free(subpath);
    return 0;
}

list_node *is_samefile_inlist(list *lst, list_node *newfile)
{
    list_node *nd = lst->head;
    while (1)
    {
        if (compare_file_blocks(nd, newfile) == 0)
        {
            verbose_msg("MD5 Checking");
            if (nd->md5 == NULL)
            {
                nd->md5 = getMD5(nd->filepath);
            }
            newfile->md5 = getMD5(newfile->filepath);
            if (memcmp(newfile->md5, newfile->md5, MD5_DIGEST_LENGTH) == 0)
            {
                return nd;
            }
        }
        if (nd->next == NULL)
        {
            break;
        }
        else
        {
            nd = nd->next;
        }
    }
    return NULL;
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

            fread(&tmp1, 1, 1, f1);
            fread(&tmp2, 1, 1, f2);
            if (tmp1 != tmp2)
            {
                debug_msg("block diff 1");
                return 1;
            }
        }
        // scan the end
        fseek(f1, file_size - BLOCK_SIZE, SEEK_SET);
        fseek(f2, file_size - BLOCK_SIZE, SEEK_SET);
        for (i = 0; i < BLOCK_SIZE; i++)
        {
            fread(&tmp1, 1, 1, f1);
            fread(&tmp2, 1, 1, f2);
            if (tmp1 != tmp2)
            {
                debug_msg("block diff 2");
                return 1;
            }
        }
        // randomly scan the middle
        long offset = (rand() % (file_size - 2 * BLOCK_SIZE)) + 100;
        fseek(f1, offset, SEEK_SET);
        fseek(f2, offset, SEEK_SET);
        for (i = 0; i < BLOCK_SIZE; i++)
        {
            fread(&tmp1, 1, 1, f1);
            fread(&tmp2, 1, 1, f2);
            if (tmp1 != tmp2)
            {
                debug_msg("block diff 3");
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
    uchar *out = (uchar *)malloc(sizeof(char) * (MD5_DIGEST_LENGTH));

    MD5_Init(&c);
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
    if (debug_mod)
    {
        fprintf(stderr, "[DEBUG]");
        for (n = 0; n < MD5_DIGEST_LENGTH; n++)
            fprintf(stderr, "%02x", out[n]);
        fprintf(stderr, "\n");
    }

    return out;
}