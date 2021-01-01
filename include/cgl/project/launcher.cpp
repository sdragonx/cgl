/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 launcher.cpp

 2020-02-09 06:24:00

*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

const char* app = "launcher";

void help(void)
{
    printf("launcher 1.0 by sdragonx 2020-2-9\n");
    printf("  launcher [pipe.out|0] command ...\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    HANDLE handle;
    FILE*  pipe;
    DWORD  size;
    char   commandline[1024] = {0};
    struct{
        DWORD size;
        char  data[256];
    }buffer;

    //for(int i=0; i<argc; ++i){ printf("%i - %s\n", i, argv[i]); }
    //system("pause");

    // ��������У�����ڹܵ����������ת��ΪHANDLE����
    if (argc < 3){
        help();
    }

    handle = (HANDLE)atoi(argv[1]);

    //�ϲ�������
    for(int i=2; i<argc; ++i){
        strcat(commandline, argv[i]);
        strcat(commandline, " ");
    }

    // ʹ��_popen�����ӽ��̲��ض������׼������ļ�ָ����
    if( (pipe = _popen( commandline, "rt" )) == NULL ){
        printf("command start failed.\n");
        exit( 1 );
    }

    while( !feof( pipe ) ){
        if( fgets( buffer.data, 256, pipe ) != NULL ){
            if (handle){
                // ���ӽ��̵ı�׼���д��ܵ����ṩ���Լ��ĸ�����
                // ��ʽ����д���ݿ鳤�ȣ�0��ʾ����������д���ݿ�����
                buffer.size = strlen(buffer.data);
                WriteFile(handle, &buffer, buffer.size + 4, &size, NULL);
                printf("%s> write : %lu\n", app, buffer.size);
            }
            else{
                // ��������δ�ṩ�ܵ��������ֱ�Ӵ�ӡ���
                printf("%s> %s", app, buffer.data);
            }
        }
    }

     // д��0����ʾ�������ݶ���д��
    buffer.size = 0;
    if (handle){
        WriteFile(handle, &buffer.size, sizeof(DWORD), &size, NULL);
    }

    return _pclose( pipe );
}

#if 0

void command_launcher(const char* cmd)
{
    pipefile pipe;
    char buffer[1024];
    DWORD size;
    char *p;
    DWORD n = 0;

    pipe.create();
    snprintf(buffer, 1024, "launcher %i %s", (int)pipe.out(), cmd);
    CGL_LOG_DEBUG(buffer);

    PROCESS_INFORMATION pi = {0};
    STARTUPINFOA si = {0};
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfoA(&si);
    //si.hStdInput  = 0;
    si.hStdOutput   = 0;
    si.hStdError    = 0;
    si.wShowWindow  = SW_HIDE;
    si.dwFlags      = STARTF_USESHOWWINDOW;
    if (!CreateProcessA(NULL, buffer, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)){
        CGL_LOG("Error on CreateProcess()");
        return ;
    }

    while(true){
        Application->ProcessMessages();
        size = pipe.query();
        if(size){
            //CGL_LOG_DEBUG("query size = %u, n = %u", size, n);
            size = pipe.read(buffer, size);
            p = buffer;
            while(size){
                //CGL_LOG_DEBUG("size = %u, n = %u", size, n);
                if(n == 0){
                    if(size < 4)ShowMessage("err");
                    n = *((DWORD*)p);
                    if(n == 0){
                        n = INT16_MAX;
                        size = 0;
                        //CGL_LOG_DEBUG("n == 0 quit");
                    }
                    else{
                        p += 4;
                        size -= 4;
                    }
                }
                else if(n < size){
                    frmMain->txtMessage->Append(p, n);
                    p += n;
                    size -= n;
                    n = 0;
                }
                else{
                    frmMain->txtMessage->Append(p, size);
                    p += size;
                    n -= size;
                    size = 0;
                }
            }
            if(n == INT16_MAX){
                //CGL_LOG_DEBUG("while end");
                break;
            }
        }
    }
    //CGL_LOG_DEBUG("end");
    pipe.close();
}

#endif