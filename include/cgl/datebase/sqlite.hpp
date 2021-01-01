/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 sqlite.hpp

 2018-01-17 02:32:26

*/
#ifndef SQLITE_HPP_20180117023226
#define SQLITE_HPP_20180117023226

//create table tabname (set type unique
//"insert into abc (id) values (255)"
//"drop table if exists SqliteBlobTest"
//select name from abc
//update table set name='name'//必须单引号
//INT, INTEGER, TINYINT, SMALLINT, MEDIUMINT, BIGINT, UNSIGNED BIG INT, INT2, INT8
//模糊搜索 select ... from table where name='%.bak'

#include <cstdlib>
#include <stdint.h>
#include <string>
#include <vector>

#include <cgl/public.h>
#include "external/sqlite/sqlite3.h"

#ifdef CGL_PLATFORM_CBUILDER
#pragma link "sqlite.lib"
#endif

namespace cgl{

void SQL_LOG(const char* param, ...)
{
    va_list body;
    va_start(body, param);
    __cgl_log_vprint("SQL > ", param, body);
    va_end(body);
}

std::string to_string(int n)
{
    char buf[32];
    return std::itoa(n, buf, 10);
}

struct sql_table
{
    typedef std::vector<std::string> row_type;
    std::vector<std::string> columns;
    std::vector<row_type> rows;
};



class sql_stmt
{
public:
    sqlite3* database;
    sqlite3_stmt *stmt;

public:
    sql_stmt(sqlite3* db) : database(db), stmt(null)
    {
    }
    ~sql_stmt()
    {
        this->dispose();
    }

    //执行一条命令，成功返回true
    bool exec(const char* command, size_t size)
    {
        if(sqlite3_prepare(database, command, size, &stmt, 0) == SQLITE_OK){
            return true;
        }
        return false;
    }

    void dispose()
    {
        if(stmt){
            sqlite3_finalize(stmt);
            stmt = null;
        }
    }

public://需要检查exec()是否成功
    bool set_int(int value)
    {
        if(sqlite3_bind_int(stmt, 1, value) == SQLITE_OK){
            sqlite3_step(stmt);
            return true;
        }
        return false;
    }

    int get_int(int allways = 0)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW){
            return sqlite3_column_int(stmt, 0);
        }
        return allways;
    }

    bool set_int64(int64_t value)
    {
        if(sqlite3_bind_int64(stmt, 1, value) == SQLITE_OK){
            sqlite3_step(stmt);
            return true;
        }
        return false;
    }

    int64_t get_int64(int64_t allways = 0)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW){
            return sqlite3_column_int64(stmt, 0);
        }
        return allways;
    }

    bool set_text(const wchar_t* text, size_t size)
    {
        if(sqlite3_bind_text16(stmt, 1, text, size * sizeof(wchar_t), NULL) == SQLITE_OK){
            sqlite3_step(stmt);
            return true;
        }
        return false;
    }

    const wchar_t* get_text()
    {
        if(sqlite3_step(stmt) == SQLITE_ROW){
            return (const wchar_t*)sqlite3_column_text16(stmt, 0);
        }
        return null;
    }

    bool blob_write(const void* data, size_t size)
    {
        if(sqlite3_bind_blob(stmt, 1, data, size, NULL) == SQLITE_OK){
            sqlite3_step(stmt);
            return true;
        }
        return false;
    }
};



class sql_blob
{
public:
    sqlite3* database;
    sqlite3_blob* blob;
    /*
    struct
    {
        char table[16];
        char name[16];
        int id;
    }state;
    */
    int seekpos;

public:
    sql_blob(sqlite3* db) : database(db), blob(null)
    {
        seekpos = 0;
    }
    ~sql_blob()
    {
        this->close();
    }

    bool open(const char* table, int id, const char* name)
    {
        if(sqlite3_blob_open(database, "main", table, name, id, 0, &blob) == SQLITE_OK){
            //cscpy(state.table, table, 16);
            //cscpy(state.name, name, 16);
            //state.id = id;
            return true;
        }
        return false;
    }

    void close()
    {
        if(blob){
            sqlite3_blob_close(blob);
            blob = null;
        }
    }

    int size()
    {
        if(blob){
            return sqlite3_blob_bytes(blob);
        }
        return 0;
    }

    int read(void* data, size_t size)
    {
        if(blob){
            return sqlite3_blob_read(blob, data, size, seekpos);
        }
        return 0;
    }

    /*
    int write(const void* data, size_t size)
    {
        sql_stmt stmt(database);
        char command[256];
        int n = snprintf(command, 256, "update %s set %s=? where id=%i;", state.table, state.name, state.id);
        if(stmt.exec(command, n)){
            if(stmt.write(data, size)){
                return size;
            }
        }
        return 0;
    }
    */

    int write(const void* data, size_t size)
    {
        if(blob){
            return sqlite3_blob_write(blob, data, size, seekpos);
        }
    }
};



class sql_database
{
public:
    typedef std::string string_type;
    typedef std::wstring wstring_type;

    enum{
        sqlc_size = 1024
    };

    typedef char sqlc[sqlc_size];

private:
    sqlite3* m_db;
    sqlc commandline; //命令行缓冲区

public:
    sql_database():m_db(0)
    {
    }
    ~sql_database()
    {
        close();
    }

    sqlite3* handle()
    {
        return m_db;
    }

    int open(const char *path)
    {
        this->close();
        int n = sqlite3_open(path, &m_db);
        if(n != SQLITE_OK)
        {
            this->close();
            return SQLITE_ERROR;
        }
        return SQLITE_OK;
    }

    int open(const wchar_t *path)
    {
        this->close();
        int n = sqlite3_open16(path, &m_db);
        if(n != SQLITE_OK)
        {
            this->close();
            return SQLITE_ERROR;
        }
        return SQLITE_OK;
    }

    void close()
    {
        if(m_db)
        {
            sqlite3_close(m_db);
            m_db = NULL;
        }
    }

    bool is_open()const
    {
        return m_db;
    }

    int create_table(const char *table, const char* fields)
    {
        //默认添加id主键
        this->command("create table if not exists %s (id integer primary key, %s);", table, fields);
        return this->execute();
    }

    int drop_table(const char *table)
    {
        this->command("drop table %s;", table);
        return this->execute();
    }

    int insert(const char* table, const char* set, const char* values)
    {
        this->command("insert into %s (%s) values (%s);", table, set, values);
        if(this->execute() == SQLITE_OK){
            return sqlite3_last_insert_rowid(m_db);
        }
        return 0;
    }

    int erase(const char* table, int id)
    {
        this->command("delete from %s where id=%i;", table, id);
        return this->execute();
    }

    //int select(const char *table, const char* any, const char* where)
    /*
    int insert(const char *table, const char* sets)
    {
        int n;
        //n = exec("begin transaction;");
        std::string c = "insert into ";
        c += table;
        c += " ";
        c += sets;
        n = this->exec(c.c_str());
        //n = exec("commit transaction;");
        return n;
    }

    int erase(const char* table, int id)
    {

    }
    // 更新操作
    int update(const char *table, const char* set, const char* where);

    int count(const char* table, const char* where = NULL);
    */

    int list(std::vector<int>& ls, const char* table, const char* where)
    {
        //"select id from table where ...";
        sqlite3_stmt *stmt;
        string_type state = "select id from ";
        state += table;
        if(where){
            state += " where ";
            state += where;
        }

        int result = sqlite3_prepare(m_db, state.c_str(), state.length(), &stmt, 0);
        if(result != SQLITE_OK){
            sqlite3_finalize(stmt);
            return result;
        }

        while(sqlite3_step(stmt) == SQLITE_ROW){
            //int count = sqlite3_data_count(stmt);
            //sqlite3_column_count(stmt);
            //int type = sqlite3_column_type(stmt, 0);
            int n = sqlite3_column_int(stmt, 0);
            ls.push_back(n);
        }
        sqlite3_finalize(stmt);
        return 0;
    }

    int set_int(const char* table, int id, const char* name, int value)
    {
        int n = this->command("update %s set %s=? where id=%i;", table, name, id);
        sql_stmt stmt(m_db);
        if(stmt.exec(commandline, n)){
            return stmt.set_int(value);
        }
        return 0;
    }

    int get_int(const char* table, int id, const char* name)
    {
        int n = this->command("select %s from %s where id=%i;", name, table, id);
        sql_stmt stmt(m_db);
        if(stmt.exec(commandline, n)){
            return stmt.get_int();
        }
        return 0;
    }

    int set_int64(const char* table, int id, const char* name, int64_t value)
    {
        int n = this->command("update %s set %s=? where id=%i;", table, name, id);
        sql_stmt stmt(m_db);
        if(stmt.exec(commandline, n)){
            return stmt.set_int64(value);
        }
        return 0;
    }

    int64_t get_int64(const char* table, int id, const char* name)
    {
        int n = this->command("select %s from %s where id=%i;", name, table, id);
        sql_stmt stmt(m_db);
        if(stmt.exec(commandline, n)){
            return stmt.get_int64();
        }
        return 0;
    }

    int set_text16(const char* table, int id, const char* name, const wchar_t* text, int length)
    {
        int n = this->command("update %s set %s=? where id=%i;", table, name, id);
        sql_stmt stmt(m_db);
        if(stmt.exec(commandline, n)){
            return stmt.set_text(text, length);
        }
        return 0;
    }

    //test 未完成
    wstring_type get_text16(const char* table, int id, const char* name)
    {
        int n = this->command("select %s from %s where id=%i;", name, table, id);
        sql_stmt stmt(m_db);
        if(stmt.exec(commandline, n)){
            return stmt.get_text();
        }
        return 0;
        /*
        sqlite3_stmt *stmt;
        const wchar_t* str = NULL;
        wstring_type wstr;

        string_type state = "select ";
        state += name;
        state += " from ";
        state += table;
        state += " where id=";
        state += to_string(id);
        int n = sqlite3_prepare(m_db, state.c_str(), state.length(), &stmt, 0);
        if(sqlite3_step(stmt) != SQLITE_ROW){
            sqlite3_finalize(stmt);
            mmlog<<"get text16 error."<<endline;
            return wstr;
        }
        if(n == SQLITE_OK){
            //int count = sqlite3_column_count(stmt);
            //int type = sqlite3_column_type(stmt, 0);
            str = (const wchar_t*)sqlite3_column_text16(stmt, 0);
            if(str)wstr.assign(str);
        }
        sqlite3_finalize(stmt);
        return wstr;
        */
    }

    int blob_size(const char* table, int id, const char* name)
    {
        sql_blob blob(m_db);
        if(blob.open(table, id, name)){
            return blob.size();
        }
        return 0;
    }

    int blob_read(const char* table, int id, const char* name, void* data, int size)
    {
        sql_blob blob(m_db);
        if(blob.open(table, id, name)){
            return blob.read(data, size);
        }
        return 0;
    }

    int blob_write(const char* table, int id, const char* name, const void* data, int size)
    {
        int n = this->command("update %s set %s=? where id=%i;", table, name, id);
        sql_stmt stmt(m_db);
        if(stmt.exec(commandline, n)){
            if(stmt.blob_write(data, size)){
                return size;
            }
        }
        return 0;
    }

    //写入数据，长度不能超过第一次写入的长度
    int blob_static_write(const char* table, int id, const char* name, const void* data, int size)
    {
        sql_blob blob(m_db);
        if(blob.open(table, id, name)){
            return blob.write(data, size);
        }
        return 0;
    }

    //事务操作
    void begin_update()
    {
        this->exec("BEGIN TRANSACTION;");
    }

    void end_update()
    {
        this->exec("COMMIT;");
    }

    //执行命令
    int exec(const char *command, sql_table* table = NULL)
    {
        if(is_open()){
            return sqlite3_exec(
                m_db,
                command,
                sql_callback,   //回调
                table,          //传入callback的参数
                NULL);          //errMsg
        }
        return SQLITE_ERROR;
    }

    //格式化命令
    int command(const char* param, ...)
    {
        va_list body;
        va_start(body, param);
        int n = STD::vsnprintf(commandline, sqlc_size, param, body);
        va_end(body);
        return n;
    }
    //执行
    int execute()
    {
        return this->exec(commandline);
    }

    const char* errmsg()const
    {
        if(!is_open()){
            return "database is not opened.";
        }
        const char* msg = sqlite3_errmsg(m_db);
        if(msg){
            return msg;
        }
        else{
            return "done.";
        }
    }

    const char* errstr(int n)const
    {
        return sqlite3_errstr(n);
    }

private:
    static int sql_callback(void* outdata, int columns, char **values, char **names);
};

int sql_database::sql_callback(void *outdata, int columns, char **values, char **names)
{
    sql_table* table = (sql_table*)(outdata);

    //list columns names
    if(table){
        if(columns && !table->columns.size()){
            for(int i=0; i<columns; ++i){
                table->columns.push_back(names[i]);
            }
        }
        table->rows.push_back(std::vector<std::string>());
    }

    //list values
    for(int i= 0 ; i < columns; ++i)
    {
        SQL_LOG("%s = %s", names[i], values[i] ? values[i] : "null");
        if(table){
            //if(table->name == name[i]){
                //result->values.push_back(argv[i] ? argv[i] : "null");
            table->rows.back().push_back(values[i] ? values[i] : "null");
            //}
        }
    }
    return 0;
}

}//end namespace cgl

//---------------------------------------------------------------------------
#endif //SQLITE_HPP_20180117023226
