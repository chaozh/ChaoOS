#ifndef TTY_H
#define TTY_H

#define QBUFSIZ  1024 // size of a line buffer, and the limit of each line input.

/*
 * A cycle queue.
 * */
struct qbuf {
    uint            q_start;
    uint            q_end;
    uint            q_count;
    char            q_char[QBUFSIZ];
};

/*
 * */
struct tty {
    int     t_flag;
    uint    t_pgrp;
    int     t_dev;  // device number.
    int     t_col;  // position x of the cursor
    int     t_row;  // position y of the cursor
    void    (*t_putc)(char);
    struct qbuf    t_rawq;
    struct qbuf    t_canq;
    struct qbuf    t_outq;
};

//extern struct tty tty[NTTY];

/* flags */
#define TTY_CANON   0x0    /* if not raw, it's a canon */
#define TTY_RAW     0x1
#define TTY_ECHO    0x2
#define TTY_WANTED  0x10

/* control characters */
#define CINTR    28  /* ctrl-C */
#define CQUIT   034  /* ctrl-\ */
#define CERASE  '\b' /* DEL, BS */
#define CKILL   025  /* ctrl-X */
#define CEOF    004  /* ctrl-D */
#define CEOL    00
#define CSTART  021 /* ctrl-Q */
#define CSTOP   023 /* ctrl-S */
#define CSUSP   032 /* ctrl-Z */

/* ------------------------------------ */

int sigsend_g(int pgrp, int n, int priv);

#endif

