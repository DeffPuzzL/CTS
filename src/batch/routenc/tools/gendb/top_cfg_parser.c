#include "top_parser.h"

#define FIELD_NORMAL        0
#define FIELD_LEFT_BRACE    1
#define FIELD_RIGHT_BRACE   2
#define FIELD_ROWCOUNT      3

#define TOKEN_ERROR         0
#define TOKEN_LEFT_BRACE    1 
#define TOKEN_RIGHT_BRACE   2
#define TOKEN_SEMICOLON     3
#define TOKEN_COMMA         4
#define TOKEN_AND           5
#define TOKEN_OR            6
#define TOKEN_QUOTA         7
#define TOKEN_EQUAL         8
#define TOKEN_GREAT         9
#define TOKEN_LESS          10
#define TOKEN_GREAT_EQUAL   11
#define TOKEN_LESS_EQUAL    12
#define TOKEN_NOT_EQUAL     13
#define TOKEN_ANGLE         14
#define TOKEN_STRING        15
#define TOKEN_DOUBLE_QUOTA  16
#define TOKEN_LIKE          17
#define TOKEN_NOT_LIKE      18

#define SELECT_STATMENT  1
#define LOCK_STATMENT    2
#define DELETE_STATMENT  3 
#define UPDATE_STATMENT  4
#define CURSOR_STATMENT  5
#define COUNT_STATMENT   6
#define SUM_STATMENT     7
#define MAX_STATMENT     8
#define MIN_STATMENT     9
#define STAT_STATMENT    10

#define CURSOR_ASC_SORT  0x00000001
#define CURSOR_DSC_SORT  0x00000010

#define STATMENT_OPR_EQUAL        1
#define STATMENT_OPR_GREAT        2
#define STATMENT_OPR_GREAT_EQUAL  3
#define STATMENT_OPR_LESS         4
#define STATMENT_OPR_LESS_EQUAL   5
#define STATMENT_OPR_NOT_EQUAL    6
#define STATMENT_OPR_FIELD_ADD    7
#define STATMENT_OPR_FIELD_MINUS  8
#define STATMENT_OPR_LIKE         9
#define STATMENT_OPR_NOT_LIKE     10
#define STATMENT_OPR_FIELD_SUM    11
#define STATMENT_OPR_FIELD_COUNT  12

#define STATMENT_CON_AND          1
#define STATMENT_CON_OR           2

#define CONST_VALUE               1
#define FIELD_VALUE               2 

#define CUR (*this->cur)
#define NEXT top_parser_cfg_next_char(this)
#define SKIP_BLANKS top_parser_cfg_skip_blank(this)
#define NEXT_LINE top_parser_cfg_next_line(this)
#define CUR_PTR     (this->cur)
#define SET_CUR(p)  (this->cur = p)
#define IS_BLANK_CH(ch) (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
#define IS_TOKEN_CH(ch) (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '(' \
                         || ch == ')' || ch == ';' || ch == ',' || ch == '&' || ch == '|' \
                         || ch == '\'' || ch == '=' || ch == '>' || ch == '<' || ch == '^' || ch == '"' || ch == '%' || ch == '!')
#define SKIP(val) do { \
    this->cur += (val), this->col += (val); \
  } while (0)
#define NEXT_TOKEN top_parser_cfg_next_token(this)
#define SAVE_LINE_COL do { \
    this->prev_line = this->line, this->prev_col = this->col; \
 } while (0)

static int nWorking = 1;

static enum {STATE_INIT = 0, STATE_ST_BEGIN, STATE_ST_SELECT_BEGIN, STATE_ST_SELECT_END,
             STATE_ST_LOCK_BEGIN, STATE_ST_LOCK_END,
             STATE_ST_UPDATE_BEGIN, STATE_ST_UPDATE_END, STATE_ST_DELETE_BEGIN,
             STATE_ST_DELETE_END, STATE_ST_CURSOR_BEGIN, STATE_ST_CURSOR_END,
             STATE_ST_COUNT_BEGIN, STATE_ST_STAT_BEGIN, STATE_ST_STAT_END, STATE_ST_COUNT_END,STATE_ST_SUM_BEGIN, STATE_ST_SUM_END,
             STATE_ST_MAX_BEGIN, STATE_ST_MAX_END, STATE_ST_MIN_BEGIN, STATE_ST_MIN_END,
             STATE_END} state = STATE_INIT;

static char  gs_token[MAX_TOKEN_LEN]; /* current token */
static int   gn_token;                /* current token len */
static int   gn_token_type;           /* current token type */

extern int load_map_config(char *ptTableName);
extern ipath_key_value_t* top_parser_find_key(char *col_name, int col_name_len);
extern top_parser_buf_t *top_parser_get_ipath(void);
extern ipath_key_value_t* top_parser_find_value(char *col_name, int col_name_len);

int top_parser_cfg_buf_create(top_parser_buf_t* this, top_file_t* file, unsigned int size)
{
    this->file = file;

    if (size == 0) { /* count the input file size */
        this->size = file->file_size(file);
        if (this->size == 0) {
            fprintf(stderr, "get input file size error %d\n", errno);
            return -1;
        }
    }

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

/*
 * top_parser_buf_grow:
 * @this: the parser input
 *
 * Grow up the content of the input buffer, the old data are preserved
 * Returns the number of chars read and stored in the buffer, or -1
 * in case of error.
 */
int top_parser_cfg_buf_grow(top_parser_buf_t* this)
{
    unsigned int left_size = (int)(this->end - this->cur);
    int rc;

    memmove(this->content, this->cur, left_size);
    this->cur = this->content;
    this->end = this->cur + left_size;

    rc = this->file->file_read(this->file, this->end,
                               sizeof(char),
                               this->size - left_size);
    this->end += rc;

    memset(this->end, 0, this->size - left_size - rc);
    return (left_size + rc);
}

/*
 * top_parser_chk_buf:
 * @this: the parser input
 * @len: the length must in the left buffer
 *
 * check the left length of the content in the input buffer
 * Returns 0 if the left length is enough, or -1
 * in case of error.
 */
int top_parser_cfg_chk_buf(top_parser_buf_t* this, unsigned int len)
{
    unsigned int left_size = (int)(this->end - this->cur);
    unsigned int size;
    int rc;

    if (len > this->size)
        return -1;

    if (left_size >= len)
        return 0;

    rc = top_parser_cfg_buf_grow(this);
    if (rc < len)
        return -1;

    return 0;
}

/*
 * top_parser_next_char:
 * @this: the parser input
 *
 * Skip to the next char input char.
 */
void top_parser_cfg_next_char(top_parser_buf_t* this)
{
    if (CUR == '\n') { /* a new line */
        this->line++;
        this->col = 1;
    } else
        this->col++;

    this->cur++;
    top_parser_cfg_chk_buf(this, 1);

    return;
}

void top_parser_cfg_next_line(top_parser_buf_t *this)
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

/*
 * top_parser_next_token:
 * this: the parser input
 *
 * Skip to the next input token.
 */
void top_parser_cfg_next_token(top_parser_buf_t* this)
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
        case '(':
            gn_token_type = TOKEN_LEFT_BRACE;
            break;
        case ')':
            gn_token_type = TOKEN_RIGHT_BRACE;
            break;
        case ';':
            gn_token_type = TOKEN_SEMICOLON;
            break;
        case ',':       
            gn_token_type = TOKEN_COMMA;
            break;
        case '^':
            gn_token_type = TOKEN_ANGLE;
            break;
        case '"':
            gn_token_type = TOKEN_DOUBLE_QUOTA;
            break;
        case '&':
            NEXT;
            cur = CUR;
            if (cur != '&') 
                gn_token_type = TOKEN_ERROR;
            else
                gn_token_type = TOKEN_AND;
            break;
        case '|':
            NEXT;
            cur = CUR;
            if (cur != '|')
                gn_token_type = TOKEN_ERROR;
            else
                gn_token_type = TOKEN_OR;
            break;            
        case '\'':
            gn_token_type = TOKEN_QUOTA;
            break;
        case '=':
            gn_token_type = TOKEN_EQUAL;
            break;
        case '>':
            NEXT;
            cur = CUR;
            if (cur == '=')
                gn_token_type = TOKEN_GREAT_EQUAL;
            else {
                gn_token_type = TOKEN_GREAT;
                next_flag = '0';
            }
            break;
        case '<':
            NEXT;
            cur = CUR;
            if (cur == '=')
                gn_token_type = TOKEN_LESS_EQUAL;
            else if (cur == '>') 
                gn_token_type = TOKEN_NOT_EQUAL;
            else {  
                gn_token_type = TOKEN_LESS;
                next_flag = 0;
            }
            break;
        case '%':
            gn_token_type = TOKEN_LIKE;
            break;
        case '!':
            NEXT;
            cur = CUR;
            if (cur == '%')
                 gn_token_type = TOKEN_NOT_LIKE;
            else
                gn_token_type = TOKEN_ERROR;
            break;
        }
        if (next_flag == 1) 
            NEXT;
    }
    gn_token = i;
}

/*
 * top_parser_skip_blank:
 * @this:  the parser input
 *
 * skip all blanks character found at that point in the input streams.
 *
 * Returns the number of space chars skipped
 */
int top_parser_cfg_skip_blank(top_parser_buf_t* this)
{
    int res = 0;
    int cur;
    int rc;

    rc = top_parser_cfg_chk_buf(this, 1);
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

int top_parser_free_opr_field(cfg_field_t *ptField)
{
    int rc;
    cfg_field_t *ptTmp, *ptNext;
 
    ptNext = ptField;
    while (ptNext != NULL) {
        ptTmp = ptNext;
        ptNext = ptNext->next;
        if (ptTmp->const_value != NULL && ptTmp->value_type == CONST_VALUE)
            free(ptTmp->const_value); 
        free(ptTmp);
    }

    return 0;
}

int top_parser_free_con_field(cfg_field_t *ptField)
{
    int rc;
    cfg_field_t *ptTmp, *ptNext;

    ptNext = ptField;
    while (ptNext != NULL) {
        if (ptNext->field_type == FIELD_LEFT_BRACE) {
            top_parser_free_con_field(ptNext->nest);
            ptTmp = ptNext;
            ptNext = ptNext->next;
            free(ptTmp); 
        } else {
            ptTmp = ptNext;
            ptNext = ptNext->next;
            if (ptTmp->const_value != NULL && ptTmp->value_type == CONST_VALUE)
                free(ptTmp->const_value); 
            free(ptTmp);  
        }
    }
   
    return 0;
}
 
int top_parser_free_select(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_opr_field(ptStmt->opr_field_list);
        rc = top_parser_free_con_field(ptStmt->con_field_list);    
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}

int top_parser_free_update(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_opr_field(ptStmt->opr_field_list);
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}


int top_parser_free_delete(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}


int top_parser_free_cursor(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_opr_field(ptStmt->opr_field_list);
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        rc = top_parser_free_opr_field(ptStmt->sort_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}

int top_parser_free_stat(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_opr_field(ptStmt->opr_field_list);
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        rc = top_parser_free_opr_field(ptStmt->group_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}

int top_parser_free_count(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}


int top_parser_free_sum(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        rc = top_parser_free_opr_field(ptStmt->opr_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}

int top_parser_free_max(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        rc = top_parser_free_opr_field(ptStmt->opr_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}

int top_parser_free_min(cfg_statment_t *ptStmt)
{
    int rc;
    cfg_statment_t *ptTmp;

    while (ptStmt != NULL) {
        rc = top_parser_free_con_field(ptStmt->con_field_list);
        rc = top_parser_free_opr_field(ptStmt->opr_field_list);
        ptTmp = ptStmt->next;
        free(ptStmt);
        ptStmt = ptTmp;
    }
    return 0;
}

int top_parser_cfg_free_table(table_statment_t *ptTableStmt)
{
    int rc;

    rc = top_parser_free_select(ptTableStmt->pt_select_st);
    if (rc < 0)
        return -1;

    rc = top_parser_free_delete(ptTableStmt->pt_delete_st);
    if (rc < 0)
        return -1;

    rc = top_parser_free_update(ptTableStmt->pt_update_st);
    if (rc < 0)
        return -1;

    rc = top_parser_free_cursor(ptTableStmt->pt_cursor_st);
    if (rc < 0)
        return -1;
	
	rc = top_parser_free_stat(ptTableStmt->pt_stat_st);
    if (rc < 0)
        return -1;

    rc = top_parser_free_count(ptTableStmt->pt_count_st);
    if (rc < 0)
        return -1;

    rc = top_parser_free_sum(ptTableStmt->pt_sum_st);
    if (rc < 0)
        return -1;

    rc = top_parser_free_max(ptTableStmt->pt_max_st);
    if (rc < 0)
        return -1;

    rc = top_parser_free_min(ptTableStmt->pt_min_st);
    if (rc < 0)
        return -1;

    return 0; 
}

int top_parser_cfg_pk(top_parser_buf_t* this,
                      table_desc_t *ptTable,
                      cfg_field_t  **ptFieldBegin,
                      cfg_field_t  **ptFieldEnd)
{
    int rc, i;
    table_index_t *p_index;
    cfg_field_t   *p_field = NULL, *p_tmp_field;
  
    p_index = top_parser_find_primary_key(this, ptTable);
    if (p_index == NULL)
        return -1;

    for (i = 0; i < p_index->nIndexField; ++i) {
         p_tmp_field = calloc(1, sizeof(cfg_field_t));
         p_tmp_field->field_type = FIELD_NORMAL;
         p_tmp_field->pt_field = p_index->ptIndexField[i];
         p_tmp_field->opr_type = STATMENT_OPR_EQUAL;
         p_tmp_field->con_type = STATMENT_CON_AND;
         if (p_field == NULL) { 
             p_field = p_tmp_field;
             *ptFieldBegin = p_field;
         } else {
             p_field->next = p_tmp_field;
             p_field = p_tmp_field;
          }
    }

    *ptFieldEnd = p_field;
    return 0;
} 

int top_parser_cfg_index(top_parser_buf_t* this,
                      table_desc_t *ptTable,
                      cfg_field_t  **ptFieldBegin,
                      cfg_field_t  **ptFieldEnd)
{
    int rc, i;
    table_index_t *p_index;
    cfg_field_t   *p_field = NULL, *p_tmp_field;
    char *p;

    gs_token[gn_token] = '\0';
    p = strchr(gs_token, ':');
    ++p;

    p_index = top_parser_find_index(this, ptTable, p, strlen(p));
    if (p_index == NULL)
        return -1;

    for (i = 0; i < p_index->nIndexField; ++i) {
         p_tmp_field = calloc(1, sizeof(cfg_field_t));
         p_tmp_field->field_type = FIELD_NORMAL;
         p_tmp_field->pt_field = p_index->ptIndexField[i];
         p_tmp_field->opr_type = STATMENT_OPR_EQUAL;
         p_tmp_field->con_type = STATMENT_CON_AND;
         if (p_field == NULL) {
             p_field = p_tmp_field;
             *ptFieldBegin = p_field;
         } else {
             p_field->next = p_tmp_field;
             p_field = p_tmp_field;
          }
    }

    *ptFieldEnd = p_field;
    return 0;
}

int top_parser_cfg_field(top_parser_buf_t* this,
                         table_desc_t *ptTable,
                         cfg_field_t  **ptFieldBegin,
                         cfg_field_t  **ptFieldEnd)
{
    field_desc_t *p_field;
    cfg_field_t  *p_tmp_field;

    gs_token[gn_token] = '\0';
    p_field = top_parser_find_col(this, ptTable, gs_token, gn_token);
    if (p_field == NULL)
        return -1;

    p_tmp_field = calloc(1, sizeof(cfg_field_t));
    p_tmp_field->field_type = FIELD_NORMAL;
    p_tmp_field->pt_field = p_field;
    p_tmp_field->opr_type = STATMENT_OPR_EQUAL;
    p_tmp_field->con_type = STATMENT_CON_AND; 

    *ptFieldBegin = p_tmp_field;
    *ptFieldEnd = p_tmp_field;
    return 0;
}

int top_parser_cfg_opr_all(top_parser_buf_t* this,
                           table_desc_t *ptTable,
                           cfg_field_t  **ptFieldBegin,
                           cfg_field_t  **ptFieldEnd)
{
    field_desc_t *ptField;
    cfg_field_t   *p_field = NULL, *p_tmp_field;

    ptField = ptTable->pt_field_first;
    while (ptField != NULL) {
        p_tmp_field = calloc(1, sizeof(cfg_field_t));
        p_tmp_field->field_type = FIELD_NORMAL;
        p_tmp_field->pt_field = ptField;
        if (p_field == NULL) {
            p_field = p_tmp_field;
            *ptFieldBegin = p_field;
        } else {
            p_field->next = p_tmp_field;
            p_field = p_tmp_field;
        }
        ptField = ptField->next;
    }

    *ptFieldEnd = p_field;
    return 0;
} 

int top_parser_cfg_opr_field(top_parser_buf_t* this,
                           table_desc_t *ptTable,
                           cfg_field_t  **ptFieldBegin,
                           cfg_field_t  **ptFieldEnd)
{
    field_desc_t *p_field;
    cfg_field_t  *p_tmp_field;
    char         *p = NULL;
    int          opr_type = 0;

    gs_token[gn_token] = '\0';
    if ((p = strchr(gs_token, ':')) != NULL) {
        if (*(p + 1) == '+')
            opr_type = STATMENT_OPR_FIELD_ADD;
    	else if (*(p + 1) == '-')
            opr_type = STATMENT_OPR_FIELD_MINUS;
		else if (*(p + 1) == 's' || *(p + 1) == 'S')
			opr_type = STATMENT_OPR_FIELD_SUM;
        *p = 0;
    	gn_token = strlen(gs_token);
    }    

	if (strncasecmp(gs_token, "count", 5) == 0) {
		p_tmp_field = calloc(1, sizeof(cfg_field_t));
		p_tmp_field->field_type = FIELD_ROWCOUNT; 
		p_tmp_field->pt_field = NULL;
		p_tmp_field->opr_type = STATMENT_OPR_FIELD_COUNT;
		*ptFieldBegin = p_tmp_field;
		*ptFieldEnd = p_tmp_field;
		return 0;
	} else {

    	p_field = top_parser_find_col(this, ptTable, gs_token, gn_token);
    	if (p_field == NULL)
        	return -1;
	}

    p_tmp_field = calloc(1, sizeof(cfg_field_t));
    p_tmp_field->field_type = FIELD_NORMAL;
    p_tmp_field->pt_field = p_field;
    p_tmp_field->opr_type = opr_type;
    *ptFieldBegin = p_tmp_field;
    *ptFieldEnd = p_tmp_field;
    return 0;
}
     

int top_parser_cfg_subexpr(top_parser_buf_t* this,
                           table_desc_t *ptTable,
                           cfg_field_t  **ptFieldBegin,
                           cfg_field_t  **ptFieldEnd)
{
    int rc;

    if (gn_token == 3 && !strncasecmp(gs_token, "pk:", gn_token)) {
        rc  = top_parser_cfg_pk(this, ptTable, ptFieldBegin, ptFieldEnd); 
        if (rc < 0)
            return -1;
    } else if (gn_token > 6 && !strncasecmp(gs_token, "index:", 6)) {
        rc  = top_parser_cfg_index(this, ptTable, ptFieldBegin, ptFieldEnd);
        if (rc < 0)
            return -1;
    } else {
        rc  = top_parser_cfg_field(this, ptTable, ptFieldBegin, ptFieldEnd);
        if (rc < 0)
            return -1;
    }

    return 0;
} 

int top_parser_cfg_subopr(top_parser_buf_t* this,
                          table_desc_t *ptTable,
                          cfg_field_t  **ptFieldBegin,
                          cfg_field_t  **ptFieldEnd)
{
    int rc;

    if (gn_token == 4 && !strncasecmp(gs_token, "all:", gn_token)) {
        rc = top_parser_cfg_opr_all(this, ptTable, ptFieldBegin, ptFieldEnd);
        if (rc < 0)
            return -1;
    } else {
        rc = top_parser_cfg_opr_field(this, ptTable, ptFieldBegin, ptFieldEnd);
        if (rc < 0)
            return -1;
    }
   
    return 0;
}  

int top_parser_cfg_expr(top_parser_buf_t* this,
                        table_desc_t *ptTable,
                        cfg_field_t  **ptFieldBegin,
                        cfg_field_t  **ptFieldEnd)
{
    field_desc_t *p_field;
    int rc;
 
    while (1) {
        NEXT_TOKEN;
        switch (gn_token_type) {
        case TOKEN_STRING:
            rc = top_parser_cfg_subexpr(this, ptTable, ptFieldBegin, ptFieldEnd);
            if (rc < 0)
                return -1; 
            ptFieldBegin = &((*ptFieldEnd)->next); 
            break;
        case TOKEN_LEFT_BRACE:
            *ptFieldBegin = calloc(1, sizeof(cfg_field_t));
            (*ptFieldBegin)->field_type = FIELD_LEFT_BRACE; 
            rc = top_parser_cfg_expr(this, ptTable, &((*ptFieldBegin)->nest), ptFieldEnd);
            if (rc < 0)
                return -1;
            ptFieldBegin = &((*ptFieldBegin)->next);
            break; 
        case TOKEN_RIGHT_BRACE:
            *ptFieldEnd = *ptFieldBegin = calloc(1, sizeof(cfg_field_t));
            (*ptFieldBegin)->field_type = FIELD_RIGHT_BRACE;
            return 0;
        case TOKEN_AND: 
            (*ptFieldEnd)->con_type = STATMENT_CON_AND;
            break;
        case TOKEN_OR:
            (*ptFieldEnd)->con_type = STATMENT_CON_OR;
            break;
        case TOKEN_LIKE:
            (*ptFieldEnd)->opr_type = STATMENT_OPR_LIKE;
            break;
        case TOKEN_NOT_LIKE:
            (*ptFieldEnd)->opr_type = STATMENT_OPR_NOT_LIKE;
            break;
        case TOKEN_GREAT:
            (*ptFieldEnd)->opr_type = STATMENT_OPR_GREAT;
            break;
        case TOKEN_EQUAL: 
            (*ptFieldEnd)->opr_type = STATMENT_OPR_EQUAL;
            break;
        case TOKEN_GREAT_EQUAL:
            (*ptFieldEnd)->opr_type = STATMENT_OPR_GREAT_EQUAL;
            break;
        case TOKEN_LESS:
            (*ptFieldEnd)->opr_type = STATMENT_OPR_LESS;
            break;
        case TOKEN_LESS_EQUAL:
            (*ptFieldEnd)->opr_type = STATMENT_OPR_LESS_EQUAL;
            break;
        case TOKEN_NOT_EQUAL:      
            (*ptFieldEnd)->opr_type = STATMENT_OPR_NOT_EQUAL;
            break;
        case TOKEN_QUOTA:
            NEXT_TOKEN;
            (*ptFieldEnd)->const_value = calloc(1, gn_token + 1);
            memcpy((*ptFieldEnd)->const_value, gs_token, gn_token);
            (*ptFieldEnd)->value_type = CONST_VALUE;
            NEXT_TOKEN;
            if (gn_token_type != TOKEN_QUOTA) {
                fprintf(stderr, "line: %d col: %d parser error, "
                        "expect key word: '\n",
                        this->prev_line, this->prev_col);
                return -1;
            }
            break;     
        case TOKEN_DOUBLE_QUOTA:
            NEXT_TOKEN;
            (*ptFieldEnd)->value_type = FIELD_VALUE;
            gs_token[gn_token] = '\0';
            p_field = top_parser_find_col(this, ptTable, gs_token, gn_token);
            if (p_field == NULL) {
                printf("field %s not exist in table\n", gs_token);
                return -1;
            }
            (*ptFieldEnd)->const_value = (char*)p_field;
            NEXT_TOKEN;
            if (gn_token_type != TOKEN_DOUBLE_QUOTA) {
                fprintf(stderr, "line: %d col: %d parser error, "
                        "expect key word: '\n",
                        this->prev_line, this->prev_col);
                return -1;
            }
            break;
        case TOKEN_ANGLE:
            return 0;
        default:
            fprintf(stderr, "line: %d col: %d parser error, " 
					"invalid token, token type %d\n",
					this->prev_line, this->prev_col, gn_token_type);
            return -1;
        }
    }

    return 0;
}
    
int top_parser_cfg_opr(top_parser_buf_t* this,
                       table_desc_t *ptTable,
                       cfg_field_t  **ptFieldBegin,
                       cfg_field_t  **ptFieldEnd)
{
    int rc;

    while (1) {
        NEXT_TOKEN;
        switch (gn_token_type) {
        case TOKEN_STRING:
            rc = top_parser_cfg_subopr(this, ptTable, ptFieldBegin, ptFieldEnd);
            if (rc < 0)
                return -1;
            ptFieldBegin = &((*ptFieldEnd)->next);
            break;
        case TOKEN_COMMA:
            break;
        case TOKEN_ANGLE:
            return 0;
        case TOKEN_EQUAL: 
            (*ptFieldEnd)->opr_type = STATMENT_OPR_EQUAL;
            break;
        case TOKEN_QUOTA:
            NEXT_TOKEN;
            (*ptFieldEnd)->const_value = calloc(1, gn_token + 1);
            memcpy((*ptFieldEnd)->const_value, gs_token, gn_token);
            (*ptFieldEnd)->value_type = CONST_VALUE;
            NEXT_TOKEN;
            if (gn_token_type != TOKEN_QUOTA) {
                fprintf(stderr, "line: %d col: %d parser error, "
                        "expect key word: '\n",
                        this->prev_line, this->prev_col);
                return -1;
            }
            break;     
        default:
            return -1;
        }
    }

    return 0;
}

cfg_statment_t* top_parser_cfg_select(top_parser_buf_t* this, 
                                      table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = SELECT_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }
 
    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);  
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }
    
    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;
    if (p_stat->opr_field_list == NULL) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect field list\n",
                this->prev_line, this->prev_col);
        return NULL;
    } 
    return p_stat;
}

cfg_statment_t* top_parser_cfg_lock(top_parser_buf_t* this, 
                                      table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = LOCK_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }
 
    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);  
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }
    
    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;
    if (p_stat->opr_field_list == NULL) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect field list\n",
                this->prev_line, this->prev_col);
        return NULL;
    } 
    return p_stat;
}

cfg_statment_t* top_parser_cfg_count(top_parser_buf_t* this,
                                     table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = COUNT_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    return p_stat;
}

cfg_statment_t* top_parser_cfg_sum(top_parser_buf_t* this,
                                   table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = SUM_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    
    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }
    
    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;
    if (p_stat->opr_field_list == NULL) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect field list\n",
                this->prev_line, this->prev_col);
        return NULL;
    } 

    return p_stat;
}

cfg_statment_t* top_parser_cfg_max(top_parser_buf_t* this,
        table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = MAX_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;


    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);

        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;
    if (p_stat->opr_field_list == NULL) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect field list\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    return p_stat;
}

cfg_statment_t* top_parser_cfg_min(top_parser_buf_t* this,
        table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = MIN_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;


    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);

        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;
    if (p_stat->opr_field_list == NULL) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect field list\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    return p_stat;
}

cfg_statment_t* top_parser_cfg_update(top_parser_buf_t* this, 
                                      table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = UPDATE_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;

    return p_stat;
}

cfg_statment_t* top_parser_cfg_delete(top_parser_buf_t* this, 
                                      table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = DELETE_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    return p_stat;
}

cfg_statment_t* top_parser_cfg_cursor(top_parser_buf_t* this, 
                                      table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = CURSOR_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }
 
    /* parser the cursor order by expr */
    p_field_begin = NULL;
    p_field_end = NULL;
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->sort_field_list = p_field_begin;
    return p_stat;
}

cfg_statment_t* top_parser_cfg_stat(top_parser_buf_t* this, 
                                      table_desc_t *ptTable)
{
    int rc;
    cfg_statment_t *p_stat;
    cfg_field_t *p_field_begin = NULL, *p_field_end = NULL;

    NEXT_TOKEN;

    p_stat = calloc(1, sizeof(cfg_statment_t));
    gs_token[gn_token] = '\0';
    p_stat->idx = atoi(gs_token);
    p_stat->type = STAT_STATMENT;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the condition expr */
    rc = top_parser_cfg_expr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->con_field_list = p_field_begin;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }

    /* parse the select field expr */
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->opr_field_list = p_field_begin;

    NEXT_TOKEN;
    if (gn_token_type != TOKEN_ANGLE) {
        free(p_stat);
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect key word: ^\n",
                this->prev_line, this->prev_col);
        return NULL;
    }
 
    /* parser the cursor group by expr */
    p_field_begin = NULL;
    p_field_end = NULL;
    rc = top_parser_cfg_opr(this, ptTable, &p_field_begin, &p_field_end);
    if (rc < 0) {
        free(p_stat);
        return NULL;
    }
    p_stat->group_field_list = p_field_begin;
    return p_stat;
}

int top_parser_cfg_put_stmt(table_statment_t* ptTableStmt, cfg_statment_t *p_stmt)
{
    cfg_statment_t **p_p_prev;

    if (p_stmt->type == SELECT_STATMENT)
        p_p_prev = &(ptTableStmt->pt_select_st);
    else if (p_stmt->type == LOCK_STATMENT)         
        p_p_prev = &(ptTableStmt->pt_lock_st);
    else if (p_stmt->type == DELETE_STATMENT)         
        p_p_prev = &(ptTableStmt->pt_delete_st);
    else if (p_stmt->type == UPDATE_STATMENT)         
        p_p_prev = &(ptTableStmt->pt_update_st);
    else if (p_stmt->type == CURSOR_STATMENT)         
        p_p_prev = &(ptTableStmt->pt_cursor_st);
	else if (p_stmt->type == STAT_STATMENT)
		p_p_prev = &(ptTableStmt->pt_stat_st);
    else if (p_stmt->type == COUNT_STATMENT)         
        p_p_prev = &(ptTableStmt->pt_count_st);
    else if (p_stmt->type == SUM_STATMENT)         
        p_p_prev = &(ptTableStmt->pt_sum_st);
    else if (p_stmt->type == MAX_STATMENT)
        p_p_prev = &(ptTableStmt->pt_max_st);
    else if (p_stmt->type == MIN_STATMENT)
        p_p_prev = &(ptTableStmt->pt_min_st);
    else {
        fprintf(stderr, "invalid statement type:  %d\n", p_stmt->type);
        return -1;
    }

    while (*p_p_prev != NULL) {
        if (p_stmt->idx == (*p_p_prev)->idx) {
            fprintf(stderr, "statement idx %d duplicate\n", (*p_p_prev)->idx);
            return -1;
        } 
        if (p_stmt->idx < (*p_p_prev)->idx)
            break; 
        p_p_prev = &((*p_p_prev)->next);
    }

    p_stmt->next = *p_p_prev;
    *p_p_prev = p_stmt;
    return 0;
}
    
int top_parser_cfg_file(top_parser_buf_t *this, table_desc_t *ptTable, table_statment_t* ptTableStmt)
{
    int rc;
    int quit = 0;
    cfg_statment_t *p_stmt;

    SKIP_BLANKS;

    while (!quit) {
        switch (state) {
            case STATE_INIT:
                SKIP_BLANKS;
                if (CUR == '\0')
                    state = STATE_END;
                else if (CUR == '#') /* comment */
                    NEXT_LINE;
                else
                    state = STATE_ST_BEGIN;
                break;
            case STATE_ST_BEGIN:
                NEXT_TOKEN;
                if (gn_token_type == TOKEN_STRING && 
                    gn_token == 6 && !strncasecmp(gs_token, "select", gn_token)) {
                    state = STATE_ST_SELECT_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING && 
                    gn_token == 4 && !strncasecmp(gs_token, "lock", gn_token)) {
                    state = STATE_ST_LOCK_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING &&
                           gn_token == 6 && !strncasecmp(gs_token, "update", gn_token)) {
                    state = STATE_ST_UPDATE_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING &&
                           gn_token == 6 && !strncasecmp(gs_token, "delete", gn_token)) {
                    state = STATE_ST_DELETE_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING &&
                           gn_token == 6 && !strncasecmp(gs_token, "cursor", gn_token)) {
                    state = STATE_ST_CURSOR_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING &&
                           gn_token == 5 && !strncasecmp(gs_token, "count", gn_token)) {
                    state = STATE_ST_COUNT_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING &&
						  gn_token == 4 && !strncasecmp(gs_token, "stat", gn_token)) {
					state = STATE_ST_STAT_BEGIN;
					break;
				}  else if (gn_token_type == TOKEN_STRING &&
                           gn_token == 3 && !strncasecmp(gs_token, "sum", gn_token)) {
                    state = STATE_ST_SUM_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING &&
                           gn_token == 3 && !strncasecmp(gs_token, "max", gn_token)) {
                    state = STATE_ST_MAX_BEGIN;
                    break;
                } else if (gn_token_type == TOKEN_STRING &&
                           gn_token == 3 && !strncasecmp(gs_token, "min", gn_token)) {
                    state = STATE_ST_MIN_BEGIN;
                    break;
                } else {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: select/update/delete/cursor/stat/count/sum/max/min\n"
                            "but current word: [%s]",
                            this->prev_line, this->prev_col, gs_token);
                    return -1;
                }
            case STATE_ST_SELECT_BEGIN:
                p_stmt = top_parser_cfg_select(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc) 
                    return -1;
                state = STATE_ST_SELECT_END;
                break;
            case STATE_ST_SELECT_END:
                state = STATE_INIT;
                break;
            case STATE_ST_LOCK_BEGIN:
                p_stmt = top_parser_cfg_lock(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc) 
                    return -1;
                state = STATE_ST_LOCK_END;
                break;
            case STATE_ST_LOCK_END:
                state = STATE_INIT;
                break;
            case STATE_ST_UPDATE_BEGIN:
                p_stmt = top_parser_cfg_update(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc) 
                    return -1;
                state = STATE_ST_UPDATE_END;
                break;
            case STATE_ST_UPDATE_END:
                state = STATE_INIT;
                break;
            case STATE_ST_DELETE_BEGIN:
                p_stmt = top_parser_cfg_delete(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc) 
                    return -1;
                state = STATE_ST_DELETE_END;
                break;
            case STATE_ST_DELETE_END:
                state = STATE_INIT;
                break;
            case STATE_ST_CURSOR_BEGIN:
                p_stmt = top_parser_cfg_cursor(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc) 
                    return -1;
                state = STATE_ST_CURSOR_END;
                break;
            case STATE_ST_CURSOR_END:
                state = STATE_INIT;
                break;
            case STATE_ST_COUNT_BEGIN:
                p_stmt = top_parser_cfg_count(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc)
                    return -1;
                state = STATE_ST_COUNT_END;
                break;
			case STATE_ST_COUNT_END:
				state = STATE_INIT;
				break;
			case STATE_ST_STAT_BEGIN:
				p_stmt = top_parser_cfg_stat(this, ptTable);
				if (p_stmt == NULL)
						return -1;
				rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
				if (rc) 
						return -1;
				state = STATE_ST_COUNT_END;
				break;
			case STATE_ST_STAT_END:
				state = STATE_INIT;
				break;

            case STATE_ST_SUM_BEGIN:
                p_stmt = top_parser_cfg_sum(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc)
                    return -1;
                state = STATE_ST_SUM_END;
                break;
            case STATE_ST_SUM_END:
                state = STATE_INIT;
                break;
            case STATE_ST_MAX_BEGIN:
                p_stmt = top_parser_cfg_max(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc)
                    return -1;
                state = STATE_ST_MAX_END;
                break;
            case STATE_ST_MAX_END:
                state = STATE_INIT;
                break;
            case STATE_ST_MIN_BEGIN:
                p_stmt = top_parser_cfg_min(this, ptTable);
                if (p_stmt == NULL)
                    return -1;
                rc = top_parser_cfg_put_stmt(ptTableStmt, p_stmt);
                if (rc)
                    return -1;
                state = STATE_ST_MIN_END;
                break;
            case STATE_ST_MIN_END:
                state = STATE_INIT;
                break;
            case STATE_END:
                quit = 1;  
                break;                   
        }
    }

    return 0;
}

int top_parser_output_header(top_parser_buf_t *this, table_desc_t *ptTable, table_statment_t* ptTableStmt)
{
    int  rc;
    char path[256];
    top_file_t *file_struct;

    char   dtm[32];
    time_t current;
    struct tm *tmCurrentTime, tmCurr;

    tzset();
    time(&current);
    tmCurrentTime = localtime_r(&current, &tmCurr);
    memset(dtm, 0, sizeof(dtm));
    sprintf(dtm, "%4d/%02d/%02d %02d:%02d:%02d",
            tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
            tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
            tmCurrentTime->tm_min, tmCurrentTime->tm_sec);

    this->file_esql->file_printf(this->file_esql, "/*\n");
    this->file_esql->file_printf(this->file_esql, 
          " *  Copyright 2006, Shanghai Huateng Software Systems Co., Ltd.\n");
    this->file_esql->file_printf(this->file_esql, " *  All right reserved.\n *\n"); 
    this->file_esql->file_printf(this->file_esql, 
          " *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF SHANGHAI HUATENG\n");
    this->file_esql->file_printf(this->file_esql, 
          " *  SOFTWARE SYSTEMS CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT\n"); 
    this->file_esql->file_printf(this->file_esql, 
          " *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,\n");
    this->file_esql->file_printf(this->file_esql, 
          " *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMISSION OF\n");
    this->file_esql->file_printf(this->file_esql,
          " *  SHANGHAI HUATENG SOFTWARE SYSTEMS CO., LTD.\n *\n");
    this->file_esql->file_printf(this->file_esql, " *  $Id$\n *\n"); 
    this->file_esql->file_printf(this->file_esql, 
          " *  function: table %.*s operator function\n *\n", ptTable->table_name_len, ptTable->table_name);
    this->file_esql->file_printf(this->file_esql, " *  Edit History:\n *\n");
    this->file_esql->file_printf(this->file_esql, " *    %s by gendb\n", dtm);
    this->file_esql->file_printf(this->file_esql, " */\n");
    this->file_esql->file_printf(this->file_esql, " \n");
    this->file_esql->file_printf(this->file_esql, "#include <stdio.h>\n");
    this->file_esql->file_printf(this->file_esql, "#include <string.h>\n");
    this->file_esql->file_printf(this->file_esql, "#include <stdlib.h>\n");
    this->file_esql->file_printf(this->file_esql, "#include \"dbs/dbsdef.h\"\n");
    this->file_esql->file_printf(this->file_esql, "EXEC SQL include sqlca;\n");
    this->file_esql->file_printf(this->file_esql, "EXEC SQL include sqlda;\n\n");

    this->file_esql->file_printf(this->file_esql, "\nEXEC SQL BEGIN DECLARE SECTION;\n");
 
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql, "struct %.*s_Def {\n",
           ptTable->table_name_len, ptTable->table_name);
   
    top_parser_sql_struct_field(ptTable, this->file_esql);

    this->file_esql->file_printf(this->file_esql, "} lt%.*s;\n", 
           ptTable->table_name_len, ptTable->table_name);

    this->file_esql->file_printf(this->file_esql, "short s_ind;\n");
    
    this->file_esql->file_printf(this->file_esql, "char  s_count[33];\n");

    this->file_esql->file_printf(this->file_esql, "EXEC SQL END DECLARE SECTION;\n");

    
#ifdef icepine_08262012
/* 写 dbsqlstruct.h , 供dbuser下程序使用 */
    file_struct = disk_file_create();
    if (file_struct == NULL) {
        printf("disk_file_create error: %d", errno);
        return -1;
    }

    this->file_struct = file_struct;

    memset(path, 0, sizeof(path));
    sprintf(path, "%s/sql/dbsqlstruct.h", getenv("GENDB_HOME"));

    rc = file_struct->file_open(file_struct, path, "w");
    if (rc) {
        printf("file_open [%s] error: %d", path, errno);
        disk_file_free(file_struct);
        return -1;
    }

    this->file_struct->file_printf(this->file_struct, "#ifndef _%.*s_H\n#define _%.*s_H\n",
                                   ptTable->table_name_len, ptTable->table_name,
                                   ptTable->table_name_len, ptTable->table_name);

    this->file_struct->file_printf(this->file_struct, "\nEXEC SQL BEGIN DECLARE SECTION;\n");
 
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    this->file_struct->file_printf(this->file_struct, "struct %.*s_Def {\n",
           ptTable->table_name_len, ptTable->table_name);
   
    top_parser_sql_struct_field(ptTable, this->file_struct);

    this->file_struct->file_printf(this->file_struct, "} lt%.*s;\n", 
           ptTable->table_name_len, ptTable->table_name);

    this->file_struct->file_printf(this->file_struct, "short s_ind;\n");
    
    this->file_struct->file_printf(this->file_struct, "char  s_count[33];\n");

    this->file_struct->file_printf(this->file_struct, "EXEC SQL END DECLARE SECTION;\n");

    this->file_struct->file_printf(this->file_struct, "\n#endif\n");

    disk_file_free(file_struct);
/* 写 dbsqlstruct.h 结束 */
#endif

    this->file_esql->file_printf(this->file_esql, 
           "/*************************************************************************/\n");
    this->file_esql->file_printf(this->file_esql, 
           "/*    input : DBS_FUNC   the dbs func                                    */\n");
    this->file_esql->file_printf(this->file_esql,
           "/*            The struct pointer defined in upper.                       */\n");
    this->file_esql->file_printf(this->file_esql,
           "/*    return : the sqlca.sqlcode                                         */\n");
    this->file_esql->file_printf(this->file_esql,
           "/*************************************************************************/\n");

    StrToLower(ptTable->table_name, ptTable->table_name_len); 
    return 0;
}

static const char* OperatorToStr(int opr_type)
{
    switch (opr_type) {
    case STATMENT_OPR_EQUAL:
        return "=";
    case STATMENT_OPR_GREAT:
        return ">";
    case STATMENT_OPR_GREAT_EQUAL:
        return ">=";
    case STATMENT_OPR_LESS:
        return "<";
    case STATMENT_OPR_LESS_EQUAL:
        return "<=";
    case STATMENT_OPR_NOT_EQUAL:
        return "<>";
    case STATMENT_OPR_LIKE:
        return "LIKE";
    case STATMENT_OPR_NOT_LIKE:
        return "NOT LIKE";
    default:
        return "unknown opr";
    }
}   

int top_parser_output_expr(top_parser_buf_t *this, table_desc_t *ptTable, 
                           cfg_field_t* ptField, int indent)
{
    int rc;

    while (ptField != NULL) {
        if (ptField->field_type == FIELD_LEFT_BRACE) {
            this->file_esql->file_printf(this->file_esql, "%*.*s(\n", indent, indent, " ");
            rc = top_parser_output_expr(this, ptTable, ptField->nest, indent + 2);
            if (rc < 0)
                return -1;
            if (ptField->next != NULL && ptField->next->field_type != FIELD_RIGHT_BRACE) { 
                if (rc != STATMENT_CON_AND && rc != STATMENT_CON_OR) {
                    fprintf(stderr, "no right brace found\n");
                    return -1;
                } 
                this->file_esql->file_printf(this->file_esql,
                       " %s\n", (rc == STATMENT_CON_AND ? "AND" : "OR"));
            }
        } else if (ptField->field_type == FIELD_RIGHT_BRACE) {
            this->file_esql->file_printf(this->file_esql, "\n%*.*s)", indent - 2, indent - 2, " ");
            return ptField->con_type;
        } else {
            StrToUpper(ptField->pt_field->field_name, ptField->pt_field->field_name_len);
            this->file_esql->file_printf(this->file_esql,
                   "%*.*s%.*s %s ", indent, indent, " ",
                   ptField->pt_field->field_name_len,
                   ptField->pt_field->field_name,
                   OperatorToStr(ptField->opr_type));

            StrToLower(ptField->pt_field->field_name, ptField->pt_field->field_name_len);
            if (ptField->const_value == NULL) {
                StrToUpper(ptTable->table_name, ptTable->table_name_len);
                this->file_esql->file_printf(this->file_esql,
                       ":lt%.*s.%.*s",
                       ptTable->table_name_len, ptTable->table_name,
                       ptField->pt_field->field_name_len,
                       ptField->pt_field->field_name); 
                StrToLower(ptTable->table_name, ptTable->table_name_len);
            } else if (ptField->value_type == CONST_VALUE) {
                if (ptField->pt_field->header_type == C_CHAR_C)
                    this->file_esql->file_printf(this->file_esql,
                           "'%s'", ptField->const_value); 
                else 
                    this->file_esql->file_printf(this->file_esql,
                           "%s", ptField->const_value); 
            } else if (ptField->value_type == FIELD_VALUE) {
                    StrToUpper(ptTable->table_name, ptTable->table_name_len);
                    this->file_esql->file_printf(this->file_esql,
                           ":lt%.*s.%.*s",
                           ptTable->table_name_len, ptTable->table_name,
                           ((field_desc_t*)(ptField->const_value))->field_name_len,
                           ((field_desc_t*)(ptField->const_value))->field_name);
                    StrToLower(ptTable->table_name, ptTable->table_name_len);
            }
            
            if (ptField->next != NULL && ptField->next->field_type != FIELD_RIGHT_BRACE)  
                this->file_esql->file_printf(this->file_esql,
                       " %s\n", (ptField->con_type == STATMENT_CON_AND ? "AND" : "OR"));
        }

        ptField = ptField->next;
    }             

    return 0;
}

int top_parser_output_update_field(top_parser_buf_t *this, table_desc_t *ptTable,
                                   cfg_field_t *pOprField, int indent)
{
    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    while (pOprField != NULL) {
        if (pOprField->field_type == FIELD_NORMAL) {
            StrToUpper(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);

			if(strncasecmp(pOprField->pt_field->field_name, "rowid", pOprField->pt_field->field_name_len) == 0) {
				pOprField = pOprField->next;
				continue;
			}
            this->file_esql->file_printf(this->file_esql,
                   "%*.*s%-*.*s=", indent, indent, " ",
                   24, 
                   pOprField->pt_field->field_name_len,
                   pOprField->pt_field->field_name);

            if (pOprField->opr_type == STATMENT_OPR_FIELD_ADD) {
                this->file_esql->file_printf(this->file_esql,
                       " %.*s +", pOprField->pt_field->field_name_len,
                       pOprField->pt_field->field_name);
            } else if (pOprField->opr_type == STATMENT_OPR_FIELD_MINUS) {
            	this->file_esql->file_printf(this->file_esql,
                       " %.*s -", pOprField->pt_field->field_name_len,
                       pOprField->pt_field->field_name);
            } 

            if (pOprField->const_value != NULL) {
                if (pOprField->pt_field->header_type == C_CHAR_C)
                    this->file_esql->file_printf(this->file_esql,
                           " '%s'", pOprField->const_value); 
                else 
                    this->file_esql->file_printf(this->file_esql,
                           " %s", pOprField->const_value); 
            } else {
                StrToLower(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);
                this->file_esql->file_printf(this->file_esql,
                       " :lt%.*s.%.*s",
                       ptTable->table_name_len, ptTable->table_name,
                       pOprField->pt_field->field_name_len,
                       pOprField->pt_field->field_name);
             } 
             if (pOprField->next != NULL)
                 this->file_esql->file_printf(this->file_esql, " ,\n");
        }
        pOprField = pOprField->next;
    }

    StrToLower(ptTable->table_name, ptTable->table_name_len);    
    return 0;
}

int top_parser_output_select_field(top_parser_buf_t *this, table_desc_t *ptTable, 
                                   cfg_field_t *pOprField, int indent)
{
    while (pOprField != NULL) {
        if (pOprField->field_type == FIELD_NORMAL) {
            StrToUpper(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);
			
			if (pOprField->opr_type == STATMENT_OPR_FIELD_SUM) {
				this->file_esql->file_printf(this->file_esql,
                   "%*.*sSUM(%.*s)", indent, indent, " ", 
                   pOprField->pt_field->field_name_len,
                   pOprField->pt_field->field_name);
			} else {
				this->file_esql->file_printf(this->file_esql,
                   "%*.*s%.*s", indent, indent, " ", 
                   pOprField->pt_field->field_name_len,
                   pOprField->pt_field->field_name);
			}
            

            if (pOprField->next != NULL)
                this->file_esql->file_printf(this->file_esql, " ,\n");
            else
                this->file_esql->file_printf(this->file_esql, "\n");
            
            StrToLower(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);
        } else if (pOprField->field_type == FIELD_ROWCOUNT) {
#ifdef _DB_ORA
			this->file_esql->file_printf(this->file_esql,
				"%*.*s%s", indent, indent, " ", 
				"to_char(nvl(count(1), 0))");
#else
			this->file_esql->file_printf(this->file_esql,
				"%*.*s%s", indent, indent, " ", 
				"to_char(coalesce(count(1), 0)), \n");
#endif
			if (pOprField->next != NULL)
                this->file_esql->file_printf(this->file_esql, " ,\n");
            else
                this->file_esql->file_printf(this->file_esql, "\n");
		}
        pOprField = pOprField->next;
    }

    return 0;
}

int top_parser_output_order_field(top_parser_buf_t *this, table_desc_t *ptTable,
                                  cfg_field_t *pOprField, int indent)
{
    while (pOprField != NULL) {
        if (pOprField->field_type == FIELD_NORMAL) {
            StrToUpper(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);

            this->file_esql->file_printf(this->file_esql,
                   "%*.*s%.*s", indent, indent, " ",
                   pOprField->pt_field->field_name_len,
                   pOprField->pt_field->field_name);
            
            if (pOprField->opr_type == STATMENT_OPR_FIELD_ADD)
                this->file_esql->file_printf(this->file_esql," asc");
            else if (pOprField->opr_type == STATMENT_OPR_FIELD_MINUS)
         	this->file_esql->file_printf(this->file_esql," desc");

            if (pOprField->next != NULL)
                this->file_esql->file_printf(this->file_esql, " ,\n");
            
            StrToLower(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);
        }
        pOprField = pOprField->next;
    }

    return 0;
}

int top_parser_output_group_field(top_parser_buf_t *this, table_desc_t *ptTable,
                                  cfg_field_t *pOprField, int indent)
{
    while (pOprField != NULL) {
        if (pOprField->field_type == FIELD_NORMAL) {
            StrToUpper(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);

            this->file_esql->file_printf(this->file_esql,
                   "%*.*s%.*s", indent, indent, " ",
                   pOprField->pt_field->field_name_len,
                   pOprField->pt_field->field_name);
            

            if (pOprField->next != NULL)
                this->file_esql->file_printf(this->file_esql, " ,\n");
            
            StrToLower(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);
        }
        pOprField = pOprField->next;
    }

    return 0;
}


int top_parser_output_into_field(top_parser_buf_t *this, table_desc_t *ptTable,
                                 cfg_field_t *pOprField, int indent, int cursor)
{
    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    while (pOprField != NULL) {
        if (pOprField->field_type == FIELD_NORMAL) {
            StrToLower(pOprField->pt_field->field_name, pOprField->pt_field->field_name_len);

			this->file_esql->file_printf(this->file_esql,
                   "%*.*s:lt%.*s.%.*s :s_ind", 
                   indent, indent, " ",
                   ptTable->table_name_len, ptTable->table_name,
                   pOprField->pt_field->field_name_len,
                   pOprField->pt_field->field_name); 

            if (pOprField->next != NULL)
                this->file_esql->file_printf(this->file_esql, " ,\n");
            else if (cursor == 1)
                this->file_esql->file_printf(this->file_esql, ";\n");
            else
                this->file_esql->file_printf(this->file_esql, "\n");
        } else if (pOprField->field_type == FIELD_ROWCOUNT) {
			
			this->file_esql->file_printf(this->file_esql,
					"%*.*s:s_count", indent, indent, " ");
			if (pOprField->next != NULL)
                this->file_esql->file_printf(this->file_esql, " ,\n");
            else
                this->file_esql->file_printf(this->file_esql, "\n");
		}
        pOprField = pOprField->next;
    }

    StrToLower(ptTable->table_name, ptTable->table_name_len);    
    return 0;
}   

int top_parser_output_cursor_o_st(top_parser_buf_t *this, table_desc_t *ptTable,
                                  cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL DECLARE\n", indent, indent, " ");
   
    /* cursor name */
    this->file_esql->file_printf(this->file_esql,
           "%*.*scur_%.*s_%d",
           indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name,
           ptStmt->idx);
    
    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sCURSOR FOR SELECT\n", 
           indent, indent, " ");

    /* select field list */
    rc = top_parser_output_select_field(this, ptTable, ptStmt->opr_field_list, indent + 4);
    if (rc < 0)
        return -1;
 
    this->file_esql->file_printf(this->file_esql,
           "%*.*sFROM\n%*.*s%.*s", indent, indent, " ",
            indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");
   
        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }

    if (ptStmt->sort_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sORDER BY\n", indent, indent, " ");
        rc = top_parser_output_order_field(this, ptTable, ptStmt->sort_field_list, indent + 4);    
        if (rc < 0)
            return -1;
    }
 
    this->file_esql->file_printf(this->file_esql, ";\n");
    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sEXEC SQL OPEN cur_%.*s_%d;\n", indent, indent, " ",
           ptTable->table_name_len, ptTable->table_name, ptStmt->idx);
    
    this->file_esql->file_printf(this->file_esql,
           "%*.*sbreak;\n\n", indent, indent, " ");
 
    return 0;
} 


int top_parser_output_cursor_f_st(top_parser_buf_t *this, table_desc_t *ptTable,
                                  cfg_statment_t* ptStmt, int indent)
{
    int rc;
 
    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL FETCH cur_%.*s_%d\n", indent, indent, " ",
           ptTable->table_name_len, ptTable->table_name, ptStmt->idx);
    
    this->file_esql->file_printf(this->file_esql,
           "%*.*sINTO\n", indent, indent, " ");
    
    /* select into list */
    rc = top_parser_output_into_field(this, ptTable, ptStmt->opr_field_list, indent + 4, 1);
    if (rc < 0)
        return -1;
    
    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    this->file_esql->file_printf(this->file_esql,
           "\n%*.*smemcpy(p%.*s, &lt%.*s, sizeof(lt%.*s));",
           indent, indent, " ",
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name);
    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
}

int top_parser_output_cursor_c_st(top_parser_buf_t *this, table_desc_t *ptTable,
                                  cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL CLOSE cur_%.*s_%d;\n", indent, indent, " ",
           ptTable->table_name_len, ptTable->table_name, ptStmt->idx);

    this->file_esql->file_printf(this->file_esql,
           "%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
}

int top_parser_output_select_st(top_parser_buf_t *this, table_desc_t *ptTable, 
                                cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL SELECT\n", indent, indent, " ");
       
    /* select field list */
    rc = top_parser_output_select_field(this, ptTable, ptStmt->opr_field_list, indent + 4);
    if (rc < 0)
        return -1;
 
    this->file_esql->file_printf(this->file_esql,
           "%*.*sINTO\n", indent, indent, " ");

    /* select into list */
    rc = top_parser_output_into_field(this, ptTable, ptStmt->opr_field_list, indent + 4, 0);
    if (rc < 0)
        return -1;
 
    this->file_esql->file_printf(this->file_esql,
           "%*.*sFROM\n%*.*s%.*s", indent, indent, " ", 
            indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql, ";\n");

    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    this->file_esql->file_printf(this->file_esql,
           "\n%*.*smemcpy(p%.*s, &lt%.*s, sizeof(lt%.*s));",
           indent, indent, " ",
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name);
    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
} 

int top_parser_output_lock_st(top_parser_buf_t *this, table_desc_t *ptTable, 
                                cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL SELECT\n", indent, indent, " ");
       
    /* select field list */
    rc = top_parser_output_select_field(this, ptTable, ptStmt->opr_field_list, indent + 4);
    if (rc < 0)
        return -1;
 
    this->file_esql->file_printf(this->file_esql,
           "%*.*sINTO\n", indent, indent, " ");

    /* select into list */
    rc = top_parser_output_into_field(this, ptTable, ptStmt->opr_field_list, indent + 4, 0);
    if (rc < 0)
        return -1;
 
    this->file_esql->file_printf(this->file_esql,
           "%*.*sFROM\n%*.*s%.*s", indent, indent, " ", 
            indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sfor update;\n", indent, indent, " ");

    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    this->file_esql->file_printf(this->file_esql,
           "\n%*.*smemcpy(p%.*s, &lt%.*s, sizeof(lt%.*s));",
           indent, indent, " ",
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name);
    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
}

int top_parser_output_count_st(top_parser_buf_t *this, table_desc_t *ptTable,
                               cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL SELECT ", indent, indent, " ");

#ifdef _DB_ORA
        this->file_esql->file_printf(this->file_esql,
               "to_char(nvl(count(*), 0))\n");
#else
        this->file_esql->file_printf(this->file_esql,
               "to_char(coalesce(count(*), 0))\n");
#endif

         
    this->file_esql->file_printf(this->file_esql,
           "%*.*sINTO :s_count\n", indent, indent, " ");

    this->file_esql->file_printf(this->file_esql,
           "%*.*sFROM\n%*.*s%.*s", indent, indent, " ",
            indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql, ";\n");

    this->file_esql->file_printf(this->file_esql,
           "%*.*sstrncpy(psCount, s_count, nCntSize - 1);",
           indent, indent, " ");

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
}


int top_parser_output_stat_st(top_parser_buf_t *this, table_desc_t *ptTable,
                               cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL SELECT \n", indent, indent, " ");

	/* select field list */
    rc = top_parser_output_select_field(this, ptTable, ptStmt->opr_field_list, indent + 4);
	if (rc < 0)
		return -1;

	this->file_esql->file_printf(this->file_esql,
		"%*.*sINTO\n", indent, indent, " ");

	/* select into list */
    rc = top_parser_output_into_field(this, ptTable, ptStmt->opr_field_list, indent + 4, 0);
    if (rc < 0)
        return -1;
	
    this->file_esql->file_printf(this->file_esql,
           "%*.*sFROM\n%*.*s%.*s", indent, indent, " ",
            indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
	
	if (ptStmt->group_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sGROUP BY\n", indent, indent, " ");
        rc = top_parser_output_group_field(this, ptTable, ptStmt->group_field_list, indent + 4);    
        if (rc < 0)
            return -1;
    }
	
    this->file_esql->file_printf(this->file_esql, ";\n");

    this->file_esql->file_printf(this->file_esql,
           "%*.*sif (psCount && nCntSize > 0) strncpy(psCount, s_count, nCntSize - 1);",
           indent, indent, " ");

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
}

int top_parser_output_sum_st(top_parser_buf_t *this, table_desc_t *ptTable,
                              cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL SELECT ", indent, indent, " ");

    StrToUpper(ptStmt->opr_field_list->pt_field->field_name,
               ptStmt->opr_field_list->pt_field->field_name_len);
 
#ifdef _DB_ORA
        this->file_esql->file_printf(this->file_esql,
               "to_char(nvl(sum(%.*s), 0))\n", 
        ptStmt->opr_field_list->pt_field->field_name_len,
        ptStmt->opr_field_list->pt_field->field_name);
#else
        this->file_esql->file_printf(this->file_esql,
               "to_char(coalesce(sum(%.*s), 0))\n",
        ptStmt->opr_field_list->pt_field->field_name_len,
        ptStmt->opr_field_list->pt_field->field_name);
#endif

    this->file_esql->file_printf(this->file_esql,
           "%*.*sINTO :s_count\n", indent, indent, " ");

    this->file_esql->file_printf(this->file_esql,
           "%*.*sFROM\n%*.*s%.*s", indent, indent, " ",
            indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql, ";\n");

    this->file_esql->file_printf(this->file_esql,
           "%*.*sstrncpy(psCount, s_count, nCntSize - 1);",
           indent, indent, " ");

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    StrToLower(ptStmt->opr_field_list->pt_field->field_name,
               ptStmt->opr_field_list->pt_field->field_name_len);
    return 0;
}

int top_parser_output_max_st(top_parser_buf_t *this, table_desc_t *ptTable,
        cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
            "%*.*sEXEC SQL SELECT\n", indent, indent, " ");

    StrToUpper(ptStmt->opr_field_list->pt_field->field_name,
            ptStmt->opr_field_list->pt_field->field_name_len);

    this->file_esql->file_printf(this->file_esql,
            "%*.*smax(%.*s)\n",
            indent+4, indent+4, " ",
            ptStmt->opr_field_list->pt_field->field_name_len,
            ptStmt->opr_field_list->pt_field->field_name);

    this->file_esql->file_printf(this->file_esql,
            "%*.*sINTO\n", indent, indent, " ");

    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    StrToLower(ptStmt->opr_field_list->pt_field->field_name,
            ptStmt->opr_field_list->pt_field->field_name_len);
    this->file_esql->file_printf(this->file_esql,
            "%*.*s:lt%.*s.%.*s :s_ind\n",
            indent+4, indent+4, " ",
            ptTable->table_name_len, ptTable->table_name,
            ptStmt->opr_field_list->pt_field->field_name_len,
            ptStmt->opr_field_list->pt_field->field_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);
    this->file_esql->file_printf(this->file_esql,
            "%*.*sFROM\n%*.*s%.*s", indent, indent, " ",
            indent + 4, indent + 4, " ",
            ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
                "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql, ";\n");

    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    this->file_esql->file_printf(this->file_esql,
            "\n%*.*smemcpy(p%.*s, &lt%.*s, sizeof(lt%.*s));",
            indent, indent, " ",
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name);
    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
            "\n%*.*sbreak;\n\n", indent, indent, " ");

    StrToLower(ptStmt->opr_field_list->pt_field->field_name,
            ptStmt->opr_field_list->pt_field->field_name_len);
    return 0;
}

int top_parser_output_min_st(top_parser_buf_t *this, table_desc_t *ptTable,
        cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
            "%*.*sEXEC SQL SELECT\n", indent, indent, " ");

    StrToUpper(ptStmt->opr_field_list->pt_field->field_name,
            ptStmt->opr_field_list->pt_field->field_name_len);

    this->file_esql->file_printf(this->file_esql,
            "%*.*smin(%.*s)\n",
            indent+4, indent+4, " ", 
            ptStmt->opr_field_list->pt_field->field_name_len,
            ptStmt->opr_field_list->pt_field->field_name);

    this->file_esql->file_printf(this->file_esql,
            "%*.*sINTO\n", indent, indent, " ");

    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    StrToLower(ptStmt->opr_field_list->pt_field->field_name,
            ptStmt->opr_field_list->pt_field->field_name_len);
    this->file_esql->file_printf(this->file_esql,
            "%*.*s:lt%.*s.%.*s :s_ind\n",
            indent+4, indent+4, " ",
            ptTable->table_name_len, ptTable->table_name,
            ptStmt->opr_field_list->pt_field->field_name_len,
            ptStmt->opr_field_list->pt_field->field_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);
    this->file_esql->file_printf(this->file_esql,
            "%*.*sFROM\n%*.*s%.*s", indent, indent, " ",
            indent + 4, indent + 4, " ",
            ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
                "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql, ";\n");

    StrToUpper(ptTable->table_name, ptTable->table_name_len);
    this->file_esql->file_printf(this->file_esql,
            "\n%*.*smemcpy(p%.*s, &lt%.*s, sizeof(lt%.*s));",
            indent, indent, " ",
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name);
    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
            "\n%*.*sbreak;\n\n", indent, indent, " ");

    StrToLower(ptStmt->opr_field_list->pt_field->field_name,
            ptStmt->opr_field_list->pt_field->field_name_len);
    return 0;
}


int top_parser_output_update_st(top_parser_buf_t *this, table_desc_t *ptTable,
                                cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL UPDATE\n", indent, indent, " ");

    this->file_esql->file_printf(this->file_esql,
           "%*.*s%.*s\n%*.*sSET\n",
           indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name,
           indent, indent, " ");

    /* update field list */
    rc = top_parser_output_update_field(this, ptTable, ptStmt->opr_field_list, indent + 4);
    if (rc < 0)
        return -1;

    if (ptStmt->con_field_list != NULL) {
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql, ";\n");

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
}

int top_parser_output_delete_st(top_parser_buf_t *this, table_desc_t *ptTable,
                                cfg_statment_t* ptStmt, int indent)
{
    int rc;

    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL DELETE\n", indent, indent, " ");

    this->file_esql->file_printf(this->file_esql,
           "%*.*sFROM\n%*.*s%.*s", indent, indent, " ", 
            indent + 4, indent + 4, " ",
           ptTable->table_name_len, ptTable->table_name);

    if (ptStmt->con_field_list != NULL) { 
        this->file_esql->file_printf(this->file_esql,
               "\n%*.*sWHERE\n", indent, indent, " ");

        /* condition list */
        rc = top_parser_output_expr(this, ptTable, ptStmt->con_field_list, indent + 4);
        if (rc < 0)
            return -1;
    }
    this->file_esql->file_printf(this->file_esql, ";\n");

    this->file_esql->file_printf(this->file_esql,
           "\n%*.*sbreak;\n\n", indent, indent, " ");

    return 0;
}

int top_parser_output_insert_st(top_parser_buf_t *this, table_desc_t *ptTable,
                                cfg_statment_t* ptStmt, int indent)
{
    int               rc;
    field_desc_t     *ptField;
    top_file_t       *file_move = NULL;
    top_parser_buf_t *ptIpathParser;
    char              path[256]={0}; 

    if(nWorking) {
        ptIpathParser = top_parser_get_ipath();
        file_move     = ptIpathParser->file_move;
    }

    char               sTableName[256] = {0};
    strncpy(sTableName, ptTable->table_name, 256);
    if(file_move != NULL) {
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "int MoveIml2%.*s(void)\n{\n", ptTable->table_name_len, ptTable->table_name);
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*sint  iRet = -1;\n", 4, 4, " ");
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*schar sTmp[128]={0};\n\n", 4, 4, " ");
        StrToUpper(sTableName, ptTable->table_name_len);
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*s%.*s_def st%.*s;\n\n", 4, 4, " ", ptTable->table_name_len, ptTable->table_name,
                ptTable->table_name_len, sTableName); 
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*smemset(&st%.*s, 0x00, sizeof(%.*s_def));\n\n",
                4, 4, " ", ptTable->table_name_len, sTableName, 
                ptTable->table_name_len, ptTable->table_name);

        top_file_t *file_struct;
        char path[1024 + 1] = {0};

        file_struct = disk_file_create();
        if (file_struct == NULL) {
            printf("disk_file_create error: %d", errno);
            return -1;
        }
        snprintf(path, sizeof(path), "dbstu_%.*s.h", ptTable->table_name_len, ptTable->table_name);

        rc = file_struct->file_open(file_struct, path, "a");
        if (rc) {
            printf("file_open [%s] error: %d", path, errno);
            disk_file_free(file_struct);
            return -1;
        }

        file_struct->file_printf(file_struct,
                "\n\nextern int MoveIml2%.*s(void);\n",
                ptTable->table_name_len, ptTable->table_name);
        disk_file_free(file_struct);
    }
   
    this->file_esql->file_printf(this->file_esql,
           "%*.*sEXEC SQL INSERT INTO %.*s (\n", indent, indent, " ",
           ptTable->table_name_len, ptTable->table_name);
   
    ptField = ptTable->pt_field_first;
    while (ptField != NULL) {
        StrToUpper(ptField->field_name, ptField->field_name_len);

		if (strncasecmp(ptField->field_name, "rowid", ptField->field_name_len) == 0){
			ptField = ptField->next;
			continue;
		}
        this->file_esql->file_printf(this->file_esql,
               "%*.*s%.*s", indent + 4, indent + 4, " ",
               ptField->field_name_len,
               ptField->field_name);

            if (ptField->next != NULL)
                this->file_esql->file_printf(this->file_esql, " ,\n");
            else
                this->file_esql->file_printf(this->file_esql, "\n");
         
        StrToLower(ptField->field_name, ptField->field_name_len);
        ipath_key_value_t *ptKey;
        ipath_key_value_t *ptVal;
        ptKey = top_parser_find_key(ptField->field_name, ptField->field_name_len);
        if(ptKey && file_move) {
            if(ptField->header_type == C_SHORT_C || ptField->header_type == C_INT_C) {
                if(ptField->comment_len != 0) {
                    ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                            "%*.*s/* %.*s */\n", 4, 4, " ",
                            ptField->comment_len, ptField->comment);
                }
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*smemset(sTmp, 0x00, sizeof(sTmp));\n", 4, 4, " ");
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*sImlGetNodeVal(\"%.*s\", sizeof(sTmp), sTmp, NULL);\n",
                        4, 4, " ", ptKey->value_len, ptKey->value);
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*sst%.*s.%.*s = atoi(sTmp);\n",
                        4, 4, " ", ptTable->table_name_len, sTableName,
                        ptField->field_name_len, ptField->field_name);
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*slogger(LOG_DEBUG, \"table name [%.*s], field name [%.*s], value [%%d]\", st%.*s.%.*s);\n\n",
                        4, 4, " ", ptTable->table_name_len, sTableName, ptField->field_name_len, ptField->field_name,
                        ptTable->table_name_len, sTableName, ptField->field_name_len, ptField->field_name);
            } else if (ptField->header_type == C_DOUBLE_C || ptField->header_type == C_FLOAT_C) {
                if(ptField->comment_len != 0) {
                    ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                            "%*.*s/* %.*s */\n", 4, 4, " ",
                            ptField->comment_len, ptField->comment);
                }
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*smemset(sTmp, 0x00, sizeof(sTmp));\n", 4, 4, " ");
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*sImlGetNodeVal(\"%.*s\", sizeof(sTmp), sTmp, NULL);\n",
                        4, 4, " ", ptKey->value_len, ptKey->value);
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*sst%.*s.%.*s = atof(sTmp);\n",
                        4, 4, " ", ptTable->table_name_len, sTableName,
                        ptField->field_name_len, ptField->field_name);
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*slogger(LOG_DEBUG, \"table name [%.*s], field name [%.*s], value [%%f]\", st%.*s.%.*s);\n\n",
                        4, 4, " ", ptTable->table_name_len, sTableName, ptField->field_name_len, ptField->field_name,
                        ptTable->table_name_len, sTableName, ptField->field_name_len, ptField->field_name);
            } else {
                if(ptField->comment_len != 0) {
                    ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                            "%*.*s/* %.*s */\n", 4, 4, " ",
                            ptField->comment_len, ptField->comment);
                }
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*sImlGetNodeVal(\"%.*s\", sizeof(st%.*s.%.*s), st%.*s.%.*s, NULL);\n", 
                        4, 4, " ", ptKey->value_len, ptKey->value, 
                        ptTable->table_name_len, sTableName,
                        ptField->field_name_len, ptField->field_name,
                        ptTable->table_name_len, sTableName, 
                        ptField->field_name_len, ptField->field_name);
                /*
                   ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                   "%*.*sstrRightTrim(st%.*s.%.*s);\n",
                   4, 4, " ",ptTable->table_name_len, sTableName,
                   ptField->field_name_len, ptField->field_name);
                */
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*slogger(LOG_DEBUG, \"table name [%.*s], field name [%.*s], value [%%s]\", st%.*s.%.*s);\n\n",
                        4, 4, " ", ptTable->table_name_len, sTableName, ptField->field_name_len, ptField->field_name,
                        ptTable->table_name_len, sTableName, ptField->field_name_len, ptField->field_name);
            }

        }
        ptField = ptField->next;
    }

    if(file_move != NULL) {
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*siRet = Dbs%.*s(DBS_INSERT, &st%.*s);\n",
                4, 4, " ", ptTable->table_name_len, sTableName,
                ptTable->table_name_len, sTableName);
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*sif(iRet != 0) {\n", 4, 4, " ");
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*slogger(LOG_ERROR, \"insert into table %.*s error [%%d]\", iRet);\n", 
                8, 8, " ", ptTable->table_name_len, ptTable->table_name);
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*sreturn -1;\n%*.*s}\n", 8, 8, " ", 4, 4, " ");
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                "%*.*sreturn 0;\n", 4, 4, " ");
                
        ptIpathParser->file_move->file_printf(ptIpathParser->file_move, "}\n");
    }

    this->file_esql->file_printf(this->file_esql,
           "%*.*s) VALUES (\n", indent, indent, " ");

    ptField = ptTable->pt_field_first;
    StrToUpper(ptTable->table_name, ptTable->table_name_len);
	while (ptField != NULL) {

		if (strncasecmp(ptField->field_name, "rowid", ptField->field_name_len) == 0){
			ptField = ptField->next;
			continue;
		}
        this->file_esql->file_printf(this->file_esql,
              "%*.*s:lt%.*s.%.*s",
              indent + 4, indent + 4, " ",
              ptTable->table_name_len, ptTable->table_name,
              ptField->field_name_len, ptField->field_name);
        
        if (ptField->next != NULL)
            this->file_esql->file_printf(this->file_esql, " ,\n");
        else
            this->file_esql->file_printf(this->file_esql, "\n");
        ptField = ptField->next;
    } 
    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
           "%*.*s);\n", indent, indent, " ");  
    this->file_esql->file_printf(this->file_esql,
           "%*.*sbreak;\n\n", indent, indent, " ");
 
    return 0;
}

int top_parser_output_select(top_parser_buf_t *this, 
                             table_desc_t *ptTable, 
                             cfg_statment_t* ptStmt,
                             int indent)
{
    int rc;
    top_parser_buf_t *ptIpathParser;
    top_file_t       *file_move = NULL;
    char              path[256]={0};
    char              sTableName[256] = {0};

    strncpy(sTableName, ptTable->table_name, 256);
    if(nWorking) {
        file_move = disk_file_create();
        if(file_move != NULL) {
            ptIpathParser = top_parser_get_ipath();
            ptIpathParser->file_move = file_move;
            snprintf(path, sizeof(path), "move_%.*s.c", ptTable->table_name_len, ptTable->table_name);
            rc = file_move->file_open(file_move, path, "w");
            if (rc) {
                file_move = NULL;
            }
        }
    }

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_SELECT :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_SELECT%d :\n", indent, indent, " ", ptStmt->idx);
        }

        if(file_move != NULL) {
            StrToUpper(sTableName, ptTable->table_name_len);
            if (ptStmt->idx == 0) {
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "int Move%.*s2Iml(%.*s_def *pt%.*s)\n{\n", ptTable->table_name_len, ptTable->table_name,
                        ptTable->table_name_len, ptTable->table_name, ptTable->table_name_len, sTableName);
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*sint  iRet = -1;\n%*.*schar sTmp[128] = {0};\n\n", 4, 4, " ", 4, 4, " ");
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*siRet = Dbs%.*s(DBS_SELECT, pt%.*s);\n",
                        4, 4, " ", ptTable->table_name_len, sTableName, ptTable->table_name_len, sTableName);

                top_file_t *file_struct;
                char path[1024 + 1] = {0};

                file_struct = disk_file_create();
                if (file_struct == NULL) {
                    printf("disk_file_create error: %d", errno);
                    return -1;
                }

                snprintf(path, sizeof(path), "dbstu_%.*s.h", ptTable->table_name_len, ptTable->table_name);

                rc = file_struct->file_open(file_struct, path, "a");
                if (rc) {
                    printf("file_open [%s] error: %d", path, errno);
                    disk_file_free(file_struct);
                    return -1;
                }

                file_struct->file_printf(file_struct,
                        "\n\nextern int Move%.*s2Iml(%.*s_def *pt%.*s);\n",
                        ptTable->table_name_len, ptTable->table_name,
                        ptTable->table_name_len, ptTable->table_name, ptTable->table_name_len, sTableName);
                disk_file_free(file_struct);
            } else {
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "\n\nextern int Move%.*s2Iml%d(%.*s_def *pt%.*s)\n{\n", ptTable->table_name_len, ptTable->table_name,
                        ptStmt->idx, ptTable->table_name_len, ptTable->table_name, ptTable->table_name_len, sTableName);
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*sint  iRet = -1;\n%*.*schar sTmp[128] = {0};\n\n", 4, 4, " ", 4, 4, " ");
                ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                        "%*.*siRet = Dbs%.*s(DBS_SELECT%d, pt%.*s);\n",
                        4, 4, " ", ptTable->table_name_len, sTableName, ptStmt->idx,
                        ptTable->table_name_len, sTableName);
                top_file_t *file_struct;
                char path[1024 + 1] = {0};

                file_struct = disk_file_create();
                if (file_struct == NULL) {
                    printf("disk_file_create error: %d", errno);
                    return -1;
                }
                snprintf(path, sizeof(path), "dbstu_%.*s.h", ptTable->table_name_len, ptTable->table_name);

                rc = file_struct->file_open(file_struct, path, "a");
                if (rc) {
                    printf("file_open [%s] error: %d", path, errno);
                    disk_file_free(file_struct);
                    return -1;
                }

                file_struct->file_printf(file_struct,
                        "\n\nextern int Move%.*s2Iml%d(%.*s_def *pt%.*s);\n",
                        ptTable->table_name_len, ptTable->table_name,
                        ptStmt->idx, ptTable->table_name_len, ptTable->table_name, ptTable->table_name_len, sTableName);
                disk_file_free(file_struct);
            }


            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                    "%*.*sif(iRet != 0) {\n", 4, 4, " ");
            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                    "%*.*slogger(LOG_ERROR, \"SELECT %.*s, error [%%d]!\", iRet);\n", 8, 8, " ",
                    ptTable->table_name_len, sTableName);
            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                    "%*.*sreturn -1;\n%*.*s}\n\n", 8, 8, " ", 4, 4, " ");

            ipath_key_value_t *ptField   = NULL;
            cfg_field_t       *ptTmpStmt = ptStmt->opr_field_list;
            while (ptTmpStmt != NULL) {
                if (ptTmpStmt->field_type == FIELD_NORMAL) {
                    ptField = top_parser_find_key(ptTmpStmt->pt_field->field_name, 
                            ptTmpStmt->pt_field->field_name_len);

                    if(ptField != NULL) {
                        ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                "%*.*s/* %.*s */\n", 4, 4, " ", 
                                ptTmpStmt->pt_field->comment_len, ptTmpStmt->pt_field->comment);
                        StrToLower(ptTmpStmt->pt_field->field_name, ptTmpStmt->pt_field->field_name_len);

                        if(ptTmpStmt->pt_field->header_type == C_SHORT_C ||
                                ptTmpStmt->pt_field->header_type == C_INT_C) {
                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                    "%*.*slogger(LOG_DEBUG, \"ipath [%.*s], value [%%d]\", pt%.*s->%.*s);\n",
                                    4,4," ", ptField->value_len, ptField->value,
                                    ptTable->table_name_len, sTableName,
                                    ptTmpStmt->pt_field->field_name_len, ptTmpStmt->pt_field->field_name);
                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                    "%*.*ssnprintf(sTmp, sizeof(sTmp), \"%%d\", pt%.*s->%.*s);\n",
                                    4,4," ", ptTable->table_name_len, sTableName,
                                    ptTmpStmt->pt_field->field_name_len, ptTmpStmt->pt_field->field_name);
                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                    "%*.*sImlSetNodeVal(\"%.*s\", sTmp, -1);\n\n",
                                    4, 4, " ", ptField->value_len, ptField->value);
                        } else if(ptTmpStmt->pt_field->header_type == C_DOUBLE_C ||
                                ptTmpStmt->pt_field->header_type == C_FLOAT_C) {
                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                    "%*.*slogger(LOG_DEBUG, \"ipath [%.*s], value [%%f]\", pt%.*s->%.*s);\n",
                                    4,4," ", ptField->value_len, ptField->value,
                                    ptTable->table_name_len, sTableName,
                                    ptTmpStmt->pt_field->field_name_len, ptTmpStmt->pt_field->field_name);
                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                    "%*.*ssnprintf(sTmp, sizeof(sTmp), \"%%f\", pt%.*s->%.*s);\n",
                                    4,4," ", ptTable->table_name_len, sTableName,
                                    ptTmpStmt->pt_field->field_name_len, ptTmpStmt->pt_field->field_name);
                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                    "%*.*sImlSetNodeVal(\"%.*s\", sTmp, -1);\n\n",
                                    4, 4, " ", ptField->value_len, ptField->value);
                        } else {
                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                    "%*.*slogger(LOG_DEBUG, \"ipath [%.*s], value [%%s]\", pt%.*s->%.*s);\n",
                                    4,4," ", ptField->value_len, ptField->value,
                                    ptTable->table_name_len, sTableName,
                                    ptTmpStmt->pt_field->field_name_len, ptTmpStmt->pt_field->field_name);
                            /*
                               ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                               "%*.*sImlSetNodeVal(\"%.*s\", strRightTrim(pt%.*s->%.*s), -1);\n\n",
                               4, 4, " ", ptField->value_len, ptField->value,
                               ptTable->table_name_len, sTableName, 
                               ptTmpStmt->pt_field->field_name_len, ptTmpStmt->pt_field->field_name);
                            */

                            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                                   "%*.*sImlSetNodeVal(\"%.*s\", pt%.*s->%.*s, -1);\n\n",
                                   4, 4, " ", ptField->value_len, ptField->value,
                                   ptTable->table_name_len, sTableName,
                                   ptTmpStmt->pt_field->field_name_len, ptTmpStmt->pt_field->field_name);
                        }
                    }
                }
                ptTmpStmt = ptTmpStmt->next;
            }

            ptIpathParser->file_move->file_printf(ptIpathParser->file_move,
                    "%*.*sreturn 0;\n}\n\n", 4, 4, " ");
        }

        rc = top_parser_output_select_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }
    
    return 0;
} 

int top_parser_output_lock(top_parser_buf_t *this, 
                             table_desc_t *ptTable, 
                             cfg_statment_t* ptStmt,
                             int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_LOCK :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_LOCK%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_lock_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }
    
    return 0;
} 

int top_parser_output_delete(top_parser_buf_t *this, 
                             table_desc_t *ptTable, 
                             cfg_statment_t* ptStmt,
                             int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_DELETE :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_DELETE%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_delete_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}

int top_parser_output_insert(top_parser_buf_t *this,
                             table_desc_t *ptTable,
                             cfg_statment_t* ptStmt,
                             int indent)
{
    int               rc;
    this->file_esql->file_printf(this->file_esql,
           "%*.*scase DBS_INSERT :\n", indent, indent, " ");

    rc = top_parser_output_insert_st(this, ptTable, ptStmt, indent + 4);
    if (rc < 0)
        return -1;

    return 0;
}

int top_parser_output_update(top_parser_buf_t *this, 
                             table_desc_t *ptTable, 
                             cfg_statment_t* ptStmt,
                             int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_UPDATE :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_UPDATE%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_update_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}

int top_parser_output_count(top_parser_buf_t *this,
                            table_desc_t *ptTable,
                            cfg_statment_t* ptStmt,
                            int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_COUNT :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_COUNT%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_count_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}

int top_parser_output_stat(top_parser_buf_t *this,
                            table_desc_t *ptTable,
                            cfg_statment_t* ptStmt,
                            int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_STAT :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_STAT%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_stat_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}


int top_parser_output_sum(top_parser_buf_t *this,
                          table_desc_t *ptTable,
                          cfg_statment_t* ptStmt,
                          int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_SUM :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_SUM%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_sum_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}

int top_parser_output_max(top_parser_buf_t *this,
        table_desc_t *ptTable,
        cfg_statment_t* ptStmt,
        int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                    "%*.*scase DBS_MAX :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                    "%*.*scase DBS_MAX%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_max_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}

int top_parser_output_min(top_parser_buf_t *this,
        table_desc_t *ptTable,
        cfg_statment_t* ptStmt,
        int indent)
{
    int rc;

    while (ptStmt != NULL) {
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                    "%*.*scase DBS_MIN :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                    "%*.*scase DBS_MIN%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_min_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}

int top_parser_output_cursor(top_parser_buf_t *this, 
                             table_desc_t *ptTable, 
                             cfg_statment_t* ptStmt,
                             int indent)
{
    int rc;

    while (ptStmt != NULL) {
        /* open */
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_OPEN :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_OPEN%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_cursor_o_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        /* fetch */
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_FETCH :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_FETCH%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_cursor_f_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        /* close */
        if (ptStmt->idx == 0) {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_CLOSE :\n", indent, indent, " ");
        } else {
            this->file_esql->file_printf(this->file_esql,
                   "%*.*scase DBS_CLOSE%d :\n", indent, indent, " ", ptStmt->idx);
        }
        rc = top_parser_output_cursor_c_st(this, ptTable, ptStmt, indent + 4);
        if (rc < 0)
            return -1;

        ptStmt = ptStmt->next;
    }

    return 0;
}

int top_parser_output_statment(top_parser_buf_t *this,
                               table_desc_t *ptTable,
                               table_statment_t *ptTableStmt)
{
    int rc;

    rc = top_parser_output_select(this, ptTable, ptTableStmt->pt_select_st, 4);
    if (rc < 0)
        return -1;

    rc = top_parser_output_lock(this, ptTable, ptTableStmt->pt_select_st, 4);
    if (rc < 0)
        return -1;
  
    rc = top_parser_output_update(this, ptTable, ptTableStmt->pt_update_st, 4);
    if (rc < 0)
        return -1;
    
    rc = top_parser_output_delete(this, ptTable, ptTableStmt->pt_delete_st, 4);
    if (rc < 0)
        return -1;

    rc = top_parser_output_cursor(this, ptTable, ptTableStmt->pt_cursor_st, 4);
    if (rc < 0)
        return -1;
   
    rc = top_parser_output_insert(this, ptTable, NULL, 4);
    if (rc < 0)
        return -1;

    rc = top_parser_output_max(this, ptTable, ptTableStmt->pt_max_st, 4);
    if (rc < 0)
        return -1;

    rc = top_parser_output_min(this, ptTable, ptTableStmt->pt_min_st, 4);
    if (rc < 0)
        return -1;

    return 0;
}

int top_parser_output_tail(top_parser_buf_t *this,
                           table_desc_t *ptTable,
                           table_statment_t *ptTableStmt)
{
    this->file_esql->file_printf(this->file_esql,
           "%*.*sdefault :\n", 4, 4, " ");

    this->file_esql->file_printf(this->file_esql,
           "%*.*sreturn DB_DBSFUNCERR;\n", 4 + 4, 4 + 4, " ");

    this->file_esql->file_printf(this->file_esql,
           "%*.*s}\n\n%*.*sreturn sqlca.sqlcode;\n}\n",
           4, 4, " ", 4, 4, " "); 
    return 0;
}

int top_parser_output_normal_statment(top_parser_buf_t *this,
                                      table_desc_t *ptTable,
                                      table_statment_t *ptTableStmt) 
{
    int rc;
    top_file_t *file_struct;
    char path[1024 + 1] = {0};

    StrToUpper(ptTable->table_name, ptTable->table_name_len);
   
    this->file_esql->file_printf(this->file_esql,
           "int Dbs%.*s(int ifunc, struct %.*s_Def *p%.*s)\n{\n",
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name);

    file_struct = disk_file_create();
    if (file_struct == NULL) {
        printf("disk_file_create error: %d", errno);
        return -1;
    }
    StrToLower(ptTable->table_name, ptTable->table_name_len);
    snprintf(path, sizeof(path), "dbstu_%.*s.h", ptTable->table_name_len, ptTable->table_name);
    rc = file_struct->file_open(file_struct, path, "a");
    if (rc) {
        printf("file_open [%s] error: %d", path, errno);
        disk_file_free(file_struct);
        return -1;
    }
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    file_struct->file_printf(file_struct,
            "\n\nextern int Dbs%.*s(int ifunc, ",
            ptTable->table_name_len, ptTable->table_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);
    file_struct->file_printf(file_struct,
            "%.*s_def ",
            ptTable->table_name_len, ptTable->table_name);
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    file_struct->file_printf(file_struct,
            "*p%.*s);\n",
            ptTable->table_name_len, ptTable->table_name);

    disk_file_free(file_struct);

    this->file_esql->file_printf(this->file_esql,
           "    memcpy(&lt%.*s, p%.*s, sizeof(lt%.*s));\n\n",
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);
    
    this->file_esql->file_printf(this->file_esql,
           "    switch (ifunc) {\n");

    rc = top_parser_output_statment(this, ptTable, ptTableStmt);
    if (rc < 0)
        return -1;

    top_parser_output_tail(this, ptTable, ptTableStmt);
    return 0;
}
     
int top_parser_output_count_statment(top_parser_buf_t *this,
                                     table_desc_t *ptTable,
                                     table_statment_t *ptTableStmt)
{
    int rc;
    top_file_t *file_struct;
    char path[1024 + 1] = {0};

    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
            "\n\nint DbsCnt%.*s(int ifunc, struct %.*s_Def *p%.*s, char *psCount, int nCntSize)\n{\n",
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name);

    file_struct = disk_file_create();
    if (file_struct == NULL) {
        printf("disk_file_create error: %d", errno);
        return -1;
    }
    StrToLower(ptTable->table_name, ptTable->table_name_len);
    snprintf(path, sizeof(path), "dbstu_%.*s.h", ptTable->table_name_len, ptTable->table_name);
    rc = file_struct->file_open(file_struct, path, "a");
    if (rc) {
        printf("file_open [%s] error: %d", path, errno);
        disk_file_free(file_struct);
        return -1;
    }
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    file_struct->file_printf(file_struct,
            "\n\nextern int DbsCnt%.*s(int ifunc, ",
            ptTable->table_name_len, ptTable->table_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);
    file_struct->file_printf(file_struct, 
            "%.*s_def ",
            ptTable->table_name_len, ptTable->table_name);
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    file_struct->file_printf(file_struct,
            "*p%.*s, char *psCount, int nCntSize);\n",
            ptTable->table_name_len, ptTable->table_name);

    disk_file_free(file_struct);

    this->file_esql->file_printf(this->file_esql,
           "    memcpy(&lt%.*s, p%.*s, sizeof(lt%.*s));\n\n",
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
           "    memset(s_count, 0, sizeof(s_count));\n\n");

    this->file_esql->file_printf(this->file_esql,
           "    switch (ifunc) {\n");

    rc = top_parser_output_count(this, ptTable, ptTableStmt->pt_count_st, 4); 
    if (rc < 0)
        return -1;

    rc = top_parser_output_sum(this, ptTable, ptTableStmt->pt_sum_st, 4);
    if (rc < 0)
        return -1;

    top_parser_output_tail(this, ptTable, ptTableStmt);
    
    return 0;
}

int top_parser_output_stat_statment(top_parser_buf_t *this,
                                     table_desc_t *ptTable,
                                     table_statment_t *ptTableStmt)
{
    int rc;
    top_file_t *file_struct;
    char path[1024 + 1] = {0};

    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
            "\n\nint DbsStat%.*s(int ifunc, struct %.*s_Def *p%.*s, char *psCount, int nCntSize)\n{\n",
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name,
            ptTable->table_name_len, ptTable->table_name);

    file_struct = disk_file_create();
    if (file_struct == NULL) {
        printf("disk_file_create error: %d", errno);
        return -1;
    }
    StrToLower(ptTable->table_name, ptTable->table_name_len);
    snprintf(path, sizeof(path), "dbstu_%.*s.h", ptTable->table_name_len, ptTable->table_name);
    rc = file_struct->file_open(file_struct, path, "a");
    if (rc) {
        printf("file_open [%s] error: %d", path, errno);
        disk_file_free(file_struct);
        return -1;
    }
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    file_struct->file_printf(file_struct,
            "\n\nextern int DbsStat%.*s(int ifunc, ",
            ptTable->table_name_len, ptTable->table_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);
    file_struct->file_printf(file_struct, 
            "%.*s_def ",
            ptTable->table_name_len, ptTable->table_name);
    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    file_struct->file_printf(file_struct,
            "*p%.*s, char *psCount, int nCntSize);\n",
            ptTable->table_name_len, ptTable->table_name);

    disk_file_free(file_struct);

    this->file_esql->file_printf(this->file_esql,
           "    memcpy(&lt%.*s, p%.*s, sizeof(lt%.*s));\n\n",
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name,
           ptTable->table_name_len, ptTable->table_name);

    StrToLower(ptTable->table_name, ptTable->table_name_len);

    this->file_esql->file_printf(this->file_esql,
           "    memset(s_count, 0, sizeof(s_count));\n\n");

    this->file_esql->file_printf(this->file_esql,
           "    switch (ifunc) {\n");

    rc = top_parser_output_stat(this, ptTable, ptTableStmt->pt_stat_st, 4); 
    if (rc < 0)
        return -1;

    top_parser_output_tail(this, ptTable, ptTableStmt);
    
    return 0;
}

int top_parser_cfg(table_desc_t *ptTable)
{
    char path[256];
    int rc;
    table_statment_t table_stmt;
    char suffix[4], seq[4];
    top_parser_buf_t tBuf;
    top_file_t *file, *file_sql;
 
    file_sql = disk_file_create();
    if (file_sql == NULL) {
        printf("disk_file_create error: %d", errno);
        return -1;
    }

    rc = load_map_config(ptTable->table_name);
    if(rc) {
        nWorking = 0;
    }
#ifdef _DB_ORA
        strcpy(suffix, "pc");
        sprintf(path, "dbsvr_%.*s.%s", ptTable->table_name_len, ptTable->table_name, suffix);
#elif defined(_DB_IFMX)
        strcpy(suffix, "ec");
        sprintf(path, "dbsvr_%.*s.%s", ptTable->table_name_len, ptTable->table_name, suffix);
#else
        strcpy(suffix, "sqc");
        sprintf(path, "dbsvr_%.*s.%s", ptTable->table_name_len, ptTable->table_name, suffix);
#endif
    rc = file_sql->file_open(file_sql, path, "w");
    if (rc) {
        printf("file_open [%s] error: %d", path, errno);
        disk_file_free(file_sql);
        return -1;
    }
    
    file = disk_file_create();
    if (file == NULL) {
        printf("disk_file_create error: %d\n", errno);
        file_sql->file_close(file_sql);
        disk_file_free(file_sql);
        return -1;
    }

    memset(&tBuf, 0, sizeof(tBuf));
    memset(&table_stmt, 0, sizeof(table_stmt));
    sprintf(path, "%s/cfg/%.*s.cfg", getenv("GENDB_HOME"), ptTable->table_name_len, ptTable->table_name);
    rc = file->file_open(file, path, "r");
    if (rc == 0) { /* open success */
        rc = top_parser_buf_create(&tBuf, file, 0, NULL, file_sql);
        if (rc) {
            printf("top_parser_buf_create error: %d\n", rc);
            file->file_close(file);
            file_sql->file_close(file_sql);
            disk_file_free(file_sql);
            disk_file_free(file);
            return rc;
        }

        rc = top_parser_cfg_file(&tBuf, ptTable, &table_stmt);
        if (rc) {
            printf("top_parser_cfg_file error: %d\n", rc);
            top_parser_cfg_free_table(&table_stmt);
            file->file_close(file);
            file_sql->file_close(file_sql);
            disk_file_free(file_sql);
            disk_file_free(file);
            top_parser_buf_free(&tBuf);
            return -1;
        }
    } else { /* open fail */
        rc = top_parser_buf_create(&tBuf, file, 1, NULL, file_sql);
        if (rc) {
            printf("top_parser_buf_create error: %d\n", rc);
            file->file_close(file);
            file_sql->file_close(file_sql);
            disk_file_free(file_sql);
            disk_file_free(file);
            return rc;
        }
    }

    rc = top_parser_output_header(&tBuf, ptTable, &table_stmt);
    if (rc < 0) {
        top_parser_cfg_free_table(&table_stmt);
        file->file_close(file);
        file_sql->file_close(file_sql);
        disk_file_free(file_sql);
        disk_file_free(file);
        top_parser_buf_free(&tBuf);
        return -1;
    }

    rc = top_parser_output_normal_statment(&tBuf, ptTable, &table_stmt);
    if (rc < 0) {
        top_parser_cfg_free_table(&table_stmt);
        file->file_close(file);
        file_sql->file_close(file_sql);
        disk_file_free(file_sql);
        disk_file_free(file);
        top_parser_buf_free(&tBuf);
        return -1;
    }

    rc = top_parser_output_count_statment(&tBuf, ptTable, &table_stmt);
    if (rc < 0) {
        top_parser_cfg_free_table(&table_stmt);
        file->file_close(file);
        file_sql->file_close(file_sql);
        disk_file_free(file_sql);
        disk_file_free(file);
        top_parser_buf_free(&tBuf);
        return -1;
    }
	
	rc = top_parser_output_stat_statment(&tBuf, ptTable, &table_stmt);
    if (rc < 0) {
        top_parser_cfg_free_table(&table_stmt);
        file->file_close(file);
        file_sql->file_close(file_sql);
        disk_file_free(file_sql);
        disk_file_free(file);
        top_parser_buf_free(&tBuf);
        return -1;
    }

    top_parser_cfg_free_table(&table_stmt);
    file->file_close(file);
    file_sql->file_close(file_sql);
    disk_file_free(file_sql);
    disk_file_free(file);
    top_parser_buf_free(&tBuf);
    return 0;
}

int top_cfg_generate_sequence(char *seq_name, int seq_name_len)
{
    top_file_t *file_sql;
    int rc;
    char suffix[4];
    char path[256];

    char   dtm[32];
    time_t current;
    struct tm *tmCurrentTime, tmCurr;

    tzset();
    time(&current);
    tmCurrentTime = localtime_r(&current, &tmCurr);
    memset(dtm, 0, sizeof(dtm));
    sprintf(dtm, "%4d/%02d/%02d %02d:%02d:%02d",
            tmCurrentTime->tm_year + 1900, tmCurrentTime->tm_mon + 1,
            tmCurrentTime->tm_mday, tmCurrentTime->tm_hour,
            tmCurrentTime->tm_min, tmCurrentTime->tm_sec);

    file_sql = disk_file_create();
    if (file_sql == NULL) {
        printf("disk_file_create error: %d", errno);
        return -1;
    }

#ifdef _DB_ORA
        strcpy(suffix, "pc");
#elif defined(_DB_IFMX)
        strcpy(suffix, "ec");
#else
        strcpy(suffix, "sqc");
#endif

    StrToLower(seq_name, seq_name_len); 
    sprintf(path, "dbsvr_%.*s.%s", seq_name_len, seq_name, suffix);
    rc = file_sql->file_open(file_sql, path, "w");
    if (rc) {
        printf("file_open [%s] error: %d", path, errno);
        disk_file_free(file_sql);
        return -1;
    }
   
    file_sql->file_printf(file_sql, "/*\n");
    file_sql->file_printf(file_sql,
          " *  Copyright 2006, Shanghai Huateng Software Systems Co., Ltd.\n");
    file_sql->file_printf(file_sql, " *  All right reserved.\n *\n");
    file_sql->file_printf(file_sql,
          " *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF SHANGHAI HUATENG\n");
    file_sql->file_printf(file_sql,
          " *  SOFTWARE SYSTEMS CO., LTD.  THE CONTENTS OF THIS FILE MAY NOT\n");
    file_sql->file_printf(file_sql,
          " *  BE DISCLOSED TO THIRD PARTIES, COPIED OR DUPLICATED IN ANY FORM,\n");
    file_sql->file_printf(file_sql,
          " *  IN WHOLE OR IN PART, WITHOUT THE PRIOR WRITTEN PERMISSION OF\n");
    file_sql->file_printf(file_sql,
          " *  SHANGHAI HUATENG SOFTWARE SYSTEMS CO., LTD.\n *\n");
    file_sql->file_printf(file_sql, " *  $Id$\n *\n");
    file_sql->file_printf(file_sql,
          " *  function: sequence %.*s operator function\n *\n", seq_name_len, seq_name);
    file_sql->file_printf(file_sql, " *  Edit History:\n *\n");
    file_sql->file_printf(file_sql, " *    %s by gendb\n", dtm);
    file_sql->file_printf(file_sql, " */\n");
    file_sql->file_printf(file_sql, " \n"); 
    file_sql->file_printf(file_sql, "#include <stdio.h>\n");
    file_sql->file_printf(file_sql, "#include <string.h>\n");
    file_sql->file_printf(file_sql, "#include <stdlib.h>\n");
    file_sql->file_printf(file_sql, "#include \"dbs/dbsdef.h\"\n");
    file_sql->file_printf(file_sql, "EXEC SQL include sqlca;\n");
    file_sql->file_printf(file_sql, "EXEC SQL include sqlda;\n\n");

    file_sql->file_printf(file_sql, "EXEC SQL BEGIN DECLARE SECTION;\n");
    file_sql->file_printf(file_sql, "char  s_sequence[50];\n");
    file_sql->file_printf(file_sql, "EXEC SQL END DECLARE SECTION;\n\n");   
    
    StrToUpper(seq_name, seq_name_len); 
    file_sql->file_printf(file_sql, "int DbsGet%.*s(char *psValue, int nValSize)\n{\n", seq_name_len, seq_name);  
   
    file_sql->file_printf(file_sql, "    memset(s_sequence, 0, sizeof(s_sequence));\n\n");
    file_sql->file_printf(file_sql, "    EXEC SQL SELECT to_char(%.*s.nextval)\n", seq_name_len, seq_name);
    file_sql->file_printf(file_sql, "    INTO :s_sequence\n");
#ifdef _DB_ORA
        file_sql->file_printf(file_sql, "    FROM dual;\n");
#elif defined(_DB_DB2)
        file_sql->file_printf(file_sql, "    FROM sysibm.sysdummy1;\n");
#else
        printf("top_cfg_generate_sequence: only support ORA or DB2!\n");
        return -1;
#endif
    file_sql->file_printf(file_sql, "    strncpy(psValue, s_sequence, nValSize - 1);\n");
    file_sql->file_printf(file_sql, "    return sqlca.sqlcode;\n");
    file_sql->file_printf(file_sql, "}\n");
    
    file_sql->file_close(file_sql);
    disk_file_free(file_sql);
    return 0;
}  

int get_tbl_seq(char *table_name, int table_len, char *seq)
{
    FILE *fp;
    char str[1024];
    char path[256];
    char tmp[100];

    memset(path, 0x00, sizeof(path));
    memset(str, 0x00, sizeof(str));
    memset(tmp, 0x00, sizeof(tmp));

    strncpy(tmp, table_name, table_len);
    StrToLower(tmp, table_len);
    
    sprintf(path, "%s/cfg/%.*s.cfg", getenv("GENDB_HOME"), table_len, tmp);
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("open file error. path[%s]\n", path);
        return -1;
    }

    fgets(str,sizeof(str)-1, fp);

    if (strncmp(str, "#id", 2) == 0) {
        strncpy(seq, str+4, sizeof(seq)-1 );
    } else {
        printf("parser cfg file error.usage:[#id:123] %s %d\n",__FILE__, __LINE__);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    return 0;
}
