/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 mm_lodfile.hpp

 2018-11-02 09:00:40

*/
#ifndef MM_LODFILE_HPP_20181102090040
#define MM_LODFILE_HPP_20181102090040

#include <cgl/public.h>
#include <cgl/io.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/stdlib.hpp>
#include <cgl/zlib.hpp>
#include <cgl/log.hpp>

#ifdef INC_VCL
#include <vcl.h>
#endif

namespace heros3{

using namespace cgl;

typedef std::vector<byte_t> mmBuffer;

void print(const char* str1, const char* str2)
{
    string_t s = str1;
    s += " ";
    s += str2;
    print_message(s.c_str());
}

template<typename T>
uint32_t read_uint32(const T* p)
{
    return *(uint32_t*)p;
}

template<typename T>
uint16_t read_uint16(const T* p)
{
    return *(uint16_t*)p;
}

uint32_t read_uint32(const mmBuffer& data, size_t address)
{
    return read_uint32(&data[address]);
}

//---------------------------------------------------------------------------
//
// mmLodFile
//
//---------------------------------------------------------------------------

#pragma pack(push, 1)
typedef struct MM_LOD    //文件头部92字节
{
    uint32_t type;       //LOD   0x444F4C {0x4C, 0x4F, 0x44, 0x00};
    uint32_t version;    //h3ab 0x01F4 h3 0xC8
    uint32_t count;      //数量
    //uint32_t unknow[20];   //未知数据空
}MM_LOD;

typedef struct MM_LIST
{
    char     name[16]; //图片名称
    uint32_t offset;   //偏移量
    uint32_t size;     //源数据大小
    uint32_t type;     //.txt = 2; 有些.pcx=16, 有些.pcx=17
    uint32_t compress; //压缩后大小，0为未压缩
}MM_LIST;
#pragma pack(pop)

class mmLodFile
{
public:
    const static uint32_t TYPE_LOD = 0x444F4C;    //LOD 文件标识
    const static uint32_t H3_VER  = 0xC8;    //200 英雄无敌3
    const static uint32_t H3A_VER = 0x01F4;  //500 死亡阴影
private:
    string_t m_filename;
    MM_LOD m_info;
    std::vector<MM_LIST> m_list;
public:
    mmLodFile() : m_filename(), m_list()
    {
        memset(&m_info, 0, sizeof(m_info));
    }

    ~mmLodFile()
    {
        this->close();
    }

    int open(const string_t& filename)
    {
        this->close();
        io::binfile f;
        f.open(filename.c_str(), io_read);
        if(!f.is_open())
        {
            print("mmLodFile file open failed :", filename.c_str());
            this->close();
            return -1;
        }
        f.read(&m_info, sizeof(m_info));
        if(m_info.type != TYPE_LOD)
        {
            print("mmLodFile file format error :", filename.c_str());
            this->close();
            return -1;
        }
        if((m_info.version != H3_VER) && (m_info.version != H3A_VER))
        {
            print("mmLodFile file version error :", filename.c_str());
            this->close();
            return -1;
        }

        m_list.resize(this->count());
        memset(&m_list.front(), 0, sizeof(MM_LIST) * this->count());
        f.seek(92, SEEK_SET);
        size_t n = f.read(&m_list.front(), sizeof(MM_LIST) * this->count());

        m_filename = filename;

        return 0;
    }

    void close()
    {
        m_filename.clear();
        m_list.clear();
        memset(&m_info, 0, sizeof(m_info));
    }

    bool is_open()const
    {
        return !m_filename.empty();
    }

    uint32_t version()const
    {
        return m_info.version;
    }

    //获取文件属性
    size_t count()const
    {
        return m_info.count;
    }

    int ls(MM_LIST& list, size_t index)
    {
        if(index < this->count()){
            list = m_list[index];
            return 0;
        }
        return -1;
    }

    int ls(MM_LIST& list, const char* fname)
    {
        for(size_t i=0; i<m_list.size(); ++i){
            if(!strnicmp(m_list[i].name, fname, 16)){
                list = m_list[i];
                return 0;
            }
        }
        return -1;
    }

    //解压读出文件到内存。lpData缓冲区，Size缓冲区大小。
    //成功返回Z_OK，失败返回Zlib错误码
    int read(mmBuffer& data, uint32_t Index)
    {
        MM_LIST Info = {0};
        this->ls(Info, Index);
        return this->read(data, Info);
    }

    int read(mmBuffer& data, const MM_LIST& info)
    {
        io::binfile f;
        f.open(m_filename.c_str(), io_read);
        if(!f.is_open()){
            return -1;
        }
        if(0 == info.compress)
        {//数据没有压缩
            data.resize(info.size);
            f.seek(info.offset, SEEK_SET);
            f.read(&data[0], info.size);
            return Z_OK;
        }

        mmBuffer source;
        source.resize(info.compress);

        f.seek(info.offset, SEEK_SET);
        f.read(&source[0], info.compress);  //读取压缩数据

        //解压数据
        size_t size = info.size;//这里会出错，不明白
        data.resize(size);
        int result = zlib_uncompress(&data[0], &size, &source[0], source.size());
        return result;
    }

    //将文件解压并导出
    int exportToFile(const char* FileName, uint32_t Index)
    {
        MM_LIST Info = {0};
        this->ls(Info, Index);
        return exportToFile(FileName, Info);
    }

    int exportToFile(const char* FileName, const MM_LIST& info)
    {
        mmBuffer buf;
        uint32_t dwSize = info.size;
        if(this->read(buf, info) == 0){
            io::binfile f;
            f.open(FileName, io_write|io_create);
            if(!f.is_open()){
                return -2;
            }
            f.write(&buf[0], buf.size());
            return 0;
        }
        return -1;
    }
};

//---------------------------------------------------------------------------
//
// mmTextFile
//
// text是直接没有压缩的文本
//---------------------------------------------------------------------------

int mmLoadText(mmBuffer& buf, mmLodFile& lodpak, const char* name)
{
    MM_LIST ls;
    if(lodpak.ls(ls, name) == 0){
        return lodpak.read(buf, ls);
    }
    return -1;
}

//---------------------------------------------------------------------------
//
// mmPcxImage
//
//---------------------------------------------------------------------------
/*
h3PCX_16 文件格式:
格式说明    长度(字节)
数据长度    4
宽度        4
高度        4
图象数据    数据长度    1字节对齐，8位调色板
调色板      780

*/
class mmPCX
{
public:
    typedef struct MM_PCX
    {
        uint32_t size;
        uint32_t width;
        uint32_t height;
        byte_t   data[];
    }MM_PCX;
    typedef vec3ub PCXCOLOR;
    typedef PCXCOLOR PCXPAL[260];//PCX调色板
private:
    mmBuffer  m_data;
    MM_PCX*   m_pcx;
    PCXCOLOR* m_pal;
    int m_pixelbits;
public:
    mmPCX() : m_data(), m_pcx(null), m_pal(null), m_pixelbits()
    {

    }

    int open(mmLodFile& lodpak, const char* filename);
    void close()
    {
        m_pixelbits = 0;
        m_data.clear();
        m_pcx = null;
        m_pal = null;
    }

    bool is_open()const
    {
        return m_pcx;
    }

    size_t size()const
    {
        return m_pcx->size;
    }

    int pixelbits()const
    {
        return m_pixelbits;
    }

    int width()const
    {
        return m_pcx->width;
    }

    int height()const
    {
        return m_pcx->height;
    }

    const byte_t* data()const
    {
        return m_pcx->data;
    }

    const PCXCOLOR& pal(int index)const
    {
        return m_pal[index];
    }

    byte_t* scanline(int y)
    {
        return m_pcx->data + y * m_pcx->width;
    }

    #ifdef INC_VCL
    void paint(Graphics::TBitmap* bmp);
    #endif
    void paint(graphics::pixelimage& img);
};

//统一加载成24bit位图
int mmPCX::open(mmLodFile& lod, const char* filename)
{
    MM_LIST ls;
    if(this->is_open())this->close();

    if(lod.ls(ls, filename) < 0){
        return -1;
    }

    lod.read(m_data, ls);

    //输出图象
    m_pcx = (MM_PCX*)&m_data[0];
    //m_data.resize(m_pcx->width * m_pcx->height * (m_pixelbits / 8));

    if(ls.type == 16 || m_pcx->size == m_pcx->width * m_pcx->height){
        m_pixelbits = 8;
        //设置调色板
        //调色板数据在图象数据的尾部，长度：780
        m_pal = (PCXCOLOR*)(m_pcx->data + m_pcx->size);
    }
    else if(ls.type == 17 || m_pcx->size == m_pcx->width * m_pcx->height * 3){
        m_pixelbits = 24;
        m_pal = null;
    }

    return 0;
}

void mmPCX::paint(graphics::pixelimage& bmp)
{
    bmp.create(this->width(), this->height(), 24);
    if(pixelbits() == 8)//8位图象
    {
        BYTE     *pSrc = m_pcx->data;
        PCXCOLOR *pDst = NULL;
        for(DWORD y = 0; y<m_pcx->height; ++y)
        {
            pDst = (PCXCOLOR*)bmp.scanline(y);
            for(DWORD x=0; x<m_pcx->width; ++x)
            {
                if(pSrc[x] == 0)continue;
                pDst[x].r = m_pal[pSrc[x]].b;
                pDst[x].g = m_pal[pSrc[x]].g;
                pDst[x].b = m_pal[pSrc[x]].r;
            }
            pSrc += this->width();
        }
    }
    else if(pixelbits() == 24)//24位图象，无法证明是否是4字节对齐
    {
        PCXCOLOR *pSrc = (PCXCOLOR*)m_pcx->data;
        BYTE     *pDst = NULL;
        for(DWORD y = 0; y<m_pcx->height; ++y)
        {
            pDst = (BYTE*)bmp.scanline(y);
            memcpy(pDst, pSrc, this->width() * 3);
            pSrc += this->width();
        }
    }
    else
    {
        print("mmPCX :", "未知数据格式");
    }
}

#ifdef INC_VCL
void mmPCX::paint(Graphics::TBitmap* bmp)
{
    bmp->Width = this->width();
    bmp->Height = this->height();
    bmp->PixelFormat = pf24bit;
    if(pixelbits() == 8)//8位图象
    {
        BYTE     *pSrc = m_pcx->data;
        PCXCOLOR *pDst = NULL;
        for(DWORD y = 0; y<m_pcx->height; ++y)
        {
            pDst = (PCXCOLOR*)bmp->ScanLine[y];
            for(DWORD x=0; x<m_pcx->width; ++x)
            {
                if(pSrc[x] == 0)continue;
                pDst[x].r = m_pal[pSrc[x]].b;
                pDst[x].g = m_pal[pSrc[x]].g;
                pDst[x].b = m_pal[pSrc[x]].r;
            }
            pSrc += this->width();
        }
    }
    else if(pixelbits() == 24)//24位图象，无法证明是否是4字节对齐
    {
        PCXCOLOR *pSrc = (PCXCOLOR*)m_pcx->data;
        BYTE     *pDst = NULL;
        for(DWORD y = 0; y<m_pcx->height; ++y)
        {
            pDst = (BYTE*)bmp->ScanLine[y];
            memcpy(pDst, pSrc, this->width() * 3);
            pSrc += this->width();
        }
    }
    else
    {
        print("mmPCX :", "未知数据格式");
    }
}

#endif

//---------------------------------------------------------------------------
//
// mmDefFile
//
//---------------------------------------------------------------------------

typedef vec3ub MM_SPPAL[256]; //精灵调色板

enum MM_DEF_TYPE
{
    DEF_SPELL    = 0x40,    //魔法动画
    DEF_SPRITE   = 0x41,    //界面元素、城堡建设
    DEF_CREATURE = 0x42,    //生物
    DEF_MAP      = 0x43,    //地图物体
    DEF_HERO     = 0x44,    //英雄
    DEF_TERRAIN  = 0x45,    //地形
    DEF_CURSOR   = 0x46,    //指针
    DEF_UI       = 0x47,    //界面元素interface
    DEF_SP_FRAME = 0x48,    //SPRITE_FRAME
    DEF_BFHERO   = 0x49     //战场英雄
};

int mmdef_types(int index) //0~9
{
    return index < 10 ?  0x40 + index : -1;
}

const char* mmdef_type_name(int type)
{
    const char* names[] = {"技能", "精灵", "生物", "地图", "英雄", "地形", "指针", "界面", "帧", "战场英雄" };
    type -= 0x40;
    return (type < 10) ? names[type] : "未知类型";
}

#pragma pack(push, 1)
typedef struct MM_DEF
{
    uint32_t type;      //精灵类型
    uint32_t width;     //宽度
    uint32_t height;    //高度
    uint32_t count;     //精灵数量
    vec3ub     palette[0];
}MM_DEF;
#pragma pack(pop)


enum h3CREATURE_ANI//生物动作类型
{
    ANI_MOVE         = 0,
    ANI_HOTSPOT      = 1,
    ANI_STAND        = 2,
    ANI_HIT          = 3,
    ANI_DEFENSE      = 4,
    ANI_DEATH        = 5,
    ANI_UNUSE        = 6,
    ANI_TURN_LEFT    = 7,
    ANI_TURN_RIGHT   = 8,
    ANI_LEFT2        = 9,
    ANI_RIGHT2       = 10,
    ANI_ATTACK_UP    = 11,
    ANI_ATTACK_FRONT = 12,
    ANI_ATTACK_DOWN  = 13,
    ANI_SHOOT_UP     = 14,
    ANI_SHOOT_FRONT  = 15,
    ANI_SHOOT_DOWN   = 16,
    ANI_MAGIC_UP     = 17,
    ANI_MAGIC_FRONT  = 18,
    ANI_MAGIC_DOWN   = 19,
    ANI_START_MOVE   = 20,
    ANI_STOP_MOVE    = 21
};

const char* creature_slot_name(int index)
{
    const char* CREATURE_SLOT_NAME[] = {
        "移动",
        "活动",
        "站立",
        "受伤",
        "防御",
        "死亡",
        "未使用",
        "左转",
        "右转",
        "左转x",
        "右转x",
        "向上攻击",
        "向前攻击",
        "向下攻击",
        "向上射击",
        "向前射击",
        "向下射击",
        "向上施法",
        "向前施法",
        "向下施法",
        "开始移动",
        "结束移动"
    };
    if(index < int(countof(CREATURE_SLOT_NAME))){
        return CREATURE_SLOT_NAME[index];
    }
    return "未知动作";
}

const char* hero_slot_name(int index)
{
    const char* HERO_SLOT_NAME[] = {//地图英雄动作类型
        "上",
        "右上",
        "右",
        "右下",
        "下",
        "向上移动",
        "右上移动",
        "向右移动",
        "右下移动",
        "向下移动",
    };
    if(index < int(countof(HERO_SLOT_NAME))){
        return HERO_SLOT_NAME[index];
    }
    return "未知动作";
}

//def.slot
const char* mmslot_name(int deftype, int index)
{
    static char buf[32];
    switch(deftype){
    case DEF_CREATURE:
        return creature_slot_name(index);
    case DEF_HERO:
        return hero_slot_name(index);
    }
    itoa(index, buf, 10);
    return buf;
}

class mmDefFile
{
public:
    typedef std::vector<byte_t> buffer_t;
    const static size_t NAME_SIZE = 13;

    struct MM_SLOT
    {
        uint32_t type;        //类型
        uint32_t count;        //帧数量
        uint32_t unknow1;
        uint32_t unknow2;
    };

    struct imageinfo
    {
        char name[NAME_SIZE];
        uint32_t address;
        imageinfo()
        {
            memset(this, 0, sizeof(*this));
        }
        bool operator==(const imageinfo& info)
        {
            return address == info.address;
        }
    };

    const MM_DEF* const info;
private:
    char     m_name[16];    //def文件名称
    buffer_t m_data;        //def数据
    std::vector<MM_SLOT*>    m_slots;//保存所有动作的地址
    std::vector<imageinfo>    m_images;
public:
    mmDefFile() : info(null), m_data(), m_slots(), m_images()
    {
        memset(m_name, 0, sizeof(m_name));
    }
    ~mmDefFile()
    {
        this->close();
    }
    int open(const char* fname)
    {
        io::binfile f;
        this->close();
        f.open(fname, io_read);
        if(!f.is_open()){
            print("mmDefFile file open error :", fname);
            return -1;
        }
        strncpy(m_name, fname, 16);
        m_data.resize(f.size());
        f.seek(0, SEEK_SET);
        f.read(&m_data[0], m_data.size());
        loadDef();
        return 0;
    }

    int open(mmLodFile& lodpak, size_t index)
    {
        MM_LIST Info = {0};
        this->close();
        if(lodpak.ls(Info, index))
        {
            print_message("mmDefFile open index ：文件错误。");
            return -1;
        }
        return this->open(lodpak, Info);
    }

    int open(mmLodFile& lodpak, const char* fname)
    {
        MM_LIST info = {0};
        this->close();
        if(lodpak.ls(info, fname))
        {
            print_message("mmDefFile open name ：文件错误。");
            return -1;
        }
        return this->open(lodpak, info);
    }

    int open(mmLodFile& lodpak, MM_LIST& info)
    {
        strncpy(m_name, info.name, 16);
        if(lodpak.read(m_data, info) == Z_OK){
            return loadDef();
        }
        return -1;
    }

    int save(const char* fname)
    {
        if(this->is_open()){
            io::binfile f;
            f.open(fname, io_write|io_create);
            f.write(&m_data[0], m_data.size());
            return 0;
        }
        return -1;
    }

    void close()
    {
        const_write(info) = null;
        m_data.clear();
        m_slots.clear();
    }

    bool is_open()const { return info; }

    const char* name()const { return m_name; }
    size_t size()const { return m_data.size(); }
    const byte_t* data()const { return &m_data[0]; }

    int width()const { return info->width; }
    int height()const { return info->height; }

    const vec3ub* palette()const { return info->palette; }

    #ifdef CGL_PLATFORM_WINDOWS
    HPALETTE create_palette()const
    {
        return cgl::graphics::create_palette(this->palette(), 256);
    }
    #endif

    size_t slots_count()const
    {//动作列表
        return info->count;
    }

    const MM_SLOT* slots(size_t index = -1)const
    {//动作信息 -1代表当前slot
        return m_slots[index];
    }

    size_t slot_images(size_t slot)const
    {//动作有多少帧图片
        return this->slots(slot)->count;
    }
    imageinfo slot_image(size_t slot, size_t image)
    {//返回图片地址
        imageinfo info;
        const MM_SLOT* pslot = this->slots(slot);
        const byte_t* data = (const byte_t*)pslot;
        data += sizeof(MM_SLOT);
        size_t offset = image * NAME_SIZE;
        cscpy<char>(info.name, (const char*)(data + offset), NAME_SIZE);
        offset = pslot->count * NAME_SIZE;//skip name list
        offset += image * sizeof(uint32_t);//jump to address
        info.address = read_uint32(data + offset);
        return info;
    }

    size_t image_count()const { return m_images.size(); }
    const imageinfo& images(size_t index)const { return m_images[index]; }
    const byte_t* image_data(size_t address)const
    {//图片数据地址
        return &m_data[address];
    }

    //将所有图片都释放到一张大的图片中，同时保存动作信息到ini文件中
    //int ExtractToFile(AnsiString fdir = '.', int iWidth = 512);
private:
    int loadDef();
};

int mmDefFile::loadDef()
{//列举所有动作的地址，保存在m_slots
    const_write(info) = (MM_DEF*)&m_data[0];

    //print(def_names(info->type));
    switch(info->type)//检测
    {
    //DEF_MAGIC, DEF_BUILD, DEF_CREATURE, DEF_MAPOBJ, DEF_HERO, DEF_UI, DEF_BFHERO
    case DEF_SPELL:
        break;
    case DEF_SPRITE:
        break;
    case DEF_CREATURE:
        break;
    case DEF_MAP:
        break;
    case DEF_HERO:
        break;
    case DEF_TERRAIN:
        break;
    case DEF_CURSOR:
        break;
    case DEF_UI:
        break;
    case DEF_SP_FRAME:
        break;
    case DEF_BFHERO:
        break;
    default:
        print_message((string_t("mmDefFile：未知类型(") + string_t((int)info->type) + ")。").c_str());
        break;
    }

    //list all slots
    size_t base_offset = sizeof(MM_DEF) + sizeof(MM_SPPAL);//first slot address, 16+768
    size_t offset;
    MM_SLOT* slot;
    imageinfo image;
    m_slots.reserve(info->count);
    for(size_t i=0; i<info->count; ++i)
    {
        slot = (MM_SLOT*)&m_data[base_offset];
        m_slots.push_back(slot);
        //list all images
        for(int j=0; j<int(slot->count); ++j){
            offset = sizeof(MM_SLOT) + NAME_SIZE * j;
            cscpy<char>(image.name, (const char*)(&m_data[base_offset + offset]), NAME_SIZE);
            offset = sizeof(MM_SLOT) + NAME_SIZE * slot->count + sizeof(uint32_t) * j;
            image.address = read_uint32(m_data, base_offset + offset);
            if(std::find(m_images.begin(), m_images.end(), image) == m_images.end()){
                m_images.push_back(image);
            }
        }
        base_offset += sizeof(MM_SLOT) + (NAME_SIZE + sizeof(uint32_t)) * slot->count;
    }
    return 0;
}

//---------------------------------------------------------------------------
//
// mmImage(sprite image)
//
//---------------------------------------------------------------------------
/*
heros3 def 格式解析

def header:
uint32_t type;    //类型
uint32_t width;    //宽度
uint32_t height;//高度
uint32_t count;    //slot数量

[RGB palette[256]]

slot list
[uint32 type][uint32 count][uint32][uint32]
[char name[13]] * slot.count    图像名字列表
[uint32 address] * slot.count    图像地址列表

---------------------------------------------------------------------------*/

#pragma pack(push, 1)
//精灵图像头
struct MM_IMAGE
{
    uint32_t size;          //数据文件大小
    uint32_t format;        //压缩方式
    uint32_t fullWidth;     //解析宽度
    uint32_t fullHeight;    //解析宽度
    uint32_t width;         //宽度
    uint32_t height;        //高度
    int32_t  offset_x;      //X偏移量
    int32_t  offset_y;      //Y偏移量
    byte_t   data[0];
};
#pragma pack(pop)

class mmImage
{
public:
    const mmDefFile* const def;
    MM_IMAGE m_image;
    std::vector<byte_t> m_data;
public:
    mmImage() : def(null), m_data()
    {
        memset(&m_image, 0, sizeof(MM_IMAGE));
    }

    int open(const mmDefFile& defFile, size_t address)
    {
        const_write(def) = &defFile;
        int line = this->uncompress(defFile.image_data(address));
        return line;
    }

    void close()
    {
        const_write(def) = null;
        m_data.clear();
        memset(&m_image, 0, sizeof(MM_IMAGE));
    }

    bool is_open()const { return m_data.size(); }

    int width()const { return m_image.width; }
    int height()const { return m_image.height; }
    int fullWidth()const { return m_image.fullWidth; }
    int fullHeight()const { return m_image.fullHeight; }
    int offset_x(){ return m_image.offset_x; }
    int offset_y(){return m_image.offset_y;}

    #ifdef INC_VCL
    int paint(Graphics::TBitmap* bmp, int left, int top);
    int paint8(Graphics::TBitmap* bmp, int left, int top);
    #endif
    //边框颜色、影子颜色
    int paint(graphics::pixelimage& image, const vec4ub& frame, const vec4ub& shadow = vec4ub(0, 0, 0, 128));

    byte_t* scanline(int y)
    {
        return &m_data[m_image.width * y];
    }

    const vec3ub& palcolor(int index)const
    {
        return def->palette()[index];
    }
private:
    int uncompress(const byte_t* source);

};

int mmImage::uncompress(const byte_t* source)
{
    const MM_IMAGE* image = (const MM_IMAGE*)source;
    const byte_t *src;
    int type;
    int size;

    m_image = *image;
    source = image->data;

    //special case for some "old" format defs (SGTWMTA.DEF and SGTWMTB.DEF)
    //地雷魔法，没有偏移量，图片头就16字节
    if(m_image.format == 1 && m_image.width > m_image.fullWidth && m_image.height > m_image.fullHeight)
    {
        m_image.offset_x = 0;
        m_image.offset_y = 0;
        m_image.width = m_image.fullWidth;
        m_image.height = m_image.fullHeight;
        source -= 16;
    }

    m_data.resize(m_image.width * m_image.height * 2);
    //memset(m_data.data(), 0, m_data.size());
    byte_t *dest = &m_data[0];

    if(image->format == 0)
    {
        //未压缩的数据
        src = source;
        for (size_t y = 0; y < m_image.height; ++y){
            memcpy(dest, src, m_image.width);
            src += m_image.width;
            dest += m_image.width;
        }
    }
    else if(image->format == 1)
    {
        //图像数据开始是32位地址偏移，代表每个扫描行的起始地址
        const uint32_t* pEntry = (uint32_t*)source;
        for(size_t y=0; y < m_image.height; ++y){
            src = source + pEntry[y];
            for(size_t x = 0; x < m_image.width; ){
                type = *src++;
                size = *src++;
                ++size;
                if(type == 0xFF){//未压缩 这里是主要的图象数据
                    memcpy(dest + x, src, size);
                    src += size;
                }
                else{//重复像素 精灵的轮廓和影子 0=图象中的空位偏移
                    memset(dest + x, type, size);
                }
                x += size;
            }
            dest += m_image.width;
        }
    }
    else if(image->format == 2)
    {
        //图像头之后，就是图像数据，RLE压缩
        //第一个字节高3位代表数据压缩方式，低5位代表数量
        //调色板前几位颜色是固定的，t<7代表调色板索引值
        //t = 0 透明色
        //t = 1 影子轮廓
        //t = 4 影子
        //t = 5 玩家颜色(旗帜、怪物描边颜色)
        //t = 7 未压缩数据
        src = source;
        for(size_t y = 0; y < image->height; ++y){
            for(size_t j = 0; j < m_image.width;){
                type = (*src) >> 5;
                size = (*src) & 31;
                ++size;
                ++src;
                if(!type){//skip
                }
                else if(type < 7){//RLE
                    memset(dest+j, type, size);
                }
                else{//a == 7
                    memcpy(dest+j, src, size);
                    src += size;
                }
                j += size;
            }
            dest += m_image.width;
        }
    }
    else if(image->format == 3)
    {
        //数据压缩方式和2一样
        //图像头之后，是每行的起始地址，16bit
        m_image.width = image->width;
        for(size_t y = 0; y < m_image.height; ++y){
            src = source + (*(uint16_t*)(source + y * 2 * m_image.width / 32));
            for(size_t j = 0; j < m_image.width;){
                type = (*src) >> 5;
                size = (*src) & 31;
                ++size;
                ++src;
                if(!type){//skip
                }
                else if(type < 7){//RLE
                    memset(dest+j, type, size);
                }
                else{//a == 7
                    memcpy(dest+j, src, size);
                    src += size;
                }
                j += size;
            }
            dest += m_image.width;
        }
    }
    else
    {
        print_message(("未知压缩格式" + string_t((int)image->format)).c_str());
        return image->format;
    }

    return image->format;
}

int mmImage::paint(graphics::pixelimage& image, const vec4ub& frame, const vec4ub& shadow)
{
    image.create(m_image.width, m_image.height, 32);
    image.clear();
    for(size_t y=0; y < m_image.height; ++y)
    {
        byte_t  *psrc = &m_data[m_image.width * y];
        graphics::color32 *pdst = (graphics::color32*)image.scanline(y);
        for(uint32_t x=0; x < m_image.width; ++x)
        {
            switch(psrc[x]){
            case 0:
                pdst[x].color = 0;
                break;
            case 1://浅色阴影
                pdst[x].r = shadow.r;
                pdst[x].g = shadow.g;
                pdst[x].b = shadow.b;
                pdst[x].a = shadow.a / 4;
                break;
            case 2:
            case 3:
                break;
            case 4://深色阴影
                pdst[x].r = shadow.r;
                pdst[x].g = shadow.g;
                pdst[x].b = shadow.b;
                pdst[x].a = shadow.a;//128
                break;
            case 5://玩家颜色、生物描边
                pdst[x].r = frame.r;
                pdst[x].g = frame.g;
                pdst[x].b = frame.b;
                pdst[x].a = frame.a;
                break;
            case 6://轮廓？
                break;
            case 7://??
                break;
            default:
                pdst[x].r = palcolor(psrc[x]).r;
                pdst[x].g = palcolor(psrc[x]).g;
                pdst[x].b = palcolor(psrc[x]).b;
                pdst[x].a = 255;
                break;
            }
        }
    }
    return 0;
}

#ifdef INC_VCL

int mmImage::paint(Graphics::TBitmap* bmp, int left, int top)
{//24位图绘制模式
    const vec3ub* pal = def->palette();
    bmp->Width = m_image.width;
    bmp->Height = m_image.height;
    bmp->PixelFormat = pf32bit;
    bmp->Canvas->Brush->Color = clBlack;
    bmp->Canvas->FillRect(TRect(0, 0, m_image.width, m_image.height));

    for(size_t y=0; y < m_image.height; ++y)
    {
        byte_t  *psrc = &m_data[m_image.width * y];
        color32 *pdst = (color32*)bmp->ScanLine[y];
        for(uint32_t x=0; x < m_image.width; ++x)
        {
            if(psrc[x] == 0){
            }
            else if(psrc[x] == 1){//浅色阴影
                pdst[x].r = 0;
                pdst[x].g = 0;
                pdst[x].b = 0;
                pdst[x].a = 64;
            }
            else if(psrc[x] == 4){//深色阴影
                pdst[x].r = 0;
                pdst[x].g = 0;
                pdst[x].b = 0;
                pdst[x].a = 128;
            }
            else if(psrc[x] == 5){//玩家颜色、生物描边
                pdst[x].r = 255;
                pdst[x].g = 0;
                pdst[x].b = 255;
                pdst[x].a = 255;
            }
            else{
                pdst[x].r = pal[psrc[x]].r;
                pdst[x].g = pal[psrc[x]].g;
                pdst[x].b = pal[psrc[x]].b;
                pdst[x].a = 255;
            }
        }
    }
    return 0;
}

int mmImage::paint8(Graphics::TBitmap* bmp, int left, int top)
{//8位图绘制模式
    //bmp->Width = this->frame_width();
    //bmp->Height = this->frame_height();
    //bmp->PixelFormat = pf8bit;
    //bmp->Palette = def->create_palette();
    //bmp->Canvas->Brush->Color = RGB(0, 255, 255);
    //bmp->Canvas->FillRect(TRect(0, 0, bmp->Width, bmp->Height));
    size_t x;
    for(uint32_t y=0; y < m_image.height; ++y)
    {
        //x = this->offset_x() + left;
        //memcpy((byte_t*)bmp->ScanLine[top + y + this->offset_y()] + x, &m_data[width() * y], width());
        memcpy((byte_t*)bmp->ScanLine[top + y] + left, &m_data[m_image.width * y], m_image.width);
    }
    return 0;
}

#endif//CGL_PLATFORM_CBUILDER

}//end namespace heros3

namespace heros3{

struct MM_WAV
{
    char name[12];    //name和ext之间是个空格
    char unknow[16 + 12];
    uint32_t offset;
    uint32_t size;
};

class mmSndLod
{
public:
    string_t filename;
    std::vector<MM_WAV> m_list;
public:
    int open(const char* fn)
    {
        uint32_t size;
        io::binfile f;
        f.open(fn, io_read);
        if(f.is_open()){
            f.read(&size, 4);
            m_list.resize(size);
            f.read(&m_list[0], sizeof(MM_WAV) * size);
            filename = fn;
            return 0;
        }
        return -1;
    }

    int read(std::vector<byte_t>& buf, const char* name)
    {
        MM_WAV* pwav = null;
        char wavname[12] = {0};
        for(int i=0; i<12; ++i)if(name[i] != '.')wavname[i] = name[i];
        for(size_t i=0; i<m_list.size(); ++i){
            if(!strnicmp(m_list[i].name, wavname, 12)){
                pwav = &m_list[i];
                break;
            }
        }
        if(pwav){
            return this->read(buf, *pwav);
        }
        return -1;
    }

    int read(std::vector<byte_t>& buf, const MM_WAV& wav)
    {
        io::binfile f;
        f.open(filename.c_str(), io_read);
        if(!f.is_open()){
            return -1;
        }
        buf.resize(wav.size);
        f.seek(wav.offset, SEEK_SET);
        f.read(&buf[0], wav.size);
        return 0;
    }

};

}//end namespace heros3

#endif //MM_LODFILE_HPP_20181102090040
