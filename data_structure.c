#include "cleaner.h"

list* listheadnew(off_t filesize, unsigned int filetype){
    list *ls = (list *) malloc(sizeof(list));
    ls->filesize = filesize;
    ls->filetype = filetype;
    ls->init = 0;
    return ls;
}

void listheadfree(void *ls_p){
    list *ls = (list *) ls_p;
    // delete the list here

    // free list struct
    free(ls);
}

int list_additem(list *ls, list_node* item){
    if(ls->init == 0){
        // new node as head
    }
    // insert after head
    ls->init = 1;
    return 0;
}

// compar points to a comparison routine, which takes pointers to two items.
int list_compare(const void *ls_p1, const void *ls_p2){
    {
        list *ls1 = (list *) ls_p1;
        list *ls2 = (list *) ls_p2;
        if (ls1->filesize > ls2->filesize)
        {
            return 1;
        }
        else if (ls1->filesize < ls2->filesize)
        {
            return -1;
        }
        else
        {
            if (ls1->filetype > ls2->filetype)
            {
                return 1;
            }
            else if (ls1->filetype < ls2->filetype)
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }
    }
    
}

/**
file_fingerprint *ffpnew(const char *filepath, off_t filesize, unsigned int filetype)
{
    file_fingerprint *ffp = (file_fingerprint *)malloc(sizeof(file_fingerprint));
    ffp->filepath = filepath;
    ffp->filetype = filetype;
    ffp->filesize = filesize;
    return ffp;
}

void ffpfree(void *ffp)
{ // file_fingerprint*
    free((char *)((file_fingerprint *)ffp)->filepath);
    if (((file_fingerprint *)ffp)->md5)
    {
        free(((file_fingerprint *)ffp)->md5);
    }
    free(ffp);
}

// compar points to a comparison routine, which takes pointers to two items.
int ffp_compare(const void *ffp1, const void *ffp2)
{
    file_fingerprint *fp1 = (file_fingerprint *)ffp1;
    file_fingerprint *fp2 = (file_fingerprint *)ffp2;
    if (fp1->filesize > fp2->filesize)
    {
        return 1;
    }
    else if (fp1->filesize < fp2->filesize)
    {
        return -1;
    }
    else
    {
        if (fp1->filetype > fp2->filetype)
        {
            return 1;
        }
        else if (fp1->filetype < fp2->filetype)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
}
**/