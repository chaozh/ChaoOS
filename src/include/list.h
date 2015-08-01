#ifndef LIST_H
#define LIST_H

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
 * Insert a new entry between two known consecutive entries. 
 *
 * This is only for internal qlist manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head * new,
                                   struct list_head * prev,
                                   struct list_head * next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}


/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev,
                                   struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
 */
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry); 
}

/**
 * list_empty - tests whether a qlist is empty
 * @head: the list to test.
 */
static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

/**
 * list_pop - pop the first item off the list and return it
 * @head: qlist to modify
 */
static inline struct list_head* list_pop(struct list_head *head)
{
    struct list_head *item = NULL;

    if (!list_empty(head))
    {
        item = head->next;
        list_del(item);
    }

    return item;
}

/**
 * list_splice - join two qlists
 * @list: the new list to add.
 * @head: the place to add it in the first qlist.
 */
static inline void list_splice(struct list_head *list, struct list_head *head)
{
    struct list_head *first = list->next;

    if (first != list) {
        struct list_head *last = list->prev;
        struct list_head *at = head->next;

        first->prev = head;
        head->next = first;

        last->next = at;
        at->prev = last;
    }
}

static inline void list_splice_init(struct list_head *list, struct list_head *head){
	if(!list_empty(list)){
		list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}


/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)((&((type *)0)->member))))

/**
 * list_for_each	-	iterate over a qlist
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_safe - iterate over a list safe against 
 *     removal of list entry
 * @pos:  the &struct list_head to use as a loop counter.
 * @n:    another &struct list_head to use as temporary storage
 * @head: the head for your list.
 */
#define list_for_each_safe(pos, scratch, head) \
    for (pos = (head)->next, scratch = pos->next; pos != (head);\
         pos = scratch, scratch = pos->next)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)				\
    for (pos = list_entry((head)->next, typeof(*pos), member);	\
         &pos->member != (head); 					\
         pos = list_entry(pos->member.next, typeof(*pos), member))	\

/**
 * qlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
    for (pos = list_entry((head)->next, typeof(*pos), member),	\
         n = list_entry(pos->member.next, typeof(*pos), member);	\
         &pos->member != (head); 					\
         pos = n, n = list_entry(n->member.next, typeof(*n), member))

static inline int list_exists(struct list_head *list, struct list_head *qlink)
{
    struct list_head *pos;

    if(list_empty(list)) return 0;

    list_for_each(pos, list)
    {
        if(pos == qlink)
        {
            return 1;
        }
    }
    return 0;
}

static inline int list_count(struct list_head *list)
{
    struct list_head *pos;
    int count = 0;

    pos = list->next;

    while(pos != list)
    {
        ++count;
        pos = pos->next;
    }

    return count;
}

static inline struct list_head * list_find(
    struct list_head *list,
    int (*compare)(struct list_head *, void *),
    void *ptr)
{
    struct list_head *pos;
    list_for_each(pos, list)
    {
        if(compare(pos, ptr))
        {
            return pos;
        }
    }
    return NULL;
}


#endif /* QUICKLIST_H */
