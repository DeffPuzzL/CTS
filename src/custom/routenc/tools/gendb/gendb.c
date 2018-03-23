#include "top_parser.h" 
#include <stdio.h>

extern int load_ipath_header(void);

int main(int argc, char **argv)
{
    int  rc;
    char path[256];
    char suffix[4] = {0};
    top_file_t *file;
    top_file_t *file_header;
    top_parser_buf_t tBuf;

    if (argc != 2) {
        printf("USAGE: \n\t%s <table>|<sequence>\n", argv[0]);
        exit(1);
    }

    printf("\n\n\n\n");    
    printf("          ******************************************\n");
    printf("          DataBase Service routine generation tool  \n");    
    printf("                         HUATENG 2015               \n");
    printf("          ******************************************\n");
    printf("\n");
    printf("This tools need the $GENDB_HOME \n");
    printf("This tools need the compile flag DBD \n");
    printf("DBD=_DB_ORA  is uesd by ORACLE \n");
    printf("DBD=_DB_DB2  is used by DB2    \n");
    printf("DBD=_DB_IFMX is used bu INFOMIX\n");
    printf("to locate the home directory and the db type\n");
    if (getenv("GENDB_HOME") == NULL) {
        printf("check the environmental variable $GENDB_HOME error\n");
        exit(1);
    }

#ifdef _DB_ORA
    printf("db type is oracle     \n");
#elif defined(_DB_DB2)
    printf("db type is db2        \n");
#elif defined(_DB_IFMX)
    printf("db type is INFOMIX    \n");
#else
    printf("unset compile cfg DBD \n");
    exit(1);
#endif

#ifdef _DB_ORA
        strcpy(suffix, "pc");
#elif defined(_DB_IFMX)
        strcpy(suffix, "ec");
#elif defined(_DB_DB2)
        strcpy(suffix, "sqc");
#endif
    
    printf("The input %s.sql is located in %s/sql directory.\n", argv[1], getenv("GENDB_HOME"));
    printf("Be sure the .sql file is in create table/create index syntax\n");
    printf("The %s.cfg is located in %s/cfg directory.\n", argv[1], getenv("GENDB_HOME"));
    printf("Be sure the .cfg file is in statment declaration syntax\n");
    printf("The result files includes 2 files in current directory :\n\n");
    printf("    dbstu_[table].h  -- the working structure for database service routine\n");
    printf("                        Append to $IN. \n");
    printf("    dbsvr_[table].%s -- the source code of database service routine\n", suffix);
    printf("                        mv to $DB_SRC. \n");
    printf("\n\n");

    file = disk_file_create();
    if (file == NULL) {
        printf("disk_file_create error: %d\n", errno);
        return -1;
    }

    sprintf(path, "%s/sql/%s.sql", getenv("GENDB_HOME"), argv[1]);   
    rc = file->file_open(file, path, "r");
    if (rc) { 
        printf("file_open [%s] error: %d\n", path, errno); 
        disk_file_free(file);
        return rc;
    }

    file_header = disk_file_create();
    if (file_header == NULL) {
        printf("disk_file_create error: %d", errno);
        file->file_close(file);
        disk_file_free(file);
        return -1;
    }

    sprintf(path, "dbstu_%s.h", argv[1]);
    rc = file_header->file_open(file_header, path, "w");
    if (rc) {
        printf("file_open dbstruct.h error: %d", errno);
        file->file_close(file);
        disk_file_free(file);
        disk_file_free(file_header);
        return rc;
    }

    memset(&tBuf, 0, sizeof(tBuf));
    rc = top_parser_buf_create(&tBuf, file, 0, file_header, NULL);
    if (rc) {
        printf("top_parser_buf_create error: %d\n", rc);
        file->file_close(file);
        file_header->file_close(file_header);
        disk_file_free(file);
        disk_file_free(file_header);
        return rc;
    }

    load_ipath_header();

    rc = top_parser_sql_file(&tBuf);
    if (rc) {
        printf("top_parser_sql_file error: %d\n", rc);
        file->file_close(file);
        file_header->file_close(file_header);
        disk_file_free(file);
        disk_file_free(file_header);
        top_parser_buf_free(&tBuf);
        return rc;
    }

    file->file_close(file); 
    file_header->file_close(file_header);
    disk_file_free(file);
    disk_file_free(file_header);
    top_parser_buf_free(&tBuf);
    printf("Process O.K.\n");
    printf("\n\n");
    return 0;
}
