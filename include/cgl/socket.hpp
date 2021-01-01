/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 socket.hpp

 2020-02-22 07:05:42

*/
#ifndef SOCKET_HPP_20200222070542
#define SOCKET_HPP_20200222070542

#include <stdio.h>
#include <winsock2.h>
#include <cgl/string.hpp>
#include <cgl/thread.hpp>

namespace cgl{


#ifndef CGL_MAX_CONNECTIONS
    #define CGL_MAX_CONNECTIONS 8
#endif

#ifndef CGL_SOCKET_BUFFER
    #define CGL_SOCKET_BUFFER 1024
#endif

//socket ������ʼ��
class socket_auto_init
{
public:
    socket_auto_init()
    {
        init();
    }

    ~socket_auto_init()
    {
        shutdown();
    }

    int init()
    {
        WORD sockVersion = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(sockVersion, &wsaData) != 0){
            return SOCKET_ERROR;
        }
        return 0;
    }

    void shutdown()
    {
        WSACleanup();
    }
};

CGL_PUBLIC_DECLARE socket_auto_init __cgl_socket_initialize = socket_auto_init();

//��ʽ��IP��ַ
string_t sockaddr_name(const void* addr)
{
    union{
        sockaddr    *a;
        sockaddr_in *b;
    };
    char buf[32] = {0};
    a = (sockaddr*)addr;
    snprintf(buf, 32, "%hhu-%hhu-%hhu-%hhu:%i", a->sa_data[2], a->sa_data[3], a->sa_data[4], a->sa_data[5], b->sin_port);
    return buf;
}

//���ö���ģʽ��falseΪ�Ƕ���ģʽ
void socket_block_mode(SOCKET id, bool value)
{
    ULONG n = value ? 0 : 1;
    ioctlsocket(id, FIONBIO, &n);
}

//��Ϣ����
enum SKID
{
    sk_null,
    sk_id,
    sk_name,
    sk_string,
    sk_unistring,
};

//���ݽṹ
struct socket_block
{
    uint32_t id;
    uint32_t size;
    byte_t   data[0];
};

class socket_client
{
protected:
    SOCKET m_socket;

public:
    string_t name;

public:
    socket_client() : m_socket(0), name("unnamed")
    {
    }

    SOCKET handle()const
    {
        return m_socket;
    }

    bool invalid()const
    {
        return m_socket && m_socket != INVALID_SOCKET;
    }

    string_t ip()
    {
        sockaddr addr;
        int n = sizeof(sockaddr);
        getsockname(m_socket, &addr, &n);
        return sockaddr_name(&addr);
    }

    //���ӷ�����
    int connect(const char* ip, int port)
    {
        m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == INVALID_SOCKET){
            m_socket = 0;
            CGL_LOG("invalid socket !");
            return SOCKET_ERROR;
        }

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = port;
        addr.sin_addr.S_un.S_addr = inet_addr(ip);

        if (::connect(m_socket, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR){
            CGL_LOG("connect error !");
            this->close();
            return SOCKET_ERROR;
        }

        this->send(sk_name, name.c_str(), name.length());
        this->send("��ã�TCP����ˣ�\n");

        socket_block_mode(m_socket, false);
        //m_thread.create(thread_recv, &m_socket);
        return 0;
    }

    //���տͻ�������
    int accept(SOCKET server)
    {
        sockaddr addr = {0};
        int size = sizeof(sockaddr);
        m_socket = ::accept(server, &addr, &size);
        if (m_socket == INVALID_SOCKET){
            m_socket = 0;
            //CGL_LOG("accept error !");
            return SOCKET_ERROR;
        }
        //client access
        CGL_LOG("���ӽ��룺%s", sockaddr_name(&addr).c_str());

        this->send("��ã�TCP�ͻ��ˣ�\n");

        //m_thread.create(thread_recv, &m_socket);
        socket_block_mode(m_socket, false);

        return 0;
    }

    //�ر�����
    void close()
    {
        if(m_socket){
            closesocket(m_socket);
            m_socket = 0;
        }
    }

    //������Ϣ
    //write
    void send(int id, const void* data, size_t size)
    {
        if(m_socket){
            socket_block block = {id, size};
            ::send(m_socket, (char*)&block, sizeof(block), 0);
            ::send(m_socket, static_cast<const char*>(data), size, 0);
        }
    }

    void send(const char* text, size_t size = TEXT_MAX)
    {
        if(size == TEXT_MAX)size = strlen(text);
        return this->send(sk_string, text, size);
    }

    void send(const wchar_t* text, size_t size = TEXT_MAX)
    {
        if(size == TEXT_MAX)size = wcslen(text);
        return this->send(sk_string, text, size * 2);
    }

    //�Ƕ���ģʽ�½�������
    //read
    int receive(void* data, int size)
    {
        if(!this->invalid()){
            return SOCKET_ERROR;
        }

        size = ::recv(m_socket, reinterpret_cast<char*>(data), size, 0);

        if(size == 0){//client disconnect
            CGL_LOG("�����Ѿ��Ͽ���");
            this->close();
        }
        else if(size < 0){
            check_error(size);
            size = 0;
        }

        return size;
    }

protected:
    static int thread_recv(cgl::thread* instance, void *arg)
    {
        SOCKET* socket = (SOCKET*)arg;
        char buffer[CGL_SOCKET_BUFFER];
        while(instance->is_running()){
            int size = ::recv(*socket, buffer, CGL_SOCKET_BUFFER, 0);
            if(size <= 0 || errno == EINTR){
                CGL_LOG("�����Ѿ��Ͽ���");
                *socket = 0;
                return SOCKET_ERROR;
            }
            else{
                buffer[size] = 0x00;
                CGL_LOG(buffer);
            }
        }
        return 0;
    }

    int check_error(int code)
    {
        int err = WSAGetLastError();
        switch(err){
        case WSAEWOULDBLOCK://10035
        case WSAEINPROGRESS://10036
            //CGL_LOG("WSAEINPROGRESS");
            break;
        case WSAENETDOWN://10050
            CGL_LOG("WSAENETDOWN");
            break;
        case WSAECONNRESET://10054�ͻ��˹ر�����
            CGL_LOG("�����Ѿ��Ͽ���");
            this->close();
            break;
        case 10053:
            break;
        case WSAESHUTDOWN://10058
            CGL_LOG("WSAESHUTDOWN");
            break;
        default:
            CGL_LOG("socket error : %i %i", m_socket, err);
            break;
        }

        #if 0//linux
        if(errno == EINTR /*|| errno == EWOULDBLOCK*/ || errno == EAGAIN){
            CGL_LOG("EINTR ");
        }
        else{
            //return SOCKET_ERROR;
            //print(m_socket, "err");
        }
        #endif

        return 0;
    }
};

class socket_server
{
protected:
    SOCKET m_socket;
    thread m_thread;

public:
    socket_client clients[CGL_MAX_CONNECTIONS];

public:
    socket_server() : m_socket(0), m_thread()
    {
        memset(clients, 0, sizeof(clients));
    }

    SOCKET handle()const
    {
        return m_socket;
    }

    //��������
    int create(const char* ip, int port)
    {
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == INVALID_SOCKET){
            m_socket = 0;
            CGL_LOG("server : socket() error !");
            return SOCKET_ERROR;
        }

        CGL_LOG("����������    %s : %i", ip, port);

        //����Ϊ�Ƕ���ģʽ
        socket_block_mode(m_socket, false);

        return listen(ip, port);
    }

    //�ر�����
    void close()
    {
        m_thread.stop();
        m_thread.wait();
        if(m_socket){
            closesocket(m_socket);
            m_socket = 0;
        }
    }

    //�㲥��Ϣ
    void send_all(const char* text)
    {
        for(int i=0; i<CGL_MAX_CONNECTIONS; ++i){
            if(clients[i].invalid()){
                clients[i].send(text);
            }
        }
    }

private:
    //�����˿�
    int listen(const char* ip, int port)
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = port;
        addr.sin_addr.S_un.S_addr = inet_addr(ip);

        if (bind(m_socket, (LPSOCKADDR)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR){
            CGL_LOG("server : bind() error !");
            return SOCKET_ERROR;
        }

        if (::listen(m_socket, 1) == SOCKET_ERROR){//������5��
            CGL_LOG("server : listen() error !");
            return SOCKET_ERROR;
        }

        m_thread.create(thread_listen_proc, this);
        return 0;
    }

    //�ȴ��������ͻ�����
    static int thread_listen_proc(cgl::thread* instance, void *arg)
    {
        socket_server* server = (socket_server*)arg;
        socket_client* client;
        while(instance->is_running()){
            client = null;
            for(int i=0; i < CGL_MAX_CONNECTIONS; ++i){
                if(!server->clients[i].invalid()){
                    client = &server->clients[i];
                    break;
                }
            }

            if(client){
                //CGL_LOG("�ȴ�����...");
                client->accept(server->handle());
            }

            Sleep(100);
        }
        return 0;
    }
};

typedef socket_block  skblock;
typedef socket_server skserver;
typedef socket_client skclient;

}//end namespace cgl

#endif //SOCKET_HPP_20200222070542

/*

#include <ws2tcpip.h>
#pragma link "ws2_32.lib"

int getname(char * name)
{
    char   *ptr, **pptr;
    struct hostent *hptr;
    char   str[32];


    if((hptr = gethostbyname(name)) == NULL)
    {
        printf(" gethostbyname error for host:%s\n", ptr);
        return 0;
    }

    printf("official hostname:%s\n",hptr->h_name);
    for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
        printf(" alias:%s\n",*pptr);

    switch(hptr->h_addrtype)
    {
        case AF_INET:
        case AF_INET6:
            pptr=hptr->h_addr_list;
            for(; *pptr!=NULL; pptr++)
                print("addr", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
            print(" first address: %s\n", inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
        break;
        default:
            printf("unknown address type\n");
        break;
    }

    return 0;
}

*/