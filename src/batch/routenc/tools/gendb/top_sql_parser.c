#include "top_parser.h"

#define CUR (*this->cur)
#define NEXT top_parser_next_char(this)
#define SKIP_BLANKS top_parser_skip_blank(this)
#define NEXT_LINE top_parser_next_line(this)
#define CUR_PTR     (this->cur)
#define SET_CUR(p)  (this->cur = p)
#define IS_BLANK_CH(ch) (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
#define IS_TOKEN_CH(ch) (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '(' \
                         || ch == ')' || ch == ';' || ch == ',')
#define SKIP(val) do { \
    this->cur += (val), this->col += (val); \
  } while (0)
#define NEXT_TOKEN top_parser_next_token(this)
#define SAVE_LINE_COL do { \
    this->prev_line = this->line, this->prev_col = this->col; \
 } while (0)

#define IS_TABLE_NAME(ch) (isdigit(ch) || isalpha(ch) || ch == '_')
#define IS_COLUMN_NAME(ch) (isdigit(ch) || isalpha(ch) || ch == '_' )
#define IS_INDEX_NAME(ch) (isdigit(ch) || isalpha(ch) || ch == '_' || ch == '$' || ch == '&')

typedef struct {
    char *szType;
    int  (*pfParserType)(top_parser_buf_t *, int, int *, int *);
    int  nSqlType;
    int  nHeaderType;
    int  nFieldLen;
} table_type_t;  

static enum {STATE_INIT = 0, STATE_DROP_KEY, STATE_COMMENT_KEY, STATE_TABLE_BEGIN, 
             STATE_CREATE_KEY, STATE_TABLE_NAME, STATE_COL_BEGIN, 
             STATE_COL_NAME, STATE_COL_ATTRIBUTE, STATE_TABLE_END,
             STATE_TABLE_PRIMARY_KEY_BEGIN, STATE_TABLE_PRIMARY_KEY_COL_BEGIN,
             STATE_TABLE_PRIMARY_KEY_COL_END, STATE_TABLE_PRIMARY_KEY_END, 
             STATE_COL_END, STATE_TABLE_INDEX, STATE_TABLE_INDEX_COL_BEGIN,
             STATE_TABLE_INDEX_COL_END, STATE_END} state = STATE_INIT; 

int top_parser_col_none(top_parser_buf_t* this, int sql_type, 
                        int *field_len, int *header_type);
int top_parser_col_char(top_parser_buf_t* this, int sql_type,
                        int *field_len, int *header_type);
int top_parser_col_number(top_parser_buf_t* this, int sql_type, 
                          int *field_len, int *header_type);

static table_type_t sg_tTableType[] = 
   {
    {"char",      top_parser_col_char,  S_CHAR_C,      C_CHAR_C,      1},
    {"varchar2",  top_parser_col_char,  S_VARCHAR2_C,  C_CHAR_C,      1},
    {"varchar",   top_parser_col_char,  S_VARCHAR_C,   C_CHAR_C,      1},
    {"smallint",  top_parser_col_none,  S_SMALLINT_C,  C_SHORT_C,     sizeof(short)},
    {"integer",   top_parser_col_none,  S_INTEGER_C,   C_INT_C,       sizeof(int)},
    {"int",       top_parser_col_none,  S_INT_C,       C_INT_C,       sizeof(int)},
    {"double",    top_parser_col_none,  S_DOUBLE_C,    C_DOUBLE_C,    sizeof(double)},
    {"float",     top_parser_col_none,  S_FLOAT_C,     C_DOUBLE_C,    sizeof(double)},
    {"real",      top_parser_col_none,  S_REAL_C,      C_FLOAT_C,     sizeof(float)},
    {"decimal",   top_parser_col_number,S_DECIMAL_C,   C_DOUBLE_C,    sizeof(double)}, 
    {"number",    top_parser_col_number,S_NUMBER_C,    C_DOUBLE_C,    sizeof(double)},
    {"date",      top_parser_col_none,  S_DATE_C,      C_CHAR_C,      15},
    {"timestamp", top_parser_col_none,  S_TIMESTAMP_C, C_CHAR_C,      27}
   };
 
#define TABLE_TYPE_LEN          (sizeof(sg_tTableType) / sizeof(table_type_t))
#define TABLE_TYPE_TYPENAME(i)  (sg_tTableType[i].szType)
#define TABLE_TYPE_PARSERFUN(i) (sg_tTableType[i].pfParserType)
#define TABLE_TYPE_SQLTYPE(i)   (sg_tTableType[i].nSqlType)
#define TABLE_TYPE_HEADTYPE(i)  (sg_tTableType[i].nHeaderType)
#define TABLE_TYPE_FIELDLEN(i)  (sg_tTableType[i].nFieldLen)

static char  gs_token[MAX_TOKEN_LEN]; /* current token */
static int   gn_token;                /* current token len */

void StrToUpper(char *str, int len)
{
	int i;

    for (i = 0; i < len; ++i) {
	    if (str[i] >= 'a' && str[i] <= 'z')
            str[i] = str[i] - 'a' + 'A';
    }
}

void StrToLower(char *str, int len)
{
    int i;

    for (i = 0; i < len; ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] - 'A' + 'a';
    }
}

int top_parser_buf_create(top_parser_buf_t* this, top_file_t* file, 
                          unsigned int size, top_file_t *file_header,
                          top_file_t *file_esql)
{
    this->file = file;
    this->file_header = file_header;
    this->file_esql = file_esql;
    
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

void top_parser_buf_free(top_parser_buf_t* this)
{
    if (this->content != NULL) 
        free(this->content);
}


/*
 * top_parser_buf_grow: 
 * @this: the parser input 
 *
 * Grow up the content of the input buffer, the old data are preserved
 * Returns the number of chars read and stored in the buffer, or -1
 * in case of error.
 */
int top_parser_buf_grow(top_parser_buf_t* this)
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
int top_parser_chk_buf(top_parser_buf_t* this, unsigned int len)
{
    unsigned int left_size = (int)(this->end - this->cur);
    unsigned int size;    
    int rc;

    if (len > this->size)
        return -1;     
 
    if (left_size >= len)
        return 0;

    rc = top_parser_buf_grow(this);
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
void top_parser_next_char(top_parser_buf_t* this)
{
    if (CUR == '\n') { /* a new line */
        this->line++;
        this->col = 1;
    } else
        this->col++;

    this->cur++;
    top_parser_chk_buf(this, 1);

    return;
}

void top_parser_next_line(top_parser_buf_t *this)
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
void top_parser_next_token(top_parser_buf_t* this)
{
    int cur, i;

    SKIP_BLANKS;
    SAVE_LINE_COL;
    
    cur = CUR;
    i = 0;
    while (!IS_TOKEN_CH(cur) && cur != '\0' && i < MAX_TOKEN_LEN) {
        gs_token[i++] = cur;
        NEXT;
        cur = CUR;
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
int top_parser_skip_blank(top_parser_buf_t* this)
{
    int res = 0;
    int cur;
    int rc;

    rc = top_parser_chk_buf(this, 1);
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


int top_parser_col_len(top_parser_buf_t *this, int *col_len)
{
    int i = 0;

    NEXT_TOKEN;

    /* column length execeed limit */
    if (gn_token > *col_len) {
        printf("line: %d col: %d parser error, "
               "column length %.*s execeed limit: %d\n",
               this->prev_line, this->prev_col,
               gn_token, gs_token, *col_len);
        return -1;
    }

    /* no column length */
    if (gn_token == 0) {
        printf("line: %d col: %d parser error, "
               "expect column length define\n",
               this->prev_line, this->prev_col);
        return -1;
    }

    /* check length must contain only digit */
    while (i < gn_token) {
        if (!isdigit(gs_token[i])) {
            printf("line: %d col: %d parser error, "
                   "column length: %.*s contain no-digit: %.c\n",
                   this->prev_line, this->prev_col,
                   gn_token, gs_token, gs_token[i]);
            return -1;
        }
        ++i;
    }
    gs_token[i] = '\0';

    *col_len = atoi(gs_token);
    return 0;
}

int top_parser_col_none(top_parser_buf_t* this, int sql_type, 
                        int *field_len, int *header_type)
{
    int cur;

    SKIP_BLANKS;

    cur = CUR;
    while (cur != ',' && cur != '\0' && cur != ')' && cur != '\n') {
        NEXT;
        cur = CUR;
    }

    SAVE_LINE_COL;

    /* no input */
    if (cur == '\0') {
        printf("line: %d col: %d parser error, "
               "expect key word: , or )\n",
               this->prev_line, this->prev_col);
        return -1;
    }

    return 0;
} 

int top_parser_col_number(top_parser_buf_t* this, int sql_type,
                          int *field_len, int *header_type)
{
    int  rc, presion, scale;

    NEXT_TOKEN;

#ifdef _DB_ORA
	if (CUR != '(') {
		NEXT_TOKEN;
		
		if (CUR == ',' || CUR == ' ' || CUR == ')') {
			*header_type = C_INT_C;
			*field_len   = sizeof(int);
			return top_parser_col_none(this, sql_type, field_len,
                               header_type);
		} else {
			fprintf(stderr, "line: %d col: %d parser error, expect key word: (\n",
							this->prev_line, this->prev_col);
			return -1;

		}
	}

	SKIP(1);
#else
    if (CUR != '(') {
        fprintf(stderr, "line: %d col: %d parser error, expect key word: (\n",
                this->prev_line, this->prev_col);
        return -1;
    }    
    SKIP(1);
#endif
    presion = MAX_NUMBER_PRESION_LEN;
    rc = top_parser_col_len(this, &presion);
    if (rc)
        return rc;

    scale = 0;
    NEXT_TOKEN;
    if (CUR == ',') { /* parser scale length */ 
        SKIP(1);
        scale = MAX_NUMBER_SCALE_LEN;
        rc = top_parser_col_len(this, &scale);
        if (rc)
            return rc;
    } 
      
    NEXT_TOKEN;
    if (CUR != ')') {
        fprintf(stderr, "line: %d col: %d parser error, expect key word: )\n",
                this->prev_line, this->prev_col);
        return -1;
    }
    SKIP(1);

    /* for decimal and number sql type, 
     * we reset field len, c type and dbm type here 
     */
    if (scale == 0 && presion <= 4) {
        *header_type = C_SHORT_C;
        *field_len   = sizeof(short);
    } else if (scale == 0 && presion > 4 && presion <= 9) {
        *header_type = C_INT_C;
        *field_len   = sizeof(int);
    } else {
        *header_type = C_DOUBLE_C; 
        *field_len   = sizeof(double);
    }
                          
    return top_parser_col_none(this, sql_type, field_len, 
                               header_type);
} 

int top_parser_col_char(top_parser_buf_t* this, int sql_type,
                        int *field_len, int *header_type)
{
    int  rc, len;

    NEXT_TOKEN;
 
    if (CUR != '(') {
        fprintf(stderr, "line: %d col: %d parser error, expect key word: (\n",
               this->prev_line, this->prev_col);
        return -1; 
    }
    SKIP(1);

    len = MAX_CHAR_LENGTH;
    rc = top_parser_col_len(this, &len);
    if (rc)
        return rc;
    
    /* for char type, we reset 
     * field len here  
     */ 
    *field_len = len + 1;
 
    NEXT_TOKEN;
    if (CUR != ')') {
        fprintf(stderr, "line: %d col: %d parser error, expect key word: )\n",
               this->prev_line, this->prev_col);
        return -1;
    }
    SKIP(1);

    return top_parser_col_none(this, sql_type, field_len, header_type);
}
 
int top_parser_col_attribute(top_parser_buf_t* this, int *field_len, int *sql_type, int *header_type)
{
    int i = 0, rc;

    NEXT_TOKEN;

    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
               "expect column type define\n",
               this->prev_line, this->prev_col);
        return -1;
    }

    for (i = 0; i < TABLE_TYPE_LEN; ++i) {
        if (!strncasecmp(gs_token, TABLE_TYPE_TYPENAME(i),
                         gn_token)) {
            *sql_type = TABLE_TYPE_SQLTYPE(i);
            *header_type = TABLE_TYPE_HEADTYPE(i);
            *field_len   = TABLE_TYPE_FIELDLEN(i);
            if (TABLE_TYPE_PARSERFUN(i) != NULL)
                rc = TABLE_TYPE_PARSERFUN(i)(this, TABLE_TYPE_SQLTYPE(i), 
                                             field_len, header_type);
            break;   
        }
    }   

    if (i == TABLE_TYPE_LEN) {
        fprintf(stderr, "line: %d col: %d parser error, unknown column type: %.*s\n",
                this->prev_line, this->prev_col, gn_token, gs_token);
        return -1;
    }

    return rc;
} 

field_desc_t* top_parser_find_col(top_parser_buf_t *this, table_desc_t *ptTable,
                                  char *col_name, int col_name_len)
{
    field_desc_t *ptField;

    ptField = ptTable->pt_field_first;
    while (ptField != NULL) {
        if (col_name_len == ptField->field_name_len && 
            !strncasecmp(col_name, ptField->field_name, col_name_len)) 
            break;

        ptField = ptField->next;
    }

    if (ptField == NULL) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "index column: %.*s not exist in table: %.*s\n",
                this->prev_line, this->prev_col, col_name_len, col_name,
                ptTable->table_name_len, ptTable->table_name);
        return NULL;
    }

    return ptField;
}  

table_index_t* top_parser_find_index(top_parser_buf_t *this, table_desc_t *ptTable,
                                     char *index_name, int index_name_len)
{
    table_index_t *ptIndex;

    ptIndex = ptTable->pt_index_first;
    while (ptIndex != NULL) {
        if (ptIndex->nUniqueFlag != 2 &&
            index_name_len == ptIndex->index_name_len &&
            !strncasecmp(index_name, ptIndex->index_name, index_name_len))
            break;
        
        ptIndex = ptIndex->next;
    }

    if (ptIndex == NULL) {
        fprintf(stderr, "index: %.*s not exist in table: %.*s\n",
                index_name_len, index_name, ptTable->table_name_len, ptTable->table_name); 
        return NULL;
    }

    return ptIndex;
}


table_index_t* top_parser_find_primary_key(top_parser_buf_t *this, table_desc_t *ptTable)
{
    table_index_t *ptIndex;

    ptIndex = ptTable->pt_index_first;
    while (ptIndex != NULL) {
        if (ptIndex->nUniqueFlag == 2)
            break;

        ptIndex = ptIndex->next;
    }

    if (ptIndex == NULL) {
        fprintf(stderr, "primary key not exist in table: %.*s\n",
                ptTable->table_name_len, ptTable->table_name);
        return NULL;
    }

    return ptIndex;
}

int top_parser_col_name(top_parser_buf_t *this, char *col_name, int *col_name_len)
{
    int i = 0;

    /* column name exceed limit */
    if (gn_token > *col_name_len) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "column name %.*s exceed limit max length: %d\n",
                this->prev_line, this->prev_col,
                gn_token, gs_token, *col_name_len);
        return -1;
    }

    /* no column name */
    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect column name define\n",
                this->prev_line, this->prev_col);
        return -1;
    }

    if (gs_token[0] == '"') {
        gn_token--;
        memmove(gs_token, gs_token + 1, gn_token);
    }
    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect column name define\n",
                this->prev_line, this->prev_col);
        return -1;
    }
    if (gs_token[gn_token - 1] == '"') {
        gn_token--;
        gs_token[gn_token] = '\0';
    }     
    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect column name define\n",
                this->prev_line, this->prev_col);
        return -1;
    }
    
    while (i < gn_token) {
        if (gs_token[i] == '"') {
            ++i;
            continue;
        }
        if (!IS_COLUMN_NAME(gs_token[i])) {
            fprintf(stderr, "line: %d col: %d parser error, "
                    "field name: %.*s contain illegal character: %.c\n",
                    this->prev_line, this->prev_col, gn_token, gs_token,
                    gs_token[i]);
            return -1;
        }

        col_name[i] = gs_token[i];
        ++i;
    }
    *col_name_len = i;

    return 0;
}

int top_parser_index_name(top_parser_buf_t *this, char *index_name, int *index_name_len)
{
    int i = 0;

    /* index name exceed limit */
    if (gn_token > *index_name_len) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "column name %.*s exceed limit max length: %d\n",
                this->prev_line, this->prev_col,
                gn_token, gs_token, *index_name_len);
        return -1;
    }

    /* no column name */
    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect index name define\n",
                this->prev_line, this->prev_col);
        return -1;
    }

    if (gs_token[0] == '"') {
        gn_token--;
        memmove(gs_token, gs_token + 1, gn_token);
    }
    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect index name define\n",
                this->prev_line, this->prev_col);
        return -1;
    }
    if (gs_token[gn_token - 1] == '"') {
        gn_token--;
        gs_token[gn_token] = '\0';
    }     
    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect index name define\n",
                this->prev_line, this->prev_col);
        return -1;
    }

    while (i < gn_token) {
        if (!IS_INDEX_NAME(gs_token[i])) {
            fprintf(stderr, "line: %d col: %d parser error, "
                    "index name: %.*s contain illegal character: %.c\n",
                    this->prev_line, this->prev_col, gn_token, gs_token,
                    gs_token[i]);
            return -1;
        }

        index_name[i] = gs_token[i];
        ++i;
    }
    *index_name_len = i;

    return 0;
}

int top_parser_table_name(top_parser_buf_t *this, char *table_name, int *table_name_len)
{
    int i = 0;

    /* table name execeed limit */ 
    if (gn_token > *table_name_len) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "table name %.*s exceed limit max length: %d\n",
                this->prev_line, this->prev_col,
                gn_token, gs_token, *table_name_len);
        return -1;
    }
 
    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect table name define\n", 
                this->prev_line, this->prev_col);
        return -1;
    }

    if (gs_token[0] == '"') {
        gn_token--;
        memmove(gs_token, gs_token + 1, gn_token);
    }

    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect table name define\n", 
                this->prev_line, this->prev_col);
        return -1;
    }

    if (gs_token[gn_token - 1] == '"') {
        gn_token--;
        gs_token[gn_token] = '\0';
    }

    if (gn_token == 0) {
        fprintf(stderr, "line: %d col: %d parser error, "
                "expect table name define\n", 
                this->prev_line, this->prev_col);
        return -1;
    }

    while (i < gn_token) {
        if (!IS_TABLE_NAME(gs_token[i])) {
            fprintf(stderr, "line: %d col: %d parser error, "
                    "table name: %.*s contain illegal character: %.c\n",
                    this->prev_line, this->prev_col, gn_token, gs_token,
                    gs_token[i]);
            return -1; 
        }
                                         
        table_name[i] = gs_token[i];
        ++i;
    }
    *table_name_len = i;   

    return 0;
}

static const char * str_c_type(int nCtype)
{
    switch (nCtype) {
    case C_CHAR_C:
        return "char";
    case C_SHORT_C:
        return "short";
    case C_INT_C:
#ifdef _DB_ORA
            return "int";
#else
            return "long";
#endif
    case C_DOUBLE_C:
        return "double";
    case C_FLOAT_C:
        return "float";
    default:
        return "unknown type";
    }
}

int top_parser_sql_struct_field(table_desc_t *ptTable, top_file_t* pFile)
{
    field_desc_t  *ptField;

    ptField = ptTable->pt_field_first;
    while (ptField != NULL) {

        StrToLower(ptField->field_name, ptField->field_name_len);

        pFile->file_printf(pFile, "    %-10s %.*s",
                           str_c_type(ptField->header_type),
                           ptField->field_name_len, ptField->field_name);
        if (ptField->header_type == C_CHAR_C) {
            pFile->file_printf(pFile, "[%d]", ptField->field_len);
        }
        pFile->file_printf(pFile, ";");
        pFile->file_printf(pFile, "    /* %.*s */\n", ptField->comment_len, ptField->comment);
        ptField = ptField->next;
    }

    return 0;
}

int top_parser_sql_struct_ind(table_desc_t *ptTable, top_file_t* pFile)
{
    field_desc_t  *ptField;

    ptField = ptTable->pt_field_first;
    while (ptField != NULL) {

        StrToLower(ptField->field_name, ptField->field_name_len);

        pFile->file_printf(pFile, "    %-10s %.*s_ind", "short",
                           ptField->field_name_len, ptField->field_name);
        pFile->file_printf(pFile, ";\n");
        ptField = ptField->next;
    }

    return 0;
}


int top_parser_sql_desc(top_parser_buf_t *this, table_desc_t *ptTable)
{
    field_desc_t  *ptField, *ptFieldTmp;
    table_index_t *ptIndex, *ptIndexTmp;
    int i, rc;
 
    /* no table define current */
    if (ptTable->pt_field_first == NULL)
        return 0;

    StrToUpper(ptTable->table_name, ptTable->table_name_len);

    printf("\nProcess %.*s ...\n", ptTable->table_name_len, ptTable->table_name);
  
    this->file_header->file_printf(this->file_header, "#ifndef _%.*s_H\n#define _%.*s_H\n",
                                   ptTable->table_name_len, ptTable->table_name,
                                   ptTable->table_name_len, ptTable->table_name);
    this->file_header->file_printf(this->file_header, "\n\ntypedef struct {\n");
    StrToLower(ptTable->table_name, ptTable->table_name_len);

    top_parser_sql_struct_field(ptTable, this->file_header);
 
    this->file_header->file_printf(this->file_header, "} %.*s_def;\n", 
                                   ptTable->table_name_len, ptTable->table_name);

    this->file_header->file_printf(this->file_header, "\n\n#endif\n");

    rc = top_parser_cfg(ptTable);
    if (rc < 0)
        return -1;

    ptIndex = ptTable->pt_index_first;
    while (ptIndex != NULL) {
        ptIndexTmp = ptIndex; 
        ptIndex = ptIndex->next;
        free(ptIndexTmp); /* free index desc */
    }

    /* free field desc */
    ptField = ptTable->pt_field_first;
    while (ptField != NULL) {
        ptFieldTmp = ptField;
        ptField = ptField->next;
        free(ptFieldTmp);
    }

    ptTable->pt_field_first = ptTable->pt_field_last = NULL; 
    ptTable->pt_index_first = ptTable->pt_index_last = NULL;
    return 0;
}

int top_parser_sql_file(top_parser_buf_t *this)
{
    int           rc, cur, quit = 0, len, unique_flag;
    field_desc_t  *ptField, *ptIndexField;
    table_index_t *ptIndex;
    table_desc_t  gt_table = {0}; /* table desc struct */
    int           iAlterPFlg = 0;
 
    SKIP_BLANKS;

    while (!quit) {
        switch (state) {
            case STATE_INIT:
                SKIP_BLANKS;
                if (CUR == '\0')
                    state = STATE_END;
                else if (CUR == '/' || CUR == '-') /* block comment */
                    NEXT_LINE;
                else
                    state = STATE_TABLE_BEGIN;
                break;
            case STATE_TABLE_BEGIN: /* create */
                NEXT_TOKEN;
                /* drop table */
                if (gn_token == 4 && !strncasecmp(gs_token, "drop", gn_token)) {
                    state = STATE_DROP_KEY; 
                    break;
                }
               
                /* create table or create unique index or create index */
                if (gn_token == 6 && !strncasecmp(gs_token, "create", gn_token)) {
                    state = STATE_CREATE_KEY;
                    break;
                }

                /* comment */
                if (gn_token == 7 && !strncasecmp(gs_token, "comment", gn_token)) {
                    state = STATE_COMMENT_KEY;
                    break;
                }

                /* alter */
                if(gn_token == 5 && !strncasecmp(gs_token, "alter", gn_token)) {
                    NEXT_TOKEN;
                    if(gn_token == 5 && !strncasecmp(gs_token, "table", gn_token)) {
                        NEXT_TOKEN;
                        NEXT_TOKEN;
                        if(gn_token == 3 && !strncasecmp(gs_token, "add", gn_token)) {
                            state = STATE_COL_NAME; 
                            iAlterPFlg = 1;
                            break;
                        }
                    }
                }
                 
                fprintf(stderr, "line: %d col: %d parser error, "
                        "expect key word: create/drop/comment/alter\n", 
                        this->prev_line, this->prev_col);
                return -1;
            case STATE_DROP_KEY: /* drop table */
#if 0
                NEXT_TOKEN;
                if ((gn_token != 5 || strncasecmp(gs_token, "index", gn_token)) &&
                    (gn_token != 5 || strncasecmp(gs_token, "table", gn_token)) &&
                    (gn_token != 8 || strncasecmp(gs_token, "sequence", gn_token))) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: table\n", 
                            this->prev_line, this->prev_col);
                    return -1;
                }
                NEXT_TOKEN; /* table name */
                NEXT_TOKEN;
                if (gn_token == 7 && !strncasecmp(gs_token, "cascade", gn_token)) {
                    NEXT_TOKEN;
                    if (gn_token != 11 || strncasecmp(gs_token, "constraints", gn_token)) {
                        fprintf(stderr, "line: %d col: %d parser error, "
                                "expect key word: constraints\n",
                                this->prev_line, this->prev_col);
                        return -1;
                    }
                    NEXT_TOKEN;
                }  
                if (CUR != ';') {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: ;\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }
                
                SKIP(1);
#else
                NEXT_LINE;
#endif
                state = STATE_INIT;
                break;
            case STATE_COMMENT_KEY: /* table/coulmn comment */
                NEXT_TOKEN;
                if (gn_token != 2 || strncasecmp(gs_token, "on", gn_token)) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: on\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }
                
                NEXT_TOKEN;    
                if ((gn_token != 5 || strncasecmp(gs_token, "table", gn_token)) && 
                    (gn_token != 6 || strncasecmp(gs_token, "column", gn_token))) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: table/column\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }
        
                NEXT_TOKEN; /* table.field name */
                int   left = 0;
                char *ptDstStr;
                ptDstStr = strchr(gs_token, '.')+1;
                left     = ptDstStr-gs_token;
                field_desc_t *ptTmpField = top_parser_find_col(this, &gt_table, 
                        ptDstStr, gn_token-left);
                NEXT_TOKEN;
                if (gn_token != 2 || strncasecmp(gs_token, "is", gn_token)) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: is\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }
              
                SKIP_BLANKS;
                if (CUR != '\'') {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: '\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }

                while (1) {
                    NEXT;
                    if (CUR == '\'') {
                        NEXT;
                        if (CUR != '\'')
                            break;
                        if(ptTmpField) {
                            ptTmpField->comment[ptTmpField->comment_len++] = CUR;
                        }
                    }
                    if(ptTmpField) {
                        ptTmpField->comment[ptTmpField->comment_len++] = CUR;
                    }
                    if (CUR == '\0') {
                        fprintf(stderr, "line: %d col: %d parser error, "
                                "expect key word: '\n",
                                this->prev_line, this->prev_col);
                        return -1;
                    }
                }
                
                NEXT_TOKEN;
                if (CUR != ';') {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: ;\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }
                SKIP(1);
                state = STATE_INIT;
                break;
            case STATE_CREATE_KEY: /* create table or create index */
                NEXT_TOKEN;
                if (gn_token == 5 && !strncasecmp(gs_token, "table", gn_token)) {
                    state = STATE_TABLE_NAME;
                    break;
                }
             
                if (gn_token == 8 && !strncasecmp(gs_token, "sequence", gn_token)) {
                    NEXT_TOKEN; /* sequence name */
                    rc = top_cfg_generate_sequence(gs_token, gn_token);
                    if (rc < 0)
                        return rc;
                    while (1) {
                        NEXT_TOKEN;
                        if (CUR == ';') {
                            break; 
                        } else if (CUR == '\0') {
                            fprintf(stderr, "line: %d col: %d parser error, "
                                    "expect key word: ;\n",
                                    this->prev_line, this->prev_col);
                            return -1;
                        } 
                    }
 
                    SKIP(1);
                    state = STATE_INIT;
                    break;
                }
                 
                /* create index or create unique index */ 
                unique_flag = 0; 
                if (gn_token == 6 && !strncasecmp(gs_token, "unique", gn_token)) {
                    NEXT_TOKEN;
                    unique_flag = 1; /* unique index define */
                }
                
                if (gn_token == 5 && !strncasecmp(gs_token, "index", gn_token)) {
                    state = STATE_TABLE_INDEX;
                    break;
                }
 
                printf("line: %d col: %d parser error, "
                       "expect key word: table/unique index/index\n",
                       this->prev_line, this->prev_col);
                return -1;
            case STATE_TABLE_NAME: /* table name */ 
                NEXT_TOKEN;

                /* last table desc process here */
                rc = top_parser_sql_desc(this, &gt_table);
                if (rc)
                    return rc; 

                memset(gt_table.table_name, 0, sizeof(gt_table.table_name));
                gt_table.table_name_len = MAX_NAME_LEN;
                rc = top_parser_table_name(this, gt_table.table_name,
                                           &gt_table.table_name_len);
                if (rc)
                    return rc; 
                state = STATE_COL_BEGIN;
                break;
            case STATE_COL_BEGIN:  /* column */
                NEXT_TOKEN;
                
                if (CUR == '(') {
                    SKIP(1);
                    state = STATE_COL_NAME;
                    break;
                }
                printf("line: %d col: %d parser error, "
                       "expect key word: (\n", 
                       this->prev_line, this->prev_col);
                return -1;
            case STATE_COL_NAME: /* column name */
                NEXT_TOKEN;
              
/* #ifdef _DB_ORA */
                int iOraFlg = 0;
/* #endif */
                /* primary key definition */
                if (gn_token == 10 && !strncasecmp(gs_token, "constraint", gn_token)) {
                    NEXT_TOKEN;
                    NEXT_TOKEN;
                }
                if (gn_token == 7 && !strncasecmp(gs_token, "primary", gn_token)) {
                    state = STATE_TABLE_PRIMARY_KEY_BEGIN;
                    break; 
                }

/* #ifdef _DB_ORA */
                if (gn_token == 6 && !strncasecmp(gs_token, "unique", gn_token)) {
                    iOraFlg = 1;
                    state = STATE_TABLE_PRIMARY_KEY_BEGIN;
                    break;
                }
/* #endif */

                ptField = (field_desc_t*)calloc(1, sizeof(field_desc_t));
                if (ptField == NULL) {
                    fprintf(stderr, "calloc error: %d %s\n", errno, strerror(errno));
                    return -1;
                }

                ptField->field_name_len = MAX_NAME_LEN;
                rc = top_parser_col_name(this, ptField->field_name, &ptField->field_name_len);
                if (rc)
                    return rc;
 
                state = STATE_COL_ATTRIBUTE;
                break;
            case STATE_COL_ATTRIBUTE:
                rc = top_parser_col_attribute(this, &ptField->field_len, 
                                              &ptField->sql_type,
                                              &ptField->header_type);
                if (rc)
                    return rc;
               
                if (CUR == '\n') { /* not exit ',' col attribute define end */
                    SKIP(1);
                    this->line++;
                    this->col = 1;  
                    state = STATE_COL_END;
                } else if (CUR == ')') {
                    state = STATE_COL_END;
                } if (CUR == ',') {
                    SKIP(1);
                    state = STATE_COL_NAME;
                } 

                /* add field desc */
                if (gt_table.pt_field_first == NULL)
                    gt_table.pt_field_first = ptField;
                if (gt_table.pt_field_last != NULL) {
                    gt_table.pt_field_last->next = ptField; 
                }
                gt_table.pt_field_last = ptField;

                break;
            case STATE_TABLE_PRIMARY_KEY_BEGIN:

/* #ifdef _DB_ORA */
                if(iOraFlg == 0) {
/* #endif */
                    NEXT_TOKEN;
                    if (gn_token != 3 || strncasecmp(gs_token, "key", gn_token)) {
                        printf("line: %d col: %d parser error, "
                                "expect key word: key\n",
                                this->prev_line, this->prev_col);
                        return -1;                                
                    }
/* #ifdef _DB_ORA */
                }
/* #endif */

                NEXT_TOKEN;
                
                if (CUR != '(') {
                    printf("line: %d col: %d parser error, "
                           "expect key word: (\n",
                           this->prev_line, this->prev_col); 
                    return -1;
                }
              
                ptIndex = (table_index_t*)calloc(1, sizeof(table_index_t));
                if (ptIndex == NULL) {
                    printf("calloc error: %d %s\n", errno, strerror(errno));
                    return -1;
                }

                snprintf(ptIndex->index_name, MAX_NAME_LEN, "pk_%.*s",
                         gt_table.table_name_len, gt_table.table_name);
                ptIndex->index_name_len = strlen(ptIndex->index_name);
                ptIndex->nUniqueFlag = 2; 
                SKIP(1);
                state = STATE_TABLE_PRIMARY_KEY_COL_BEGIN;
                break;
            case STATE_TABLE_PRIMARY_KEY_COL_BEGIN: 
                NEXT_TOKEN;
               
                ptIndexField = top_parser_find_col(this, &gt_table, gs_token, gn_token);
                if (ptIndexField == NULL)
                    return -1;

                if (ptIndex->nIndexField >= MAX_INDEX_FIELD_NUM) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "index %.*s column num exceed limit: %d\n",
                            this->prev_line, this->prev_col,
                            gn_token, gs_token, MAX_INDEX_FIELD_NUM);
                    return -1;
                }

                ptIndex->ptIndexField[ptIndex->nIndexField++] = ptIndexField;
 
                state = STATE_TABLE_PRIMARY_KEY_COL_END;
                break;
            case STATE_TABLE_PRIMARY_KEY_COL_END:
                NEXT_TOKEN;
                
                if (CUR == ',')
                    state = STATE_TABLE_PRIMARY_KEY_COL_BEGIN; 
                else if (CUR == ')')
                    state = STATE_TABLE_PRIMARY_KEY_END;
                else {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: , or )\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }
                SKIP(1);
                break;
            case STATE_TABLE_PRIMARY_KEY_END:
                /* add index desc */
                if (gt_table.pt_index_first == NULL)
                    gt_table.pt_index_first = ptIndex;
                if (gt_table.pt_index_last != NULL) {
                    gt_table.pt_index_last->next = ptIndex;
                }
                gt_table.pt_index_last = ptIndex; 
                state = STATE_COL_END;
                break;
            case STATE_TABLE_INDEX:
                NEXT_TOKEN;
                
                ptIndex = (table_index_t*)calloc(1, sizeof(table_index_t));
                if (ptIndex == NULL) {
                    fprintf(stderr, "calloc error: %d %s\n", errno, strerror(errno));
                    return -1;
                }

                ptIndex->index_name_len = MAX_NAME_LEN;
                rc = top_parser_index_name(this, ptIndex->index_name, &ptIndex->index_name_len);
                if (rc)
                    return rc;
  
                ptIndex->nUniqueFlag = unique_flag;
 
                NEXT_TOKEN;
                if (gn_token != 2 || strncasecmp(gs_token, "on", gn_token)) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: on\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }

                NEXT_TOKEN;
                if (gn_token != gt_table.table_name_len ||
                    strncasecmp(gs_token, gt_table.table_name, gn_token)) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "table name %.*s error\n",
                            this->prev_line, this->prev_col,
                            gn_token, gs_token);
                    return -1;
                }             
              
                NEXT_TOKEN;
                if (CUR != '(') {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: (\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }    

                SKIP(1);
                state = STATE_TABLE_INDEX_COL_BEGIN;
                break;
            case STATE_TABLE_INDEX_COL_BEGIN:
                NEXT_TOKEN;
                
                ptIndexField = top_parser_find_col(this, &gt_table, gs_token, gn_token);
                if (ptIndexField == NULL)
                    return -1;

                if (ptIndex->nIndexField >= MAX_INDEX_FIELD_NUM) {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "index %.*s column num exceed limit: %d\n",
                            this->prev_line, this->prev_col,
                            gn_token, gs_token, MAX_INDEX_FIELD_NUM);
                    return -1;
                } 
  
                ptIndex->ptIndexField[ptIndex->nIndexField++] = ptIndexField;
               
                NEXT_TOKEN;
                if (gn_token == 3 && !strncasecmp(gs_token, "asc", gn_token)) {
                    NEXT_TOKEN;
                } else if (gn_token == 4 && !strncasecmp(gs_token, "desc", gn_token)) {
                    NEXT_TOKEN;
                }

                if (CUR == ',') {
                    SKIP(1);
                    state = STATE_TABLE_INDEX_COL_BEGIN;
                    break;
                }

                if (CUR == ')') {
                    SKIP(1);
                    state = STATE_TABLE_INDEX_COL_END;
                    break;
                }

                fprintf(stderr, "line: %d col: %d parser error, "
                        "expect key word: , or )\n", 
                        this->prev_line, this->prev_col); 
                return -1;
            case STATE_TABLE_INDEX_COL_END:
                NEXT_TOKEN;

                if (CUR != ';') {
                    printf("line: %d col: %d parser error, "
                           "expect key word: ;\n",
                           this->prev_line, this->prev_col);
                    return -1;
                }

                /* add index desc */
                if (gt_table.pt_index_first == NULL)
                    gt_table.pt_index_first = ptIndex;
                if (gt_table.pt_index_last != NULL) {
                    gt_table.pt_index_last->next = ptIndex;
                }
                gt_table.pt_index_last = ptIndex;

                SKIP(1);
                state = STATE_INIT;
                break;
            case STATE_COL_END:
                if(iAlterPFlg == 0) { 
                    NEXT_TOKEN;
                    if (CUR != ')') {
                        fprintf(stderr, "line: %d col: %d parser error, "
                                "expect key word: )\n",   
                                this->prev_line, this->prev_col); 
                        return -1;
                    }
                    SKIP(1);
                }
                state = STATE_TABLE_END;
                break; 
            case STATE_TABLE_END:
                NEXT_TOKEN;
                if (CUR != ';') {
                    fprintf(stderr, "line: %d col: %d parser error, "
                            "expect key word: ;\n",
                            this->prev_line, this->prev_col);
                    return -1;
                }
                
                SKIP(1);
                state = STATE_INIT;
                break;
            case STATE_END:
                /* last table desc process here */
                rc = top_parser_sql_desc(this, &gt_table);                
                if (rc)
                    return rc; 
                quit = 1;
                break;
        }
    }

    return 0;
}
