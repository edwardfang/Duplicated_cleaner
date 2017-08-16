#ifndef DATA_STRUCTURE_HEADER
#define DATA_STRUCTURE_HEADER

//#include <sys/queue.h>

typedef struct node
{
    off_t filesize;
    const char *filepath;
    unsigned char *md5;
    struct node *next;
} list_node;

extern void *tree_root;
/**
file_fingerprint *ffpnew(const char *filepath, off_t filesize, mode_t filetype);

void ffpfree(void *ffp);

int ffp_compare(const void *ffp1, const void *ffp2);
**/

typedef struct
{
    off_t filesize;
    unsigned int filetype;
    int init;
    list_node *head;
} list;

list_node *node_new(const char *filepath, off_t filesize);
list *list_new(off_t filesize, unsigned int filetype);
void list_free(void *ls_p);
int list_additem(list *ls, list_node *item);
int list_compare(const void *ls1, const void *ls2);
void node_free(list_node *nd);
#endif