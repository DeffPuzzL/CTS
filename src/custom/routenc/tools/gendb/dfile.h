#ifndef _TOP_FILE_H
#define _TOP_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct _top_file_t {
    int    (*file_open)(struct _top_file_t *, const char *, const char *);
    void   (*file_close)(struct _top_file_t *);
    size_t (*file_read)(struct _top_file_t *, char *, size_t, size_t);
    size_t (*file_write)(struct _top_file_t *, char *, size_t, size_t);
    char*  (*file_gets)(struct _top_file_t *, char *, int);
    void   (*file_printf)(struct _top_file_t *, const char *format, ...);
    off_t  (*file_size)(struct _top_file_t *);
    int    (*file_eof)(struct _top_file_t *);
    void   (*file_reset_read)(struct _top_file_t *);
    void   (*file_reset_write)(struct _top_file_t *);

    char   used[0];
} top_file_t;

extern top_file_t *disk_file_create();
extern void disk_file_free(top_file_t *this);

#endif 
