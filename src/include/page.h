#ifndef PAGE_H
#define PAGE_H

struct page {
    uchar flags;
    ushort count; //atomic
    ushort number;
    struct page *pg_next; //for free list
    struct {
        void* private;
    };
};



#endif
