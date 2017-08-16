#include "cleaner.h"

list *list_new(off_t filesize, unsigned int filetype)
{
    list *ls = (list *)malloc(sizeof(list));
    ls->filesize = filesize;
    ls->filetype = filetype;
    ls->init = 0;
    return ls;
}

void list_free(void *ls_p)
{
    list *ls = (list *)ls_p;
    // delete the list here
    // debug_msg("Freeing the list for size %u, type %u", ls->filesize, ls->filetype);
    if(ls->init==0){
        free(ls);
        return;
    }
    verbose_msg("Freeshuoing the list for size %u, type %u", ls->filesize, ls->filetype);
    list_node *nd = ls->head, *nd_tmp;
    while (nd->next != NULL)
    {
        nd_tmp = nd->next;
        node_free(nd);
        nd = nd_tmp;
    }
    node_free(nd);
    // free list struct
    free(ls);
}

int list_additem(list *ls, list_node *item)
{
    if (ls->init == 0)
    {
        ls->head = item;
        ls->init = 1;
    }
    else
    {
        item->next = ls->head;
        ls->head = item;
    }
    // insert before head
    return 0;
}

// compar points to a comparison routine, which takes pointers to two items.
int list_compare(const void *ls_p1, const void *ls_p2)
{
    {
        list *ls1 = (list *)ls_p1;
        list *ls2 = (list *)ls_p2;
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

list_node *node_new(const char *filepath, off_t filesize)
{
    list_node *lst_node = (list_node *)malloc(sizeof(list_node));
    lst_node->filepath = filepath;
    lst_node->filesize = filesize;
    lst_node->next = NULL;
    return lst_node;
}

void node_free(list_node *nd)
{
    debug_msg("Freeing file_node for %s",nd->filepath);
    if (nd->filepath != NULL)
    {
        free((char *)nd->filepath);
    }
    if (nd->md5 != NULL)
    {
        verbose_msg("fault position2, md5: %d", nd->md5);
        free(nd->md5);
    }
    free(nd);
}