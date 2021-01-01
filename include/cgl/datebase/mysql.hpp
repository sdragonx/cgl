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

//mysql���Զ���ʼ���͹ر�
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

//�����gdi+���Ǹ�һ�������߳�ģʽ����ν��
//__declspec(selectany) SQLDB_auto_init _mysql_auto_init = SQLDB_auto_init();

class SQLDB
{
public:
    const static TCHAR SPACE = ' ';//�ո�
    const static TCHAR COMMA = ',';//����
    typedef std::string String;

private:
    MYSQL mysql;//���ݿ����ָ��
public:
    //String Msg;//���ڱ��浱ǰ����������Ϣ
public:
    /*
    ���캯����ϡ������
    */
    SQLDB() //: Msg()
    {
        memset(&mysql, 0, sizeof(MYSQL));
    }

    ~SQLDB()
    {
        this->close();
    }

    //�������ݿ����ָ�룬��������������
    MYSQL* handle()
    {
        return &mysql;
    }

    //�������ݿ⣬����true�ɹ�
    bool connect(
        const char* host,        //������IP
        const char* user,        //�û�
        const char* password,    //����
        const char* database,    //���ݿ�����
        int port = 3306)                //���ݿ�˿�
    {
        if(this->is_connected()){
            //������ݿ��Ѿ��򿪣����Թر�
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

    //�������ݿ���룬Ĭ��GBK����������UTF8��
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

    //�ر����ݿ�����
    void close()
    {
        if(this->is_connected()){//����Ƿ�Ϊ��
            mysql_close(this->handle());//�ر����ݿ�
            memset(&mysql, 0, sizeof(MYSQL));//��ն���
        }
    }

    //�ж����ݿ�����Ƿ����ӳɹ�
    bool is_connected()
    {
        return mysql.db;
    }

    //�������ݿ������Ϣ
    String error()
    {
        if(this->is_connected()){
            return mysql_error(this->handle());
        }
        return String();
    }

    //ִ��һ��SQL���
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
    ��Ҫ�Ĺ��ܣ�
    ��ѯ����

    ��ڲ�����
    SQL����ѯ��SQL���
    Cnum:��ѯ������
    Msg:���ص���Ϣ������������Ϣ

    ���ڲ�����
    string ׼�����÷��ص����ݣ�������¼����0x06����,�����λ��0x05����
    ��� ���صĳ��ȣ� 0�����ʾ����
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

    //����Ƿ�װ�Ĳ����Ĳ�����������Լ��ٸĸ�
    //mysql_query(&mysqlcon,"insert into mls   (xiahuajiao,fangweijiao,xiahuacha,fangweicha,devicestatus,hangdaono,recordtime)
    //values (1.05, 1 , 1.08, 1.9,'success','660',now() )");
    bool insert(String table,//����
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
    ��Ҫ���ܣ�
    �޸�����
    */
    int update(String SQL);


    /*
    ��Ҫ���ܣ�
    ɾ������
    */
    int del(String SQL);

};


#endif //SQLDB_HPP_20181219111945
