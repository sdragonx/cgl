/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 filetype.hpp

 2017-01-20 17:17:38

 根据文件数据判断文件类型，根据文件扩展名判断文件类型

 filesystem::ft_txt

 application/exe
 image/png
 audio/wav
 video/mp4

 filetype_fromExt
 扩展名不包括. 扩展名编码是ansi(utf8)

 filesystem::ft_text
 ft_link
 ft_exe
 ft_avi

 data_type

 struct data_info
{
    ftype_t type;
    tchar_t* name;

    struct{
        byte_t data[];
        size_t   check_size;
    }check[];
};

file sn:
//00 01 42 44 ... 0 DBA Palm Desktop To Do Archive 
00 01 44 54 ... 0 TDA Palm Desktop Calendar Archive 

05 07 00 00 42 4F 42 4F
05 07 00 00 00 00 00 00
00 00 00 00 00 01 ....BOBO............ 0 cwk AppleWorks 5 document 
06 07 E1 00 42 4F 42 4F
06 07 E1 00 00 00 00 00
00 00 00 00 00 01 ....BOBO............ 0 cwk AppleWorks 6 document 

46 4F 52 4D nn nn nn nn 38 53 56 58 FORM....8SVX 0, any 8svx, 8sv, svx, snd, iff IFF 8-Bit Sampled Voice 
46 4F 52 4D nn nn nn nn 41 43 42 4D FORM....ACBM 0, any acbm, iff Amiga Contiguous Bitmap 
46 4F 52 4D nn nn nn nn 41 49 46 46 FORM....AIFF 0, any aiff, aif, aifc, snd, iff Audio Interchange File Format 
46 4F 52 4D nn nn nn nn 41 4E 42 4D FORM....ANBM 0, any anbm, iff IFF Animated Bitmap 
46 4F 52 4D nn nn nn nn 41 4E 49 4D FORM....ANIM 0, any anim, iff IFF CEL Animation 
46 4F 52 4D nn nn nn nn 43 4D 55 53 FORM....CMUS 0, any cmus, mus, iff IFF Musical Score 
46 4F 52 4D nn nn nn nn 46 41 4E 54 FORM....FANT 0, any iff Amiga Fantavision Movie 
46 4F 52 4D nn nn nn nn 46 41 58 58 FORM....FAXX 0, any faxx, fax, iff IFF Facsimile Image 
46 4F 52 4D nn nn nn nn 46 54 58 54 FORM....FTXT 0, any ftxt, txt, iff IFF Formatted Text 
46 4F 52 4D nn nn nn nn 49 4C 42 4D FORM....ILBM 0, any ilbm, lbm, ibm, iff IFF Interleaved Bitmap Image 
46 4F 52 4D nn nn nn nn 53 4D 55 53 FORM....SMUS 0, any smus, smu, mus, iff IFF Simple Musical Score 
46 4F 52 4D nn nn nn nn 59 55 56 4E FORM....YUVN 0, any yuvn, yuv, iff IFF YUV Image 



*/
#ifndef FILE_TYPE_HPP_20170120171738
#define FILE_TYPE_HPP_20170120171738

#include <cgl/string.hpp>
#include <cgl/hash.hpp>

namespace cgl{
namespace filesystem{

typedef uint64_t ftype_t;

template<typename T>cstring<T> extract_file_ext(const cstring<T>&);


const uint64_t ft_unknown = 0;
const uint64_t ft_error   = -1;

//application
const uint64_t FILE_TYPE_MZ         = 0x49AED5482BDE468Dull;
const uint64_t FILE_TYPE_PE         = 0x431D966872D4E3B1ull;
const uint64_t FILE_TYPE_BIN        = 0xF737FEB85747F6D2ull;
const uint64_t FILE_TYPE_EXE        = 0xF9F4E666A105AD87ull;
const uint64_t FILE_TYPE_LNK        = 0xA68902EFA5899B29ull;
const uint64_t FILE_TYPE_LIB        = 0x8E32A2361BE83FC5ull;
const uint64_t FILE_TYPE_DLL        = 0x19796848A3D7EEE2ull;
const uint64_t FILE_TYPE_SO         = 0xBC9AA6CEAC286B74ull;
const uint64_t FILE_TYPE_A          = 0x716E41E3DFF50B85ull;
const uint64_t FILE_TYPE_MACH_O     = 0x9FC7833DA02AA595ull;    //mach-o, OS X and iOS Executable
const uint64_t FILE_TYPE_APK        = 0xD1EF8099856A7C70ull;    //android app
const uint64_t FILE_TYPE_DEX        = 0x5BEB8FA426D50B85ull;    //android Dalvik Executable
const uint64_t FILE_TYPE_ELF        = 0x73536AE0797BC99Dull;    //Linux Executable and Linkable Format
const uint64_t FILE_TYPE_VCLOBJECT  = 0xA01AFE120A9FC031ull;    //VCL Object Binary

//document
const uint64_t FILE_TYPE_TXT        = 0x2FB8A04868D57ED5ull;

const uint64_t FILE_TYPE_UTF8       = 0x73352FC1DA925785ull;
const uint64_t FILE_TYPE_UTF16      = 0xB27C29D2179A7DFAull;    //utf16LE
const uint64_t FILE_TYPE_UTF16BE    = 0xD832F39C2C9D2CFBull;    //utf16BE
const uint64_t FILE_TYPE_UTF32      = 0x33D2344DF88BFA13ull;    //utf32LE
const uint64_t FILE_TYPE_UTF32BE    = 0x5BDCDB85E5651825ull;    //utf32BE

const uint64_t FILE_TYPE_CAD        = 0x455123C67BF0A90Cull;
const uint64_t FILE_TYPE_CHM        = 0x83C39A251C9A48FCull;
const uint64_t FILE_TYPE_CLASS      = 0x0399A3DD81DED704ull;    //java class
const uint64_t FILE_TYPE_CRX        = 0xB7B6E4F927263A69ull;    //Google Chrome extension
const uint64_t FILE_TYPE_DBA        = 0xCFCBD1DB34CA61E7ull;    //Palm Desktop Calendar Archive
const uint64_t FILE_TYPE_DEB        = 0xF15987B3D013F4B5ull;    //Debian Software Packet
const uint64_t FILE_TYPE_DOC        = 0xE648934B5E6F37C4ull;    //Microsoft Office 2003 .doc;.xsl;.ppt
const uint64_t FILE_TYPE_EML        = 0xDA5FC272EA6EAE88ull;    //Email [thorough only]
const uint64_t FILE_TYPE_EOT        = 0xE7FDFFE4808127CCull;    //EOT font
const uint64_t FILE_TYPE_EPS        = 0xDFCC929F0EB291DAull;    //Postscript .eps .ps
const uint64_t FILE_TYPE_FH8        = 0x9B0B1D41F9CE0FD2ull;    //FreeHand 8 document
const uint64_t FILE_TYPE_FITS       = 0x9F71673381311DA4ull;    //FITS (Flexible Image Transport System)
const uint64_t FILE_TYPE_GHO        = 0x7BBA4CCC3A085316ull;    //ghost image
const uint64_t FILE_TYPE_MDB        = 0xC3219DD1CC5116BEull;    //microsoft access .mdb
const uint64_t FILE_TYPE_MSI        = 0x240E4E4924E2B561ull;
const uint64_t FILE_TYPE_NES        = 0x11AFEEEE44F0D9A2ull;    //Nintendo Entertainment System ROM file
const uint64_t FILE_TYPE_OTF        = 0x567F23BBE4F0FDBEull;
const uint64_t FILE_TYPE_RPM        = 0x38BE563CFE5FC43Cull;
const uint64_t FILE_TYPE_RTF        = 0x08216EECB089C11Bull;
const uint64_t FILE_TYPE_SWF        = 0x38821E70E6D8FB54ull;
const uint64_t FILE_TYPE_SQLITE3    = 0x78C10F270FC8EA1Eull;
const uint64_t FILE_TYPE_TORRENT    = 0x1B62ABD591D61893ull;    //Torrent file
const uint64_t FILE_TYPE_TTF        = 0xD54D7E59EBA811A0ull;
const uint64_t FILE_TYPE_PDF        = 0x4D1A6892F735869Bull;
const uint64_t FILE_TYPE_PWL        = 0xA21533BD0BA3C51Aull;    //Windows Password .pwl
const uint64_t FILE_TYPE_WOFF       = 0x4A132348B51BAB56ull;    //WOFF font
const uint64_t FILE_TYPE_WOF2       = 0x75A5261CAEEA267Eull;    //WOFF font
const uint64_t FILE_TYPE_WPD        = 0x8356EC80BA5A5526ull;    //WordPerfect .wpd
const uint64_t FILE_TYPE_QDF        = 0x50F5B8B4E9341606ull;    //Quicken .qdf

//font
const uint64_t FILE_TYPE_FON        = 0xD8E0727A9225B129ull;

//archive
const uint64_t FILE_TYPE_7Z         = 0x5C6AEFD1F04F84E1ull;
const uint64_t FILE_TYPE_AR         = 0x27606BA0190AAB43ull;
const uint64_t FILE_TYPE_ARJ        = 0x052D58F81AF894ADull;
const uint64_t FILE_TYPE_BZ2        = 0x92D5F478D5A92D15ull;
const uint64_t FILE_TYPE_CAB        = 0x65849F0D887D5F0Eull;
const uint64_t FILE_TYPE_DMG        = 0x3849AF305E92A60Bull;
const uint64_t FILE_TYPE_GZ         = 0x5E025BBF812C90CAull;
const uint64_t FILE_TYPE_ISO        = 0xEEA92FB3679B01BFull;    //ISO9660 CD/DVD image file
const uint64_t FILE_TYPE_JAR        = 0x0F4691A1C40E1A01ull;    //contain a manifest file "META-INF/MANIFEST.MF"
const uint64_t FILE_TYPE_LHA        = 0x5CC7D0D045923A4Full;    //LHA Compressed
const uint64_t FILE_TYPE_LZ         = 0xDC5439334DCDC15Cull;
const uint64_t FILE_TYPE_RAR        = 0x8194BFCC9681D998ull;
const uint64_t FILETYPE_SDI         = 0x71E5DA869B1A33E0ull;    //System Deployment Image, a disk image format used by Microsoft
const uint64_t FILE_TYPE_TAR        = 0xE96442FCC13B9FE8ull;    //tar archive
const uint64_t FILE_TYPE_TOAST      = 0xB1F269C65579C646ull;    //Roxio Toast disc image file, also some .dmg-files begin with same bytes
const uint64_t FILE_TYPE_VMDK       = 0x4FDED5B66922FC39ull;
const uint64_t FILE_TYPE_WIM        = 0x078E00D9D87CE35Full;    //Microsoft Windows Image
const uint64_t FILE_TYPE_XAR        = 0xE48D3A37B9E0DE50ull;    //eXtensible ARchive format
const uint64_t FILE_TYPE_XZ         = 0xF8A93C8CB1E61AC3ull;
const uint64_t FILE_TYPE_Z          = 0xAF669A044E0E61DEull;    //z, tar.z
const uint64_t FILE_TYPE_ZIP        = 0xD52F5139A270F51Eull;    //zip
const uint64_t FILE_TYPE_ZLIB       = 0x4028BF5230B1717Cull;    //zlib stream

//image
const uint64_t FILE_TYPE_AI         = 0xE0095A9937BAF049ull;
const uint64_t FILE_TYPE_ANI        = 0x89968C963FA0CF15ull;
const uint64_t FILE_TYPE_BMP        = 0x60720D6AD62C7ABEull;
const uint64_t FILE_TYPE_CIN        = 0x00092490C1ACEDE8ull;    //Kodak Cineon image
const uint64_t FILE_TYPE_CUR        = 0x93E8BB959368C1ABull;
const uint64_t FILE_TYPE_DDS        = 0x7925D7DBBFFD0C3Eull;
const uint64_t FILE_TYPE_DPX        = 0x6C89E42CC2E3956Full;    //SMPTE DPX image
const uint64_t FILE_TYPE_EXR        = 0x81662B591907E9BFull;    //OpenEXR image
const uint64_t FILE_TYPE_GIF        = 0xBAC570FAF74A86C7ull;
const uint64_t FILE_TYPE_ICO        = 0xCBB1402BEC3253FBull;
const uint64_t FILE_TYPE_JPG        = 0x23B2AB57236EF725ull;
const uint64_t FILE_TYPE_JP2        = 0xEBBF01991374543Eull;
const uint64_t FILE_TYPE_PCX        = 0xA6C7E868B0CC1669ull;
const uint64_t FILE_TYPE_PNG        = 0xF28E48FCD55EA4E6ull;
const uint64_t FILE_TYPE_PSD        = 0xBDF729DA980D7D28ull;
const uint64_t FILE_TYPE_TGA        = 0x962D1C5AFCA2FC23ull;
const uint64_t FILE_TYPE_TIFF       = 0xC7DD6DD3A6F9FFA0ull;
const uint64_t FILE_TYPE_WEBP       = 0xF42C204DE63129A4ull;

//media
const uint64_t FILE_TYPE_RIFF       = 0xF4695074C74A6F6Aull;    //unknown RIFF format

//audio
const uint64_t FILE_TYPE_AU         = 0x6D327C948FC0A503ull;
const uint64_t FILE_TYPE_FLAC       = 0x5463C4DC49D5B7D8ull;
const uint64_t FILE_TYPE_MIDI       = 0x6BDACC7396B60A83ull;    //midi mid
const uint64_t FILE_TYPE_M4A        = 0x525A9BDDA92EF2F9ull;
const uint64_t FILE_TYPE_MP3        = 0x3F803E5CAC00F7D4ull;
const uint64_t FILE_TYPE_OGG        = 0x1715344CC451991Aull;
const uint64_t FILE_TYPE_RAM        = 0xDDA53C677F5BC103ull;    //Real Audio
const uint64_t FILE_TYPE_WAV        = 0x378E0C6BA5F41D1Dull;

//video
const uint64_t FILE_TYPE_3GP        = 0x690FD7F8CFAE11E5ull;    //3rd Generation Partnership Project
const uint64_t FILE_TYPE_ASF        = 0x5D30D262613ACDB8ull;    //Windows Media .asf .wma, .wmv
const uint64_t FILE_TYPE_ASP        = 0x1C34707DAD01DB02ull;
const uint64_t FILE_TYPE_AVI        = 0x0B0127D20100632Eull;
const uint64_t FILE_TYPE_FLV        = 0xB00B36885B252070ull;
const uint64_t FILE_TYPE_MKV        = 0x6E36F5D077830DE9ull;
const uint64_t FILE_TYPE_MOV        = 0x548A622AADCF59C0ull;    //Quicktime
const uint64_t FILE_TYPE_MP4        = 0x9141A5222544E841ull;
const uint64_t FILE_TYPE_MPEG       = 0x4CFE5F32190C2522ull;    //Moving Picture Experts Group .mpeg .mpg
const uint64_t FILE_TYPE_RM         = 0x961186C4003498AFull;
const uint64_t FILE_TYPE_RMVB       = 0x0960B5C3E408ED74ull;
const uint64_t FILE_TYPE_WEBM       = 0x668B3D83D3999611ull;

class filetype
{
private:
    typedef bool (*function_t)(const byte_t*, size_t);
    struct type{
        uint64_t id;
        const char* name;
        const char* desc;
        function_t func;
        bool operator==(const ftype_t tpid)const { return id == tpid; }
    };
    static std::vector<type> typelist;

public:
    static ftype_t check(const byte_t* data, size_t size);
    static ftype_t check(std::FILE* file);
    static ftype_t check(std::FILE* file, byte_t* data);
    static bool reg_type(ftype_t id, const char* name, const char* desc, function_t func);
    static const char* name(ftype_t id);

private:
    static const int is_init;
    static int init();
};

std::vector<filetype::type> filetype::typelist = single< std::vector<filetype::type> >::value;
const int filetype::is_init = filetype::init();

bool filetype::reg_type(ftype_t id, const char* name, const char* desc, function_t func)
{
    type t;
    t.id = id;
    t.name = name;
    t.desc = desc;
    t.func = func;
    typelist.push_back(t);
    return true;
}

ftype_t filetype::check(const byte_t* data, size_t size)
{
    for(size_t i=0; i<typelist.size(); ++i){
        if(typelist[i].func && typelist[i].func(data, size)){
            return typelist[i].id;
        }
    }
    return 0;
}

ftype_t filetype::check(std::FILE* file)
{
    byte_t buf[1024] = {0};
    fseek(file, 0, SEEK_SET);
    int size = fread(buf, 1, 1024, file);
    return check(buf, size);
}

const char* filetype::name(ftype_t id)
{
    std::vector<type>::iterator itr = std::find(typelist.begin(), typelist.end(), id);
    if(itr != typelist.end()){
        return itr->name;
    }
    return NULL;
}

#ifndef _WINDOWS_
typedef struct _IMAGE_DOS_HEADER {          // DOS .EXE header
    uint16_t   e_magic;                     // Magic number
    uint16_t   e_cblp;                      // Bytes on last page of file
    uint16_t   e_cp;                        // Pages in file
    uint16_t   e_crlc;                      // Relocations
    uint16_t   e_cparhdr;                   // Size of header in paragraphs
    uint16_t   e_minalloc;                  // Minimum extra paragraphs needed
    uint16_t   e_maxalloc;                  // Maximum extra paragraphs needed
    uint16_t   e_ss;                        // Initial (relative) SS value
    uint16_t   e_sp;                        // Initial SP value
    uint16_t   e_csum;                      // Checksum
    uint16_t   e_ip;                        // Initial IP value
    uint16_t   e_cs;                        // Initial (relative) CS value
    uint16_t   e_lfarlc;                    // File address of relocation table
    uint16_t   e_ovno;                      // Overlay number
    uint16_t   e_res[4];                    // Reserved words
    uint16_t   e_oemid;                     // OEM identifier (for e_oeminfo)
    uint16_t   e_oeminfo;                   // OEM information; e_oemid specific
    uint16_t   e_res2[10];                  // Reserved words
    uint32_t   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
#endif

bool check_windows_pe(const byte_t* data, size_t size)
{
    PIMAGE_DOS_HEADER pdos = (PIMAGE_DOS_HEADER)data;
    data += pdos->e_lfanew;
    if(size < size_t(pdos->e_lfanew +2)){
        return false;
    }
    return data[0] == 0x50 && data[1] == 0x45;    //PE
}

bool check_dos_mz(const byte_t* data, size_t size)
{
    //MZ
    return data[0] == 0x4D && data[1] == 0x5A;
}

bool check_windows_link(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4C, 0x00, 0x00, 0x00, 0x01, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_dalvik(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x64, 0x65, 0x78, 0x0A, 0x30, 0x33, 0x35, 0x00};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_linux_elf(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x7F, 0x45, 0x4C, 0x46};    //.ELF
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_java_class(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xCA, 0xFE, 0xBA, 0xBE};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

//Mach-O binary (reverse byte ordering scheme, 32-bit)
bool check_mach_o(const byte_t* data, size_t size)
{
    if(size < 4)return false;

    byte_t mach_o32r[] = {0xCE, 0xFA, 0xED, 0xFE};    //Mach-O binary reverse 32-bit
    byte_t mach_o32[]  = {0xFE, 0xED, 0xFA, 0xCE};    //Mach-O binary 32-bit, offset typically 0x1000
    byte_t mach_o64r[] = {0xCF, 0xFA, 0xED, 0xFE};    //Mach-O binary reverse 64-bit
    byte_t mach_o64[]  = {0xFE, 0xED, 0xFA, 0xCF};    //Mach-O binary 64-bit, offset typically 0x1000
    return !memcmp(mach_o32r, data, 4) || !memcmp(mach_o32, data, 4) ||
        !memcmp(mach_o64r, data, 4) || !memcmp(mach_o64, data, 4);
}

bool check_vcl_object_binary(const byte_t* data, size_t size)
{
    byte_t buf[] = { 0x54, 0x50, 0x46, 0x30 }; //TPF0
    if(size < 4)return false;
    return !memcmp(buf, data, 4);
}

bool check_text_utf8(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xEF, 0xBB, 0xBF};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_text_utf32(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xFF, 0xFE, 0x00, 0x00};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_text_utf32be(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x00, 0x00, 0xFE, 0xFF};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_text_utf16(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xFF, 0xFE};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_text_utf16be(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xFE, 0xFF};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_cad(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x41, 0x43, 0x31, 0x30 };
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_ms_chm(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x49, 0x54, 0x53, 0x46};    //ITSF
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_goole_crx(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x43, 0x72, 0x32, 0x34};    //Cr24
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_dba(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xBE, 0xBA, 0xFE, 0xCA};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

//need before ar check
bool check_deb(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x21, 0x3C, 0x61, 0x72, 0x63, 0x68, 0x3E,//ar
        0x0A, 0x64, 0x65, 0x62, 0x69, 0x61, 0x6E, 0x2D, 0x62, 0x69, 0x6E, 0x61, 0x72, 0x79};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_ms_office(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xD0, 0xCF, 0x11, 0xE0};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_ms_email(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x44, 0x65, 0x6C, 0x69, 0x76, 0x65, 0x72, 0x79, 0x2D, 0x64, 0x61, 0x74, 0x65, 0x3A};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

//EOT font
bool check_eot(const byte_t* buf, size_t size)
{
    if(size < 35)return false;
    return (buf[34] == 0x4C && buf[35] == 0x50) &&
        ((buf[8] == 0x00 && buf[9] == 0x00 && buf[10] == 0x01) ||
            (buf[8] == 0x01 && buf[9] == 0x00 && buf[10] == 0x02) ||
            (buf[8] == 0x02 && buf[9] == 0x00 && buf[10] == 0x02));
}

bool check_eps(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x25, 0x21, 0x50, 0x53};    //%!PS    252150532D41646F6265
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_fh8(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x41, 0x47, 0x44, 0x33};    //AGD3
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

//这个不保证正确
bool check_fits(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x53, 0x49, 0x4D, 0x50, 0x4C, 0x45, 0x20, 0x3D, 0x20, 0x54};    //SAMPLE = T
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_gho(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xFE, 0xEF, 0x01};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_ms_mdb(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x53, 0x74, 0x61, 0x6E, 0x64, 0x61, 0x72, 0x64, 0x20, 0x4A};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_ms_msi(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_nes_rom(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4E, 0x45, 0x53, 0x1A};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_pdf(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x25, 0x50, 0x44, 0x46};    //%PDF    255044462D312E
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_windows_pwl(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xE3, 0x82, 0x85, 0x96};
    if(size < sizeof(buf)){
        return false;
    }
    size = sizeof(buf);
    return !memcmp(buf, data, size);
}

bool check_woff(const byte_t* data, size_t size)
{
    if(size < 4)return false;
    byte_t woff[] = {0x77, 0x4F, 0x46, 0x46};    //wOFF
    byte_t wof2[] = {0x77, 0x4F, 0x46, 0x32};    //wOF2
    return !memcmp(woff, data, 4) || !memcmp(wof2, data, 4);
}

bool check_qdf(const byte_t* data, size_t size)
{
    if(size < 4)return false;
    byte_t buf[] = {0xAC, 0x9E, 0xBD, 0x8F};
    return !memcmp(buf, data, 4);
}

bool check_otf(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4F, 0x54, 0x54, 0x4F, 0x00};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_rpm(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xED, 0xAB, 0xEE, 0xDB};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_rtf(const byte_t* data, size_t size)
{
    byte_t buf[] = {'{', 'r', 't', 'f'};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_windows_sdi(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x24, 0x53, 0x44, 0x49, 0x30, 0x30, 0x30, 0x31};    //$SDI0001
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_swf(const byte_t* data, size_t size)
{
    if(size < 3)return false;
    return (data[0] == 0x43 || data[0] == 0x46) && data[1] == 0x57 && data[2] == 0x53;
}

bool check_sqlite3(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x53, 0x51, 0x4C, 0x69};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_torrent(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x64, 0x38, 0x3A, 0x61, 0x6E, 0x6E, 0x6F, 0x75, 0x6E, 0x63, 0x65};    //d8:announce
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_ttf(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x00, 0x01, 0x00, 0x00, 0x00};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_wpd(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xFF, 0x57, 0x50, 0x43};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

//---------------------------------------------------------------------------
//archive

bool check_7z(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_ar(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x21, 0x3C, 0x61, 0x72, 0x63, 0x68, 0x3E};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_arj(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x60, 0xEA};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_bz2(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x42, 0x5A, 0x68};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_cab(const byte_t* data, size_t size)
{
    if(size < 4)return false;
    byte_t buf1[] = {0x4D, 0x53, 0x43, 0x46};    //MSCF
    byte_t buf2[] = {0x49, 0x53, 0x63, 0x28};    //ISc(
    return !memcmp(buf1, data, 4) || !memcmp(buf2, data, 4);
}

bool check_dmg(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x78, 0x01};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_gzip(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x1F, 0x8B, 0x08};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_lha(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x2D, 0x6C, 0x68, 0x35, 0x2D};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_lzip(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4C, 0x5A, 0x49, 0x50};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_rar(const byte_t* data, size_t size)
{
    byte_t buf1[] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00};    //Rar!... rar1.5
    byte_t buf2[] = {0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x01, 0x00};    //Rar!.... rar5
    if(size < sizeof(buf2))return false;
    return !memcmp(buf1, data, sizeof(buf1)) || !memcmp(buf2, data, sizeof(buf2));
}

bool check_tar(const byte_t* data, size_t size)
{
    if(size < 8)return false;
    byte_t buf1[] = {0x75, 0x73, 0x74, 0x61, 0x72, 0x00, 0x30, 0x30};
    byte_t buf2[] = {0x75, 0x73, 0x74, 0x61, 0x72, 0x20, 0x20, 0x00};
    return !memcmp(buf1, data, sizeof(buf1)) || !memcmp(buf2, data, sizeof(buf2));
}

bool check_toast(const byte_t* data, size_t size)
{
    byte_t buf1[] = {0x45, 0x52, 0x02, 0x00, 0x00, 0x00};
    byte_t buf2[] = {0x8B, 0x45, 0x52, 0x02, 0x00, 0x00, 0x00};
    if(size < sizeof(buf2))return false;
    return !memcmp(buf1, data, sizeof(buf1)) || !memcmp(buf2, data, sizeof(buf2));
}

bool check_vmdk(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4B, 0x44, 0x4D};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_ms_wim(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4D, 0x53, 0x57, 0x49, 0x4D};    //MSWIM
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_xar(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x78, 0x61, 0x72, 0x21};    //xar!
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_xz(const byte_t* data, size_t size)
{
    byte_t buf[] = {0xFD, 0x37, 0x7A, 0x58, 0x5A, 0x00};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_z(const byte_t* data, size_t size)
{
    byte_t buf1[] = {0x1F, 0x9D};    //using Lempel-Ziv-Welch algorithm
    byte_t buf2[] = {0x1F, 0xA0};    //using LZH algorithm
    return !memcmp(buf1, data, 2) || !memcmp(buf2, data, 2);
}

bool check_zip(const byte_t* data, size_t size)
{
    if(size < 4) return false;
    byte_t buf1[] = {'P', 'K', 0x03, 0x04}; //empty archive
    byte_t buf2[] = {'P', 'K', 0x05, 0x06}; //empty archive
    byte_t buf3[] = {'P', 'K', 0x07, 0x08}; //spanned archive
    return !memcmp(buf1, data, 4) || !memcmp(buf2, data, 4) || !memcmp(buf3, data, 4);
    //check zip, jar, odt, ods, odp, docx, xlsx, pptx, apk
}

//zlib compressed data
bool check_zlib(const byte_t* data, size_t size)
{
    return data[0] == 0x78 &&
        (data[1] == 0x01 ||    //level 0-1
         data[1] == 0x5E ||    //level 2-5
         data[1] == 0x9C ||    //level 6
         data[1] == 0xDA);     //level 7-9
}
//---------------------------------------------------------------------------
//image

bool check_bmp(const byte_t* data, size_t size)
{
    byte_t buf[] = {'B', 'M'};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_cin(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x80, 0x2A, 0x5F, 0xD7};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_cursor(const byte_t* data, size_t size)
{
    byte_t buf1[] = {'P', 'T'};
    byte_t buf2[] = {0x00, 0x00, 0x02, 0x00};
    return !memcmp(buf1, data, 2) || !memcmp(buf2, data, 4);
}

bool check_dds(const byte_t* data, size_t size)
{
    byte_t buf[] = {'D', 'D', 'S'};    //444453
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_dpx(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x58, 0x50, 0x44, 0x53};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_exr(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x76, 0x2F, 0x31, 0x01};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_gif89a(const byte_t* data)
{
    byte_t buf[] = {'G', 'I', 'F', '8', '9', 'a'};
    return !memcmp(buf, data, sizeof(buf));
}

bool check_gif87a(const byte_t* data)
{
    byte_t buf[] = {'G', 'I', 'F', '8', '7', 'a'};
    return !memcmp(buf, data, sizeof(buf));
}

bool check_gif(const byte_t* data, size_t size)
{
    if(size < 6)return false;
    return check_gif89a(data) || check_gif87a(data);
}

bool check_icon(const byte_t* data, size_t size)
{
    byte_t buf1[] = {'I', 'C'};
    byte_t buf2[] = {0x00, 0x00, 0x01, 0x00};
    if(size < 4)return false;
    return !memcmp(buf1, data, 2) || !memcmp(buf2, data, 4);
}

bool check_jpeg(const byte_t* data, size_t size)
{
    //byte_t buf[] = {0xFF, 0xD8, 0xFF, 0xE0 };    //6-9 == JFIF格式
    //byte_t buf[] = {0xFF, 0xD8, 0xFF, 0xE1 };    //6-9 == Exif格式
    if(size < 4)return false;
    return data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF && (data[3] == 0xE0 || data[3] == 0xE1);
}

bool check_jpeg2k(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x00, 0x00, 0x00, 0x0C, 0x6A, 0x50, 0x20, 0x20};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_pcx(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x0A, 0x05, 0x01, 0x08};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_png(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x89, 0x50, 0x4E, 0x47 };
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_psd(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x38, 0x42, 0x50, 0x53 };    //8BPS
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_tga(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

//IBM Tiff image
bool check_ibm_tiff(const byte_t* data)
{
    byte_t buf[] = {0x49, 0x49, 0x2A, 0x00 };    //little endian format
    return !memcmp(buf, data, 4);
}

//Macintosh Tiff image
bool check_mac_tiff(const byte_t* data)
{
    byte_t buf[] = {0x4D, 0x4D, 0x00, 0x2A };    //big endian format
    return !memcmp(buf, data, 4);
}

bool check_tiff(const byte_t* data, size_t size)
{
    if(size < 4)return false;
    return check_ibm_tiff(data) || check_mac_tiff(data);
}

bool check_webp(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x52, 0x49, 0x46, 0x46, 0x28, 0x29, 0x00, 0x00, 0x57, 0x45, 0x42, 0x50};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

//---------------------------------------------------------------------------
//media

//---------------------------------------------------------------------------
//audio

//au

bool check_flac(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x66, 0x4C, 0x61, 0x43};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_midi(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4D, 0x54, 0x68, 0x64};    //MThd
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_mp3(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x49, 0x44, 0x33};    //ID3
    //MPEG-1 Layer 3 file without an ID3 tag or with an ID3v1 tag
    //(which‘s appended at the end of the file)
    //0xFF, 0xFB
    //0xFF, 0xFD
    //0xFF, 0xFA
    //0xFF, 0xF2
    //0xFF, 0xF3
    return !memcmp(buf, data, sizeof(buf)) ||
        (data[0] == 0xFF && (
            data[1] == 0xF2 ||
            data[1] == 0xF3 ||
            data[1] == 0xFA ||
            data[1] == 0xFB ||
            data[1] == 0xFD)
        );
}

bool check_ogg(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x4F, 0x67, 0x67, 0x53};    //OggS
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_ram(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x2E, 0x72, 0x61, 0xFD};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

//---------------------------------------------------------------------------
//video

bool check_3gp(const byte_t* data, size_t size)
{
    byte_t buf1[] = {0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70, 0x33, 0x67, 0x70};    //3gp
    byte_t buf2[] = {0x00, 0x00, 0x00, 0x20, 0x66, 0x74, 0x79, 0x70, 0x33, 0x67, 0x70};    //3g2
    if(size < sizeof(buf1))return false;
    return !memcpy(buf1, data, sizeof(buf1)) || !memcpy(buf2, data, sizeof(buf2));
}

bool check_asf(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_flv(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x46, 0x4c, 0x56, 0x01};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_rm(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x2E, 0x52, 0x4D, 0x46};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_mpeg(const byte_t* data, size_t size)
{
    byte_t buf1[] = {0x00, 0x00, 0x01, 0xBA};
    byte_t buf2[] = {0x00, 0x00, 0x01, 0xB3};
    if(size < sizeof(buf1))return false;
    return !memcmp(buf1, data, sizeof(buf1)) || !memcmp(buf2, data, sizeof(buf2));
}

bool check_riff(const byte_t* buf, size_t size)
{
    if(size < 4) return false;
    //RIFF
    return buf[0] == 0x52 && buf[1] == 0x49 && buf[2] == 0x46 && buf[3] == 0x46;
}

bool check_ani(const byte_t* buf, size_t size)
{
    if(size < 12)return false;
    //ACON
    return check_riff(buf, size) && buf[8] == 0x41 && buf[9] == 0x43 && buf[10] == 0x4F && buf[11] == 0x4E;
}

bool check_wav(const byte_t* buf, size_t size)
{
    if(size < 12)return false;
    //WAVE
    return check_riff(buf, size) && buf[8] == 0x57 && buf[9] == 0x41 && buf[10] == 0x56 && buf[11] == 0x45;
}

bool check_avi(const byte_t* buf, size_t size)
{
    if(size < 12)return false;
    //"AVI "
    return check_riff(buf, size) && buf[8] == 0x41 && buf[9] == 0x56 && buf[10] == 0x49 && buf[11] == 0x20;
}

bool check_mkv(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x1A, 0x45, 0xDF, 0xA3, 0xA3, 0x42, 0x86, 0x81,
        0x01, 0x42, 0xF7, 0x81, 0x01, 0x42, 0xF2, 0x81,
        0x04, 0x42, 0xF3, 0x81, 0x08, 0x42, 0x82, 0x88,
        0x6D, 0x61, 0x74, 0x72, 0x6F, 0x73, 0x6B, 0x61,
        0x42, 0x87, 0x81};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

bool check_ftyp(const byte_t* buf)
{
    //ftyp
    return buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70;
}

bool check_m4a(const byte_t* buf, size_t size)
{
    if(size < 16) return false;
    //xxxx ftyp 0x4D 34 41(M4A)
    //4D344120
    return (check_ftyp(buf) &&
        buf[8] == 0x4D && buf[9] == 0x34 && buf[10] == 0x41) ||
        (buf[0] == 0x4D && buf[1] == 0x34 && buf[2] == 0x41 && buf[3] == 0x20);
}

//mp4
bool check_mp4_box_size(const byte_t* buf)
{
    return buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x0 && (
        buf[3] == 0x14 ||
        buf[3] == 0x18 ||
        buf[3] == 0x20 ||
        buf[3] == 0x28 ||
        buf[3] == 0x1C
        );
}

//00000014 ftyp buf[8] == 0x69 0x73 0x6F 0x6D //isom
//00000014 ftyp buf[8] == 0x4D 0x34 0x56 0x20 //M4V
//0000001C ftyp buf[8] == 0x6D 0x70 0x34 0x32 //mp42 00 00 00 01
bool check_mp4(const byte_t* buf, size_t size)
{
    if(size < 32) return false;

    if(check_mp4_box_size(buf) && check_ftyp(buf)){
        if(buf[8] == 0x33 && buf[9] == 0x67 && buf[10] == 0x70){//3gp
            if(buf[11] >= 0x31 && buf[11] <= 0x36){//3gp1 ~ 3gp6
                return true;
            }
        }

        if(buf[8] == 0x6D && buf[9] == 0x70 && buf[10] == 0x34){//mp41 mp42
            return true;
        }

        if(buf[8] == 0x69 && buf[9] == 0x73 && buf[10] == 0x6F){//iso2 isom
            if(buf[11] == 0x32 || buf[11] == 0x6D){
                return true;
            }
        }

        if(buf[8] == 0x4D && buf[9] == 0x34 && buf[10] == 0x56){//M4V m4vh m4vp
            return true;
        }
    }
    return false;
}

//mp4 in quiketime
bool check_mov(const byte_t* data, size_t size)
{
    byte_t buf1[] = {0x6D, 0x6F, 0x6F, 0x76};    //moov
    byte_t buf2[] = {0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70};//0x71, 0x74 qt
    if(size < sizeof(buf2))return false;
    return !memcmp(buf1, data, sizeof(buf1)) || !memcmp(buf2, data, sizeof(buf2));
}

bool check_webm(const byte_t* data, size_t size)
{
    byte_t buf[] = {0x1A, 0x45, 0xDF, 0xA3};
    if(size < sizeof(buf))return false;
    return !memcmp(buf, data, sizeof(buf));
}

int filetype::init()
{
    reg_type(ft_unknown, "unknown", "unknown file format", null);

    reg_type(FILE_TYPE_PE, "application/windows.bin", "Portable Executeable File Format", check_windows_pe);
    reg_type(FILE_TYPE_MZ, "application/dos.bin", "DOS MZ executable file format", check_dos_mz);
    reg_type(FILE_TYPE_LNK, "application/windows.link", "Windows Shortcut file", check_windows_link);
    reg_type(FILE_TYPE_DEX, "application/android.dex", "Android Dalvik Executable", check_dalvik);
    reg_type(FILE_TYPE_ELF, "application/elf", "Linux Executable and Linkable Format", check_linux_elf);

    //Java class file, Mach-O Fat Binary
    reg_type(FILE_TYPE_CLASS, "application/java.class", "Java class file", check_java_class);

    reg_type(FILE_TYPE_MACH_O, "application/math-o", "Mach-O binary", check_mach_o);

    //vcl object binary
    reg_type(FILE_TYPE_VCLOBJECT, "application/vcl.object", "VCL Object Binary", check_vcl_object_binary);

    reg_type(FILE_TYPE_UTF8, "application/text-utf8", "utf8 text", check_text_utf8);
    reg_type(FILE_TYPE_UTF32, "application/text-utf32", "utf32 text", check_text_utf32);
    reg_type(FILE_TYPE_UTF32BE, "application/text-utf32be", "utf32 big endian text", check_text_utf32be);
    reg_type(FILE_TYPE_UTF16, "application/text-utf16", "utf16 text", check_text_utf16);
    reg_type(FILE_TYPE_UTF16BE, "application/text-utf16be", "utf16 big endian text", check_text_utf16be);

    reg_type(FILE_TYPE_CAD, "application/cad", "CAD file", check_cad);
    reg_type(FILE_TYPE_CHM, "application/microsoft.chm", "Windows Compiled HTML", check_ms_chm);
    reg_type(FILE_TYPE_CRX, "application/google.crx", "Google Chrome Extension", check_goole_crx);
    reg_type(FILE_TYPE_DBA, "application/dba", "Palm Desktop Calendar Archive", check_dba);
    reg_type(FILE_TYPE_DEB, "application/x-deb", "Debian Software Packet", check_deb);
    reg_type(FILE_TYPE_DOC, "application/microsoft.office", "Microsoft Office", check_ms_office);
    reg_type(FILE_TYPE_EML, "application/microsoft.email", "Microsoft Email", check_ms_email);
    reg_type(FILE_TYPE_EOT, "application/eot", "EOT font", null);
    reg_type(FILE_TYPE_EPS, "application/postscript", "PostScript", check_eps);
    reg_type(FILE_TYPE_FH8, "application/freehand8", "FreeHand8", check_fh8);
    reg_type(FILE_TYPE_FITS, "application/fits", "Flexible Image Transport System", check_fits);
    reg_type(FILE_TYPE_GHO, "application/ghost", "Ghost image", check_gho);
    reg_type(FILE_TYPE_MDB, "application/microsoft.access", "Microsoft Access Database", check_ms_mdb);
    reg_type(FILE_TYPE_MSI, "application/windows.msi", "Windows Installer", check_ms_msi);
    reg_type(FILE_TYPE_NES, "application/nes", "Nintendo Entertainment System ROM file", check_nes_rom);
    reg_type(FILE_TYPE_PDF, "application/pdf", "PDF document", check_pdf);
    reg_type(FILE_TYPE_PWL, "application/windows.pwl", "Windows Password", check_windows_pwl);
    reg_type(FILE_TYPE_WOFF, "application/font-woff", "WOFF font", check_woff);
    reg_type(FILE_TYPE_QDF, "application/quicken", "Quicken", check_qdf);
    reg_type(FILE_TYPE_OTF, "application/font-sfnt", "OTF font", check_otf);
    reg_type(FILE_TYPE_RPM, "application/rpm", "RPM Package Manager", check_rpm);
    reg_type(FILE_TYPE_RTF, "application/rtf", "RichText Format", check_rtf);
    reg_type(FILETYPE_SDI, "application/windows.sdi", "System Deployment Image", check_windows_sdi);
    reg_type(FILE_TYPE_SWF, "application/x-shockwave-flash", "shockwave flash", check_swf);
    reg_type(FILE_TYPE_SQLITE3, "application/x-sqlite3", "sqlite3 database", check_sqlite3);
    reg_type(FILE_TYPE_TORRENT, "application/torrent", "Torrent file", check_torrent);
    reg_type(FILE_TYPE_TTF, "application/font-sfnt", "TTF font", check_ttf);
    reg_type(FILE_TYPE_WPD, "application/wpd", "WordPerfect", check_wpd);

    //---------------------------------------------------------------------------
    //archive

    reg_type(FILE_TYPE_7Z, "archive/7z", "7z Archive", check_7z);
    reg_type(FILE_TYPE_AR, "archive/x-unix-archive", "ARJ archive", check_ar);
    reg_type(FILE_TYPE_ARJ, "archive/arj", "ARJ archive", check_arj);
    reg_type(FILE_TYPE_BZ2, "archive/bzip2", "BZip Archive", check_bz2);
    //vnd.ms-cab-compressed
    reg_type(FILE_TYPE_CAB, "archive/cab", "CAB archive", check_cab);
    reg_type(FILE_TYPE_DMG, "archive/dmg", "DMG Image", check_dmg);
    reg_type(FILE_TYPE_GZ, "archive/gzip", "GZip", check_gzip);
    reg_type(FILE_TYPE_LHA, "archive/lha", "LHA archive", check_lha);
    reg_type(FILE_TYPE_LZ, "archive/lzip", "LZip archive", check_lzip);
    reg_type(FILE_TYPE_RAR, "archive/rar", "RAR archive", check_rar);
    reg_type(FILE_TYPE_TAR, "archive/tar", "tar archive", check_tar);
    reg_type(FILE_TYPE_TOAST, "archive/toast", "Roxio Toast disc image file", check_toast);
    reg_type(FILE_TYPE_VMDK, "archive/vmdk", "vmdk Image", check_vmdk);
    reg_type(FILE_TYPE_WIM, "archive/microsoft.wim", "Microsoft Windows Image", check_ms_wim);
    reg_type(FILE_TYPE_XAR, "archive/xar", "extensible ARchive", check_xar);
    reg_type(FILE_TYPE_XZ, "archive/xz", "XZ archive", check_xz);
    reg_type(FILE_TYPE_Z, "archive/x-compress", "Z archive", check_z);
    reg_type(FILE_TYPE_ZIP, "archive/zip", "ZIP archive", check_zip);
    reg_type(FILE_TYPE_ZLIB, "archive/zlib", "zlib stream", check_zlib);

    //---------------------------------------------------------------------------
    //image

    reg_type(FILE_TYPE_ANI, "image/ani", "Windows Animation Cursor", null);
    reg_type(FILE_TYPE_BMP, "image/bmp", "Windows Bitmap", check_bmp);
    reg_type(FILE_TYPE_CIN, "image/cin", "Kodak Cineon image", check_cin);
    reg_type(FILE_TYPE_CUR, "image/cursor", "Windows Cursor", check_cursor);
    reg_type(FILE_TYPE_DDS, "image/dds", "DirectDraw Surface", check_dds);
    reg_type(FILE_TYPE_DPX, "image/dpx", "SMPTE DPX image", check_dpx);
    reg_type(FILE_TYPE_EXR, "image/exr", "OpenEXR image", check_exr);
    reg_type(FILE_TYPE_GIF, "image/gif", "GIF image", check_gif);
    reg_type(FILE_TYPE_ICO, "image/icon", "Icon Image", check_icon);
    reg_type(FILE_TYPE_JPG, "image/jpeg", "JPEG image", check_jpeg);
    reg_type(FILE_TYPE_JP2, "image/jpeg", "JPEG 2000 image", check_jpeg2k);
    reg_type(FILE_TYPE_PCX, "image/pcx", "PCX image", check_pcx);
    reg_type(FILE_TYPE_PNG, "image/png", "PNG image", check_png);
    reg_type(FILE_TYPE_PSD, "image/adobe-photoshop", "Adobe PhotoShop image", check_psd);
    reg_type(FILE_TYPE_TGA, "image/tga", "TGA image", check_tga);
    reg_type(FILE_TYPE_TIFF, "image/tiff", "TIFF image", check_tiff);
    reg_type(FILE_TYPE_WEBP, "image/webp", "webp image", check_webp);

    //---------------------------------------------------------------------------
    //audio

    //au
    reg_type(FILE_TYPE_FLAC, "audio/x-flac", "FLAC audio", check_flac);
    reg_type(FILE_TYPE_MIDI, "audio/midi", "MIDI file", check_midi);
    reg_type(FILE_TYPE_M4A, "audio/m4a", "M4A audio", check_m4a);
    reg_type(FILE_TYPE_MP3, "audio/mp3", "MP3 file", check_mp3);
    reg_type(FILE_TYPE_OGG, "audio/ogg", "Ogg file", check_ogg);
    reg_type(FILE_TYPE_RAM, "audio/real-audio", "Real Audio", check_ram);
    reg_type(FILE_TYPE_WAV, "audio/wav", "WAVE audio", check_wav);

    //---------------------------------------------------------------------------
    //video
    reg_type(FILE_TYPE_3GP, "video/3gp", "3GP file", check_3gp);
    reg_type(FILE_TYPE_ASF, "video/windows.media", "Windows Media", check_asf);
    reg_type(FILE_TYPE_AVI, "video/avi", "avi file", check_avi);
    reg_type(FILE_TYPE_FLV, "video/flv", "flash video", check_flv);
    reg_type(FILE_TYPE_RM, "video/real-media", "Real Media", check_rm);
    reg_type(FILE_TYPE_MP4, "video/mp4", "MP4 video", check_mp4);
    reg_type(FILE_TYPE_MOV, "video/quicktime", "Quicktime Movie", check_mov);
    reg_type(FILE_TYPE_MKV, "video/mkv", "MKV video", check_mkv);
    reg_type(FILE_TYPE_MPEG, "video/mpeg", "MPEG file", check_mpeg);
    reg_type(FILE_TYPE_WEBM, "video/webm", "WebM video", check_webm);

    reg_type(FILE_TYPE_RIFF, "application/unknown-riff", "unknown riff format", check_riff);

    return 0;
}


//---------------------------------------------------------------------------
//通过文件ext判断文件类型
//const char* FILE_EXT_GIF = "gif";
//ftype_t t = hash(FILE_EXT_GIF)

ftype_t filetype_from_ext(const cstring<char>& filename)
{
    if(filename.empty()){
        return ft_unknown;
    }

    if(filename[0] != '.'){
        return hash::mmhash64(filename.c_str(), filename.length(), 0);
    }
    else{
        return hash::mmhash64(filename.c_str()+1, filename.length()-1, 0);
    }

    /*
    cstring<char> ext = extract_file_ext<char>(filename);
    ext.tolower();
    size_t i = 0;
    if(ext[0] == '.')++i;
    switch(ext[i]){
    case 'a':
        break;
    case 'b':
        switch(ext[++i]){
        case 'm':
            switch(ext[++i]){
            case 'p':
                return FILE_TYPE_BMP;
            }
            break;
        default:
            break;
        }
    default:
        break;
    }
    return ft_unknown;
    */
}


//---------------------------------------------------------------------------


//通过数据格式判断文件类型
uint64_t filetype_from_data(const void* data, size_t size)
{
    if(!data){
        return ft_error;
    }
    const byte_t *buf = static_cast<const byte_t*>(data);
    return filetype::check(buf, size);
}

//ISO9660 CD/DVD image file
//check offset 0x8001, 0x8801 or 0x9001
bool is_iso_file(std::FILE* f)
{
    static const char* sn = "CD001";
    char buf[5];
    fseek(f, 0x8001, SEEK_SET);
    fread(buf, 1, 5, f);
    if(!memcmp(buf, sn, 5))return true;

    fseek(f, 0x8801, SEEK_SET);
    fread(buf, 1, 5, f);
    if(!memcmp(buf, sn, 5))return true;

    fseek(f, 0x9001, SEEK_SET);
    fread(buf, 1, 5, f);
    if(!memcmp(buf, sn, 5))return true;

    return false;
}

}//end namespace filesystem
}//end namespace cgl

#endif //FILE_TYPE_HPP_20170120171738
