/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 uuid.h

 2006-12-27 22:56:36

 //std uuid 8-4-4-4-12
 //uuid 8-4-4-16

 revise  2020/1/31 22:04

*/
#ifndef UUID_H_20061227225636
#define UUID_H_20061227225636

#include <cstdio>

#if defined(_WIN32) || defined(_WIN64)
    #include <objbase.h>    //rpcdce.h uuidcreate
    #if defined (_MSC_VER)
        #pragma comment(lib, "ole32.lib")
    #endif
#elif defined(__linux__) || defined(__unix__)
    #include <uuid/uuid.h>
    //uuid_unparse(in uuid, out str);
    //uuid_parse(in str, out uuid);
#elif defined( __APPLE__ )
    #include <CoreFoundation/CFUUID.h>
#endif

#if defined(__LINUX__) || defined(__linux__) || defined(__ANDROID__)
/*
FILE* f = fopen( "/proc/sys/kernel/random/uuid", "rb" );
    if( f == 0x0 )
        return;

    char uuid_str[36];
    size_t read = fread( uuid_str, 1, sizeof( uuid_str ), f );
    fclose(f);
    if( read != 36 )
        return;
    uuid_from_string( uuid_str, res );
*/
#endif

namespace cgl{

#pragma pack(push, 1)

class uuid
{
public:
    typedef uuid this_type;
    const static int DATA_SIZE = 16;
    union{
        uint8_t data[DATA_SIZE];
        struct{
            uint32_t alpha;
            uint16_t gamma;
            uint16_t beta;
            uint8_t  delta[8];
        };
    };

public:
    uuid()
    {
        memset(&data, 0, DATA_SIZE);
    }

    bool is_null()const
    {
        uint32_t n = 0;
        n |= alpha;
        n |= beta;
        n |= gamma;
        for(int i=0; i<DATA_SIZE; ++i){
            n |= delta[i];
        }
        return !n;
    }

    void create()
    {
        #if defined(_WIN32) || defined(_WIN64)
            CoCreateGuid(reinterpret_cast<GUID*>(data));
        #elif defined(__linux__) || defined(__unix__)
            uuid_generate(data);
        #elif defined( __APPLE__ )
            CFUUIDRef new_uuid = CFUUIDCreate(0x0);
            CFUUIDBytes bytes = CFUUIDGetUUIDBytes( new_uuid );
            // typedef struct {
            //   UInt8 byte0;
            //   UInt8 byte1;
            //   ...
            //   UInt8 byte15;
            // } CFUUIDBytes;
            memcpy(data, (uint8_t*)&bytes, DATA_SIZE);
            CFRelease ( new_uuid );
        #endif
    }

    char* format(char* buf, size_t size, bool guid_style = false)
    {
        using namespace std;
        //min length = 40
        const char* param = guid_style ?
            "{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}":
            "%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX";
        snprintf(buf, size, param,
            alpha, beta, gamma,
            delta[0], delta[1], delta[2], delta[3],
            delta[4], delta[5], delta[6], delta[7]);
        return buf;
    }

    char* source_code(char* buf, size_t size)
    {
        using namespace std;
        //min length = 40
        const char* param = "const static GUID <name> =\r\n{0x%08X, 0x%04hX, 0x%04hX, {0x%02hhX, 0x%02hhX, 0x%02hhX, 0x%02hhX, 0x%02hhX, 0x%02hhX, 0x%02hhX, 0x%02hhX} };";
        snprintf(buf, size, param,
            alpha, beta, gamma,
            delta[0], delta[1], delta[2], delta[3],
            delta[4], delta[5], delta[6], delta[7]);
        return buf;
    }

    int parse(const char* str)
    {
        using namespace std;
        if(strlen(str)!=36)return -1;
        sscanf(str, "%x-%hx-%hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
            &alpha, &beta, &gamma,
            &delta[0], &delta[1], &delta[2], &delta[3],
            &delta[4], &delta[5], &delta[6], &delta[7]);
        return 0;
    }

    bool is_equal(const this_type& other)const
    {
        return !memcmp(data, &other.data, DATA_SIZE);
    }

    bool operator==(const this_type& other)const
    {
        return is_equal(other);
    }

    bool operator!=(const this_type& other)const
    {
        return !is_equal(other);
    }
};

#pragma pack(pop)

}//end namespace cgl

#endif //UUID_H_20061227225636
