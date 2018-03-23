#include <sys/stat.h>
#include <errno.h>
#include "dfile.h"

typedef struct {
    FILE *fp;
    unsigned long rptr;
    unsigned long wptr;
} t_disk_file;

int disk_file_open(top_file_t* this, const char * path, const char * mode)
{
    t_disk_file *pdisk = (t_disk_file*)this->used;

    pdisk->fp = fopen(path, mode);
    return (pdisk->fp ? 0 : -1);
}

void disk_file_close(top_file_t* this)
{
    t_disk_file *pdisk = (t_disk_file*)this->used;

    if (pdisk->fp) {
        fclose(pdisk->fp);
        pdisk->fp = NULL;
    }
}

size_t disk_file_read(top_file_t* this, char *ptr, size_t size, size_t nmemb) 
{
    size_t r = 0;
    t_disk_file *pdisk = (t_disk_file*)this->used;

    if (pdisk->fp) {
        fseek(pdisk->fp, pdisk->rptr, SEEK_SET);
        r = fread(ptr, size, nmemb, pdisk->fp);
        pdisk->rptr = ftell(pdisk->fp);
    }
    return r;
}

size_t disk_file_write(top_file_t* this, char *ptr, size_t size, size_t nmemb)
{
    size_t r = 0;
    t_disk_file *pdisk = (t_disk_file*)this->used;

    if (pdisk->fp) {
        fseek(pdisk->fp, pdisk->wptr, SEEK_SET);
        r = fwrite(ptr, size, nmemb, pdisk->fp);
        pdisk->wptr = ftell(pdisk->fp);
    }

    return r;
}

char *disk_file_gets(top_file_t* this, char *s, int size)
{
    char *r = NULL;
    t_disk_file *pdisk = (t_disk_file*)this->used;

    if (pdisk->fp) {
        fseek(pdisk->fp, pdisk->rptr, SEEK_SET);
        r = fgets(s, size, pdisk->fp);
        pdisk->rptr = ftell(pdisk->fp);
    }

    return r;
}

void disk_file_printf(top_file_t* this, const char *format, ...)
{
    t_disk_file *pdisk = (t_disk_file*)this->used;
    va_list ap;
  
    if (pdisk->fp == NULL)
        return ;

    va_start(ap, format);
    fseek(pdisk->fp, pdisk->wptr, SEEK_SET);
    vfprintf(pdisk->fp, format, ap);
    pdisk->wptr = ftell(pdisk->fp);
    va_end(ap);
}

off_t disk_file_size(top_file_t* this)
{
    struct stat st;
    int    file_no;
    t_disk_file *pdisk = (t_disk_file*)this->used;

    if (pdisk->fp == NULL)
        return 0;

    file_no = fileno(pdisk->fp);
    if (fstat(file_no, &st) == -1)
        return 0;

    return st.st_size;
}

int disk_file_eof(top_file_t* this)
{
    t_disk_file *pdisk = (t_disk_file*)this->used;

    if (pdisk->fp) {
        if (feof(pdisk->fp))
            return 1;
    }

    return 0;
}

void disk_file_reset_read(top_file_t* this)
{
    t_disk_file *pdisk = (t_disk_file*)this->used;

    pdisk->rptr = 0;
}


void disk_file_reset_write(top_file_t* this)
{
    t_disk_file *pdisk = (t_disk_file*)this->used;

    pdisk->wptr = 0;
} 

top_file_t *disk_file_create()
{
    top_file_t * r = (top_file_t*)calloc(1, sizeof(top_file_t) + sizeof(t_disk_file));

    if (r == NULL)
        return r;

    r->file_open   = disk_file_open;
    r->file_close  = disk_file_close;
    r->file_read   = disk_file_read;
    r->file_write  = disk_file_write;
    r->file_gets   = disk_file_gets;
    r->file_printf = disk_file_printf;
    r->file_size   = disk_file_size;
    r->file_eof    = disk_file_eof;
    r->file_reset_read = disk_file_reset_read;
    r->file_reset_write = disk_file_reset_write;
   
    return r;
}

void disk_file_free(top_file_t *this)
{
    free(this);
}
      
