/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 example.h

 2019-12-28 10:48:16

*/
#ifndef SQLDB_HPP_20181219111945
#define SQLDB_HPP_20181219111945

#define VCL_FULL
#define STRICT

#include <stdio.h>
#include <string>
#include <winsock.h>
#include <Windows.h>
#include <strstream>

//#include <vcl.h>

#define MYSQL_ABI_CHECK

#include "include/mysql.h"

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")

template<typename T>
std::string to_string(const T& value)
{
    std::strstream stm;
    stm<<value;
    return stm.str();
}

void print(const char* str);

//mysql库自动初始化和关闭
class SQLDB_auto_init
{
public:
    SQLDB_auto_init()
    {
        mysql_library_init(0, NULL, NULL);
    }

    ~SQLDB_auto_init()
    {
        mysql_library_end();
    }
};

//这个和gdi+的那个一样。单线程模式无所谓。
//__declspec(selectany) SQLDB_auto_init _mysql_auto_init = SQLDB_auto_init();

class SQLDB
{
public:
    const static TCHAR SPACE = ' ';//空格
    const static TCHAR COMMA = ',';//逗号
    typedef std::string String;

private:
    MYSQL mysql;//数据库变量指针
public:
    //String Msg;//用于保存当前操作返回信息
public:
    /*
    构造函数和稀构函数
    */
    SQLDB() //: Msg()
    {
        memset(&mysql, 0, sizeof(MYSQL));
    }

    ~SQLDB()
    {
        this->close();
    }

    //返回数据库操作指针，这个不可随意操作
    MYSQL* handle()
    {
        return &mysql;
    }

    //链接数据库，返回true成功
    bool connect(
        const char* host,        //服务器IP
        const char* user,        //用户
        const char* password,    //密码
        const char* database,    //数据库名称
        int port = 3306)                //数据库端口
    {
        if(this->is_connected()){
            //如果数据库已经打开，尝试关闭
            this->close();
        }

        if( mysql_init(this->handle()) == NULL )
        {
            ///Msg = "init mysql handle error";
            return false;
        }

        if(mysql_real_connect(this->handle(), host, user, password, database, port, NULL, 0) == NULL)
        {
            //Msg = "failed to connect to database: Error";
            //ShowMessage(this->error());
            return false;
        }

        this->character_set("UTF8");
        return true;
    }

    //设置数据库编码，默认GBK，可以设置UTF8等
    bool character_set(const char* char_set = "GBK")
    {
        if(!this->is_connected()){
            //Msg = "database is not connected.";
            return false;
        }

        if(mysql_set_character_set(this->handle(), char_set) != 0)
        {
            //Msg = "mysql_set_character_set Error";
            return false;
        }
        return true;
    }

    //关闭数据库连接
    void close()
    {
        if(this->is_connected()){//检查是否为空
            mysql_close(this->handle());//关闭数据库
            memset(&mysql, 0, sizeof(MYSQL));//清空对象
        }
    }

    //判断数据库对象是否链接成功
    bool is_connected()
    {
        return mysql.db;
    }

    //返回数据库错误信息
    String error()
    {
        if(this->is_connected()){
            return mysql_error(this->handle());
        }
        return String();
    }

    //执行一条SQL语句
    bool exec(String SQL)
    {
        if(!this->is_connected()){
            //Msg = "database is not connected.";
            return false;
        }

        if(mysql_query(this->handle(), SQL.c_str()) != 0){
            //Msg = this->error();
            return false;
        }
        return true;
    }

    /*
    主要的功能：
    查询数据

    入口参数：
    SQL：查询的SQL语句
    Cnum:查询的列数
    Msg:返回的消息，包括错误消息

    出口参数：
    string 准备放置返回的数据，多条记录则用0x06隔开,多个栏位用0x05隔开
    如果 返回的长度＝ 0，责表示舞结果
    */
    bool select(String table, int column, float& value)
    {
        if(!this->is_connected()){
            //Msg = "database is not connected.";
            return false;
        }

        /*
        MYSQL_ROW m_row;
        MYSQL_RES *m_res;

        String sql = "select * from ";
        sql += table;
        sql += 

        if(mysql_query(mysql,sql) != 0)
        {
            Msg = "select ps_info Error";
            return "";
        }
        m_res = mysql_store_result(mysql);

        if(m_res==NULL)
        {
            Msg = "select username Error";
            return "";
        }
        String str("");
        while(m_row = mysql_fetch_row(m_res))
        {
            for(int i = 0; i < column; ++i)
            {
                 str += m_row[i];
                 str += rg;
            }
            str += rg;
            rnum++;
        }

        mysql_free_result(m_res);
        */
        return false;
    }

    //这个是封装的插入表的操作，你可以自己再改改
    //mysql_query(&mysqlcon,"insert into mls   (xiahuajiao,fangweijiao,xiahuacha,fangweicha,devicestatus,hangdaono,recordtime)
    //values (1.05, 1 , 1.08, 1.9,'success','660',now() )");
    bool insert(String table,//表名
        float hangxianghz, float fangweijiao, float xiahuacha, float fangweicha, String status, String hangdaono)
    {
        if(!this->is_connected()){
//            Msg = "database is not connected.";
            return false;
        }

        char sql[4096] = {0};
        snprintf(sql, 4096, "insert into %s (xiahuajiao,fangweijiao,xiahuacha,fangweicha,devicestatus,hangdaono,recordtime) values (%f,%f,%f,%f,\'%s\',\'%s\',now())",
            table.c_str(), hangxianghz, fangweijiao, xiahuacha, fangweicha, status.c_str(), hangdaono.c_str());

        print(sql);

        return this->exec(sql);
    }

    /*
    主要功能：
    修改数据
    */
    int update(String SQL);


    /*
    主要功能：
    删除数据
    */
    int del(String SQL);

};


#endif //SQLDB_HPP_20181219111945
