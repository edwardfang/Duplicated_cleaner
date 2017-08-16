#ifndef DATA_STRUCTURE_HEADER
#define DATA_STRUCTURE_HEADER

#include <sys/queue.h>

struct entry {
    SLIST_ENTRY(entry) links;
    // ... Any fields you like, for example:
    file_fingerprint * ffp;
};

#endif