#ifndef PAGE_H
#define PAGE_H
#include <list.h>

struct page {
    uchar flags;
    ushort count; //atomic
    ushort number;
    struct list_head free_list;
    struct {
        void* private;
    };
};



#endif
