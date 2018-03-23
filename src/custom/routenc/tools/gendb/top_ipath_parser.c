#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include "dfile.h"
#include "top_parser.h"

static char  gs_token[MAX_TOKEN_LEN]; /* current token      */
static int   gn_token;                /* current token len  */
static int   gn_token_type;           /* current token type */
static int   gn_token_quote = 0;

static enum {STATE_INIT = 0, STATE_DEFINE, STATE_DEFINE_KEY, 
             STATE_DEFINE_VALUE, STATE_DEFINE_VALUE_END, STATE_END} state = STATE_INIT;

#define CUR (*this->cur)
#define NEXT top_parser_ipath_next_char(this)
#define SKIP_BLANKS top_parser_ipath_skip_blank(this)
#define NEXT_LINE top_parser_ipath_next_line(this)
#define CUR_PTR     (this->cur)
#define SET_CUR(p)  (this->cur = p)
#define IS_BLANK_CH(ch) (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
#define IS_TOKEN_CH(ch) (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' \
        || ch == '\\' || ch == '"' || ch == '/')
#define SAVE_LINE_COL do { \
    this->prev_line = this->line, this->prev_col = this->col; \
} while (0)


#define SKIP(val) do { \
    this->cur += (val), this->col += (val); \
} while (0)
#define NEXT_TOKEN top_parser_ipath_next_token(this)

static top_parser_buf_t   tIpathParser;
static top_file_t        *tIpathFile;
static ipath_key_value_t *ptFirstElm=NULL;

/* 解析ipath头文件 */
int top_parser_ipath_file(top_parser_buf_t *ptParserBuf);

static void top_parser_ipath_buf_free(top_parser_buf_t* this)
{
    if (this->content != NULL)
        free(this->content);
}

top_parser_buf_t *top_parser_get_ipath(void) {
    return &tIpathParser;
}

ipath_key_value_t* top_parser_find_value(char *col_name, int col_name_len)
{       
    ipath_key_value_t *ptField;

    ptField = ptFirstElm;
    while (ptField != NULL) {
        if (col_name_len == ptField->value_len &&
                !strncasecmp(col_name, ptField->value, col_name_len))
            break;

        ptField = ptField->next;
    }   

    if (ptField == NULL) {
        return NULL;
    }   

    return ptField;
}

static int top_parser_ipath_buf_create(top_parser_buf_t* this, top_file_t* file,
        unsigned int size, top_file_t *file_cfile)
{       
    this->file_ipath = file;
    this->file_move  = file_cfile;

    if (size == 0) { /* count the input file size */
        this->size = file->file_size(file);
        if (this->size == 0) {
            fprintf(stderr, "get input file size error %d\n", errno);
            return -1;
        }
    } else  
        this->size = size;

    this->content = calloc(1, this->size * sizeof(char));
    if (NULL == this->content) {
        fprintf(stderr, "calloc %d error %d\n", this->size, errno);
        return -1;
    }

    this->prev_line = this->prev_col = 0;
    this->line = this->col = 1;
    this->cur = this->end = this->content;

    return 0;
}

static int top_parser_ipath_buf_grow(top_parser_buf_t* this)
{
    unsigned int left_size = (int)(this->end - this->cur);
    int rc;

    memmove(this->content, this->cur, left_size);
    this->cur = this->content;
    this->end = this->cur + left_size;

    rc = this->file_ipath->file_read(this->file_ipath, this->end,
            sizeof(char),
            this->size - left_size);
    this->end += rc;

    memset(this->end, 0, this->size - left_size - rc);
    return (left_size + rc);
}

static int top_parser_ipath_chk_buf(top_parser_buf_t* this, unsigned int len)
{
    unsigned int left_size = (int)(this->end - this->cur);
    unsigned int size;
    int rc;

    if (len > this->size)
        return -1;

    if (left_size >= len)
        return 0;

    rc = top_parser_ipath_buf_grow(this);
    if (rc < len)
        return -1;

    return 0;
}

static void top_parser_ipath_next_char(top_parser_buf_t* this)
{
    if (CUR == '\n') { /* a new line */
        this->line++;
        this->col = 1;
    } else
        this->col++;

    this->cur++;
    top_parser_ipath_chk_buf(this, 1);

    return;
}

static int top_parser_ipath_skip_blank(top_parser_buf_t* this)
{
    int res = 0;
    int cur;
    int rc;

    rc = top_parser_ipath_chk_buf(this, 1);
    if (rc)
        return res;

    cur = CUR;
    while (IS_BLANK_CH(cur)) { /* CHECKED BLANK */
        NEXT;
        cur = CUR;
        res++;
    }

    return (res);
}

static void top_parser_ipath_next_line(top_parser_buf_t *this)
{
    int cur;

    SAVE_LINE_COL;

    cur = CUR;
    while (cur != '\n' && cur != '\0') {
        NEXT;
        cur = CUR;
    }
    if (cur == '\n')
        NEXT;
}

#define TOKEN_COMMENT        1
#define TOKEN_IPATH_VALUE    2
#define TOKEN_IPATH_CONTINUE 3
#define TOKEN_STRING         4
static void top_parser_ipath_next_token(top_parser_buf_t* this)
{
    int cur, i, next_flag = 1;

    SKIP_BLANKS;
    SAVE_LINE_COL;

    cur = CUR;
    i = 0;
    gn_token_type = TOKEN_STRING;
    while (!IS_TOKEN_CH(cur) && cur != '\0' && i < MAX_TOKEN_LEN) {
        gs_token[i++] = cur;
        NEXT;
        cur = CUR;
    }

    if (i == 0 && IS_TOKEN_CH(cur)) {
        switch (cur) {
            case '/':
                NEXT;
                cur = CUR;
                if(cur == '*')
                    gn_token_type = TOKEN_COMMENT;
                else if(gn_token_quote) {
                    gn_token_type = TOKEN_STRING;
                    gs_token[i++] = '/';
                    gs_token[i++] = cur;
                    NEXT;
                    cur = CUR;
                    while(1) {
                        if(cur == '"')
                        {
                            gn_token_quote = 0;
                            break;
                        }
                        gs_token[i++] = cur;
                        NEXT;
                        cur = CUR;
                    }
                }
                break;
            case '"':
                gn_token_type = TOKEN_IPATH_VALUE;
                gn_token_quote = 1;
                NEXT;
                break;
            case '\\':
                gn_token_type = TOKEN_IPATH_CONTINUE;
                NEXT;
                break;
        }
    }
    gn_token = i;
}

/* 加载IPATH头文件 */
int load_ipath_header(void)
{
    int               iRet = -1;
    char              path[256];
    int               rc;

    tIpathFile = disk_file_create();
    if (tIpathFile == NULL) {
        printf("disk_file_create error: %d\n", errno);
        return -1;
    }

    snprintf(path, sizeof(path), "%s/include/cust/ipath.h", getenv("HOME"));
    rc = tIpathFile->file_open(tIpathFile, path, "r");
    if (rc) {
        //printf("file_open [%s] error: %d\n", path, errno);
        disk_file_free(tIpathFile);
        return rc;
    }

    memset(&tIpathParser, 0x00, sizeof(tIpathParser));
    rc = top_parser_ipath_buf_create(&tIpathParser, tIpathFile, 0, NULL);
    if (rc) {
        printf("top_parser_ipath_buf_create error: %d\n", rc);
        tIpathFile->file_close(tIpathFile);
        disk_file_free(tIpathFile);
        return rc;
    }

    rc = top_parser_ipath_file(&tIpathParser);
    if (rc) {
        printf("top_parser_ipath_file error: %d\n", rc);
        tIpathFile->file_close(tIpathFile);
        disk_file_free(tIpathFile);
        top_parser_ipath_buf_free(&tIpathParser);
        return rc;
    }

    return 0;
}

static void top_skip_comment(top_parser_buf_t *this)
{
    int cur;

    SAVE_LINE_COL;

    cur = CUR;
    while(1) {
        while (cur != '*') {
            NEXT;
            cur = CUR;
        }
        if (cur == '*') {
            NEXT;
            cur = CUR;
            if(cur == '/') {
                NEXT;
                cur = CUR;
                break;
            }
        }
    }
}

/* 解析ipath头文件 */
int top_parser_ipath_file(top_parser_buf_t *this)
{
    int  quit = 0;
    char sKey  [1024+1];
    char sValue[1024+1];
    ipath_key_value_t *ptCurElm=NULL;
    ipath_key_value_t *ptTmpElm=NULL;

    SKIP_BLANKS;
    while(!quit) {
        switch (state) {
            case STATE_INIT:
                SKIP_BLANKS;
                if (CUR == '\0')
                    state = STATE_END;
                else
                    state = STATE_DEFINE;
                break;
            case STATE_DEFINE:
                NEXT_TOKEN;
                while(gn_token_type == TOKEN_COMMENT) {
                    top_skip_comment(this);
                    NEXT_TOKEN;
                }
                if(gn_token == 7 && !strncasecmp(gs_token, "#define", gn_token)) {
                    state = STATE_DEFINE_KEY;
                    break;
                }
                if(gn_token == 7 && !strncasecmp(gs_token, "#ifndef", gn_token)) {
                    NEXT_TOKEN;
                    state = STATE_INIT;
                    break;
                }
                if(gn_token == 6 && !strncasecmp(gs_token, "#endif", gn_token)) {
                    NEXT_TOKEN;
                    state = STATE_INIT;
                    break;
                }
                return -1;
            case STATE_DEFINE_KEY:
                NEXT_TOKEN;
                while(gn_token_type == TOKEN_COMMENT) {
                    top_skip_comment(this); 
                    NEXT_TOKEN;
                }
                if(gn_token_type == TOKEN_STRING)
                {
                    memset(sKey, 0x00, sizeof(sKey));
                    memcpy(sKey, gs_token, gn_token);
                    state = STATE_DEFINE_VALUE;
                    break;
                }
                return -1;
            case STATE_DEFINE_VALUE:
                NEXT_TOKEN;
                while(gn_token_type == TOKEN_COMMENT) {
                    top_skip_comment(this);
                    NEXT_TOKEN;
                }
                if(gn_token == 7 && !strncasecmp(gs_token, "#define", gn_token)) {
                    state = STATE_DEFINE_KEY;
                    break;
                }
                if(gn_token_type == TOKEN_IPATH_VALUE) {
                    state = STATE_DEFINE_VALUE_END;
                    break;
                }
                while(gn_token_type == TOKEN_IPATH_CONTINUE) {
                    NEXT_TOKEN;
                    if(gn_token_type == TOKEN_STRING) {
                        strncat(sKey, gs_token, gn_token);
                        state = STATE_DEFINE_VALUE;
                        break;
                    }
                }
                return -1;
            case STATE_DEFINE_VALUE_END:
                NEXT_TOKEN;
                if(gn_token_type == TOKEN_STRING) {
                    memset(sValue, 0x00, sizeof(sValue));
                    memcpy(sValue, gs_token, gn_token);
                    NEXT_TOKEN;
                    while(gn_token_type == TOKEN_IPATH_CONTINUE) {
                        strncat(sValue, gs_token, gn_token);
                        NEXT_TOKEN;
                    }
                    if(gn_token_type == TOKEN_IPATH_VALUE) {
                        ptTmpElm = calloc(1, sizeof(ipath_key_value_t));
                        if(ptTmpElm ==NULL)
                            return -1;
                        ptTmpElm->key_name_len = strlen(sKey);
                        memcpy(ptTmpElm->key_name, sKey, ptTmpElm->key_name_len);
                        ptTmpElm->value_len    = strlen(sValue);
                        memcpy(ptTmpElm->value, sValue, ptTmpElm->value_len);
                        if(ptFirstElm == NULL) {
                            ptFirstElm = ptTmpElm;
                            ptCurElm   = ptFirstElm;
                        } else {
                            ptCurElm->next = ptTmpElm;
                            ptCurElm = ptCurElm->next;
                        }
                        state = STATE_INIT;
                        break;
                    }
                }
                return -1;
            case STATE_END:
                quit = 1;
                break;
        }
    }
    return 0;
}
