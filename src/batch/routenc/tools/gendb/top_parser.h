#ifndef _TOP_DB_PARSER_H
#define _TOP_DB_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#ifdef REDHAT_LINUX
#include <strings.h>
#endif

#include "dfile.h"

#define MAX_NAME_LEN 8192       /* max table,field,index name len */
#define MAX_INDEX_FIELD_NUM 20 /* max filed number in a index */
#define MAX_TOKEN_LEN 256      /* max token length */
#define MAX_NUMBER_PRESION_LEN 2 /* the max length of the number or decimal presion */
#define MAX_NUMBER_SCALE_LEN   2 /* the max length of the number or decimal scale */
#define MAX_CHAR_LENGTH        4  /* the max length of the char length 9999 */
#define MAX_LINE_LENGTH        4096 /* the max length of the line */

#define C_CHAR_C      0    /* c char  type */
#define C_SHORT_C     1    /* c short type */
#define C_INT_C       2    /* c int   type */
#define C_DOUBLE_C    3    /* c double type */
#define C_FLOAT_C     4    /* c float type */

#define S_CHAR_C      0    /* sql char type */
#define S_VARCHAR2_C  1    /* sql varchar2 type */
#define S_VARCHAR_C   2    /* sql varchar type */
#define S_SMALLINT_C  3    /* sql small int type */
#define S_INTEGER_C   4    /* sql integer type */
#define S_INT_C       5    /* sql int type */
#define S_DOUBLE_C    6    /* sql double type */
#define S_FLOAT_C     7    /* sql float type */
#define S_REAL_C      8    /* sql real type */
#define S_DECIMAL_C   9    /* sql decimal type */
#define S_NUMBER_C    10   /* sql number type */
#define S_DATE_C      11   /* sql date type */
#define S_TIMESTAMP_C 12   /* sql timestamp type */

typedef struct _top_parser_buf_t {    /* Input buffer */
     top_file_t   *file;              /* Input Source */
     top_file_t   *file_ipath;        /* Input ipath header */
     top_file_t   *file_map;          /* Input ipath map cfg */
     top_file_t   *file_header;       /* Output Header */
     top_file_t   *file_esql;         /* Output embed sql */
     top_file_t   *file_struct;       /* Output exec sql */
     top_file_t   *file_move;         /* Output move ipc */
     char         *content;           /* The buffer content */
     unsigned int size;               /* The content total size */
     char         *cur;               /* Current char being parsed */
     char         *end;               /* end of the array to parse */
     int          line;               /* Current line */
     int          col;                /* Current column */
     int          prev_line;          /* Prev line */
     int          prev_col;           /* Prev col  */
} top_parser_buf_t;

typedef struct _field_desc_t {
    char field_name  [MAX_NAME_LEN + 1]; /* field name */
    int  field_name_len;                 /* field name len */
    int  field_len;                      /* field len */
    int  sql_type;                       /* field sql type */
    int  header_type;                    /* field c type */
    char comment[MAX_NAME_LEN + 1]; 
    int  comment_len;
    struct _field_desc_t *next;          /* next field pointer */
} field_desc_t;

typedef struct _ipath_key_value {
    char   key_name   [1024+1];
    int    key_name_len;
    char   value      [1024+1];
    int    value_len;
    struct _ipath_key_value *next;
} ipath_key_value_t;

typedef struct _table_index_t {
    char index_name [MAX_NAME_LEN + 1];  /* index name */
    int  index_name_len;                 /* index name len */
    field_desc_t *ptIndexField[MAX_INDEX_FIELD_NUM]; /* pointer to index field */
    int  nIndexField; /* index field pos */
    int  nUniqueFlag; /* 0: non-unique, 1: unique, 2: primary key */
    struct _table_index_t *next;         /* next index pointer */
} table_index_t;

typedef struct {
    char table_name [MAX_NAME_LEN + 1];  /* table name */
    int  table_name_len;                 /* table name len */
    field_desc_t    *pt_field_first;     /* pointer to the frist field */
    field_desc_t    *pt_field_last;      /* pointer to the last field */
    table_index_t   *pt_index_first;     /* pointer to first table index desc */
    table_index_t   *pt_index_last;      /* pointer to last table index desc */
} table_desc_t;

typedef struct _cfg_field_t {
    int           field_type;
    field_desc_t* pt_field;
    struct _cfg_field_t *next;
    struct _cfg_field_t *nest;
    int    opr_type;
    int    con_type;
    int    value_type;
    char   *const_value;
} cfg_field_t;

typedef struct _cfg_statment_t {
    int type;
    unsigned int flag;
    int idx;
    cfg_field_t *opr_field_list;
    cfg_field_t *sort_field_list;
    cfg_field_t *con_field_list;
	cfg_field_t *group_field_list;
    struct _cfg_statment_t *next;
} cfg_statment_t;

typedef struct _table_statment_t {
    cfg_statment_t  *pt_select_st;
    cfg_statment_t  *pt_lock_st;
    cfg_statment_t  *pt_delete_st;
    cfg_statment_t  *pt_update_st;
	cfg_statment_t  *pt_stat_st;
    cfg_statment_t  *pt_cursor_st;
    cfg_statment_t  *pt_count_st;
    cfg_statment_t  *pt_sum_st;
    cfg_statment_t  *pt_max_st;
    cfg_statment_t  *pt_min_st;
} table_statment_t;

extern field_desc_t* top_parser_find_col(top_parser_buf_t *this, table_desc_t *ptTable, char *col_name, int col_name_len);
extern table_index_t* top_parser_find_index(top_parser_buf_t *this, table_desc_t *ptTable, char *index_name, int index_name_len);
extern table_index_t* top_parser_find_primary_key(top_parser_buf_t *this, table_desc_t *ptTable);
extern int top_cfg_generate_sequence(char *seq_name, int seq_name_len);
extern int top_parser_cfg_skip_blank(top_parser_buf_t* this);
extern int top_parser_buf_create(top_parser_buf_t* this, top_file_t* file,
        unsigned int size, top_file_t *file_header,
        top_file_t *file_esql);
extern int top_parser_sql_struct_field(table_desc_t *ptTable, top_file_t* pFile);
extern int top_parser_sql_file(top_parser_buf_t *this);
extern void top_parser_buf_free(top_parser_buf_t* this);
extern int top_parser_skip_blank(top_parser_buf_t* this);
extern int top_parser_cfg(table_desc_t *ptTable);
extern void StrToLower(char *str, int len);
extern void StrToUpper(char *str, int len);

#endif
