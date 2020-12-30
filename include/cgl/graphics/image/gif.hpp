/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gif.hpp

 2020-06-13 16:15:22

 gif to RGBA images

 gif是以流的方式存储的

*/
#ifndef GIF_HPP_20200613161522
#define GIF_HPP_20200613161522

#include <cgl/public.h>

namespace cgl{
namespace graphics{

#pragma pack(push, 1)

struct gif_header
{
	char		flag[6];
	uint16_t	width;
	uint16_t	height;
	byte_t		color_info;
	byte_t		background;
	byte_t		aspect;     //像素宽高比
};

//data block
struct gif_block{
	byte_t size;
	byte_t data[0];
};

//gif graphic control
struct gif_graphic_control{
	byte_t block_size;
	byte_t reserved : 3;
	byte_t disposal_method : 3; //(Packed & 0x1C) >> 2
	byte_t user_input  : 1;
	byte_t transparent : 1;     //(Packed & 0x1)
	uint16_t delay_time;        //帧停留时间 1/100秒
	byte_t transparent_color;   //transparent color index
	byte_t terminator;          //0
};

struct gif_application{
	byte_t size;
	byte_t identifier[8];       //application indentifier
	byte_t auth_code[3];        //application authentication code
};

//gif image descriptor
struct gif_desc
{
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	byte_t   color_table_size : 3;
	byte_t   reserved : 2;
	byte_t   sorted : 1;
	byte_t   interlaced : 1;
	byte_t   color_table : 1;       //local color table flag
};

#pragma pack(pop)

inline bool is_transparent(gif_graphic_control& gfx, byte_t c)
{
    //gfx->transparent貌似无效
    //gfx->disposal_method == 1 && gfx->transparent_color == *sp && gfx->transparent
    return gfx.disposal_method == 1 && gfx.transparent_color == c;
}


uint32_t code_mask[13] = { 0, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF };

const MAX_CODES = 4096;

class lzw_context
{
private:

    int curr_size;
    int clear;      //清除
    int ending;     //结束
    int newcodes;
    int top_slot;
    int slot;
    int navail_bytes;
    int nbits_left;
    byte_t	byte;
    byte_t	byte_buff[257];
    byte_t *pbytes;

    byte_t   stack[MAX_CODES];
    byte_t   suffix[MAX_CODES];
    uint16_t prefix[MAX_CODES];

public:
    lzw_context()
    {
        navail_bytes = 0;
        nbits_left = 0;
    }

    void init(int code_size)
    {

    }

    int next_code(const byte_t* &data)
    {
        int i, t;
        uint32_t ret;

        //20050102: Tests for IL_EOF were added because this function
        //crashed sometimes if igetc() returned IL_EOF
        //(for example "table-add-column-before-active.gif" included in the
        //mozilla source package)

        if (!nbits_left) {
            if (navail_bytes <= 0) {
                pbytes = byte_buff;
                navail_bytes = *data++;//igetc();

                if(navail_bytes == IL_EOF) {
                    return ending;
                }

                if (navail_bytes) {
                    for (i = 0; i < navail_bytes; i++) {
    //					if((t = igetc()) == IL_EOF) {
    //						success = IL_FALSE;
    //						return ending;
    //					}
                        t = *data++;
                        byte_buff[i] = t;
                    }
                }
            }
            byte = *pbytes++;
            nbits_left = 8;
            navail_bytes--;
        }

        ret = byte >> (8 - nbits_left);
        while (curr_size > nbits_left) {
            if (navail_bytes <= 0) {
                pbytes = byte_buff;
                navail_bytes = *data++;//igetc();

                if(navail_bytes == IL_EOF) {
                    return ending;
                }

                if (navail_bytes) {
                    for (i = 0; i < navail_bytes; i++) {
                        /*
                        if((t = igetc()) == IL_EOF) {
                            success = IL_FALSE;
                            return ending;
                        }
                        */
                        t = *data++;
                        byte_buff[i] = t;
                    }
                }
            }
            byte = *pbytes++;
            ret |= byte << nbits_left;
            nbits_left += 8;
            navail_bytes--;
        }
        nbits_left -= curr_size;

        return (ret & code_mask[curr_size]);
    }

    //返回当前data地址
    const byte_t* decode(
        byte_t* image,      //画布
        const byte_t *data, //图像数据
        size_t size,        //图像数据大小
        int width,          //宽度
        int height,         //高度
        int stride,         //画布行宽度
        gif_graphic_control *gfx)
    {
        byte_t *sp;
        int	code;
        int fc;
        int oc;
        int c;

        int x = 0;
        int y = 0;
        byte_t* line = image;


//        byte_t   stack[MAX_CODES];
//        byte_t   suffix[MAX_CODES];
//        uint16_t prefix[MAX_CODES];

        int code_size;

        code_size = *data++;

        if (code_size < 2 || 9 < code_size) {
            return null;
        }

        curr_size = code_size + 1;
        top_slot = 1 << curr_size;
        clear = 1 << code_size;
        ending = clear + 1;
        slot = newcodes = ending + 1;
        navail_bytes = nbits_left = 0;

        oc = fc = 0;
        sp = stack;

        while ((c = next_code(data)) != ending && y < height) {
            if (c == clear){//重启代码页
                curr_size = code_size + 1;
                slot = newcodes;
                top_slot = 1 << curr_size;
                while ((c = next_code(data)) == clear); //skip clear
                if (c == ending){
                    break;
                }
                if (c >= slot){
                    c = 0;
                }
                oc = fc = c;

                //透明色
                if(is_transparent(*gfx, c)){
                }
                else if (x < width){//put pixel
                    line[x] = c;
                }

                if(++x == width){
                    //DataPtr += Stride;//换行
                    x = 0;
                    ++y;
                    line += stride;
                    if (y >= height) {
                       return null;
                    }
                }
            }
            else
            {
                code = c;
                //BG-2007-01-10: several fixes for incomplete GIFs
                if (code >= slot){
                    code = oc;
                    if (sp >= stack + MAX_CODES) {
                       //cleanUpGifLoadState();
                       return null;
                    }
                    *sp++ = fc;
                }

                if (code >= MAX_CODES){
                    return null;
                }

                while (code >= newcodes)
                {
                    if (sp >= stack + MAX_CODES){
                        return null;
                    }
                    *sp++ = suffix[code];
                    code = prefix[code];
                }

                if (sp >= stack + MAX_CODES) {
                    return null;
                }

                *sp++ = (byte_t)code;
                if (slot < top_slot)
                {
                    fc = code;
                    suffix[slot]   = fc;
                    prefix[slot++] = oc;
                    oc = c;
                }
                if (slot >= top_slot && curr_size < 12)
                {
                    top_slot <<= 1;
                    curr_size++;
                }
                while (sp > stack)
                {
                    sp--;
                    if (is_transparent(*gfx, *sp)){
                    }
                    else if (x < width){
                        line[x] = *sp;
                    }

                    if(++x == width){
                        x = 0;
                        y = (y + 1) % height;
                        line += stride;
                    }
                }
            }
        }
        //CGL_LOG("decompressed = %i", i);
        return data;
    }
};



class gifimage
{
public:
    typedef memblock<byte_t> buffer_t;
    typedef memblock<vec3ub> palette_type;
    typedef memblock<vec4ub> image_type;

    enum{
        GIFNULL,
        GIF87A,
        GIF89A
    };

    struct g
    {
        palette_type pal;
        buffer_t data;
        int x, y, width, height;
    };

protected:
    int type;
    gif_header header;
    gif_graphic_control gfx;

public:
    std::vector<image_type*> images;

public:
    int open(const char* filename)
    {
        buffer_t buf;
        io::load_file(buf, filename);

        this->close();

        return decode_gif(&buf[0], buf.size());

//        io::binfile f;
//        f.open(filename, io_read);
//        decode_gif(f);

    }

    int open_memory(const byte_t* data, size_t size)
    {
        this->close();
        return decode_gif(data, size);
    }

    void close()
    {
        type = GIFNULL;
        memset(&header, 0, sizeof(header));
        memset(&gfx, 0, sizeof(gfx));
        delete_all(images);
        images.clear();
    }

    int width()const
    {
        return header.width;
    }

    int height()const
    {
        return header.height;
    }

    bool check_format(buffer_t& buf)
    {
        const header_size = 6;
        if (!strnicmp(&buf[0], "GIF87A", header_size)){
            type = GIF87A;
            return true;
        }
        if (!strnicmp(&buf[0], "GIF89A", header_size)){
            type = GIF89A;
            return true;
        }
        return false;
    }

    /*
    bool decode_gif(io::unifile& f)
    {
        //read header
        f.read(&header.flag, 6);
        f.readle<uint16_t>(header.width);
        f.readle<uint16_t>(header.height);

        header.color_info = f.getc();
        header.background = f.getc();
        header.aspect = f.getc();

        if (!strnicmp(header.flag, "GIF87A", 6)) {
            GifType = GIF87A;
            CGL_LOG("GIF87A");
        }
        else if (!strnicmp(header.flag, "GIF89A", 6)) {
            GifType = GIF89A;
            CGL_LOG("GIF89A");
        }
        else {
            return false;
        }

//        pixelimage image;
//        image.create(header.Width, header.Height, CGL_INDEX, false, NULL, NULL);

        //iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

        size_t size;

        // Check for a global colour map.
        if (header.color_info & (1 << 7)) {
            size = (1 << ((header.color_info & 0x7) + 1));
            global_pal.resize(size);
            f.read(&global_pal[0], size * sizeof(vec3ub));
        }

        //decode_images(images, header, global_pal);

        buffer_t buf;
        size = f.size() - f.tell();
        buf.resize(size);
        f.read(&buf[0], size);

        buffer_t image;
        //image.create(header.width, header.height, CGL_INDEX, false);
        image.resize(header.width * header.height);

        byte_t *begin = &buf[0];
        byte_t *end = begin + size;

        while(begin < end){
            int n = decode_image(image, begin, end - begin, global_pal);
            if( n > 0){
                images.push_back(pixelimage());
                images.back().create(header.width, header.height, CGL_RGBA);

                begin += n;
            }
            else if(n <= 0){
                break;
            }
        }

        CGL_LOG("decode images: %i", images.size());

//        if (!GetImages(f, GlobalPal, &header))
//            return IL_FALSE;
//
//        if (GlobalPal.Palette && GlobalPal.PalSize)
//            ifree(GlobalPal.Palette);
//        GlobalPal.Palette = NULL;
//        GlobalPal.PalSize = 0;

        //return ilFixImage();
        return true;
    }
    */

    bool decode_gif(const byte_t* data, size_t size)
    {
        //read header
        memcpy(&header, data, sizeof(gif_header));
        data += sizeof(gif_header);
        size -= sizeof(gif_header);

        if (!strnicmp(header.flag, "GIF87A", 6)) {
            type = GIF87A;
            CGL_LOG_DEBUG("gif: GIF87A");
        }
        else if (!strnicmp(header.flag, "GIF89A", 6)) {
            type = GIF89A;
            CGL_LOG_DEBUG("gif: GIF89A");
        }
        else {
            return false;
        }

        CGL_LOG_DEBUG("gif: size = %ix%i", header.width, header.height);

        //iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

        palette_type globalpal;
        palette_type localpal;
        size_t n;

        // Check for a global colour map.
        if (header.color_info & (1 << 7)) {
            n = (1 << ((header.color_info & 0x7) + 1));
            //CGL_LOG("global palette size %i.", n);
            globalpal.resize(n);
            n *= sizeof(vec3ub);
            memcpy(&globalpal[0], data, n);
            data += n;
            size -= n;
        }

        buffer_t image;
        image.resize(header.width * header.height);

        const byte_t *begin = data;
        const byte_t *end = begin + size;

        int count = 0;
        while(begin < end){
            ++count;
            int n = decode_image(image, begin, end - begin, localpal);

            if(n <= 0){
                break;
            }
            else{
                begin += n;
                image_type* image32 = new image_type;

                image32->resize(header.width * header.height);

                palette_type& pal = localpal.empty() ? globalpal : localpal;
                draw_image((uint32_t*)image32->data(), image, pal);

                images.push_back(image32);
            }
        }

        CGL_LOG_DEBUG("gif: frames %i", images.size());

        return true;
    }

    //解压一个图片，包括图片数据(和调色板数据)
    //返回数据长度
    //返回0表示结束
    int decode_image(buffer_t& image, const byte_t* data, size_t size, palette_type& localpal)
    {
        const byte_t *begin = data;
        const byte_t *end = begin + size;

        gif_application *app;
        gif_desc  *desc;
        gif_block *block;

        lzw_context lzw;

        //CGL_LOG("清理画布 %i", gfx.disposal_method);
        switch(gfx.disposal_method){
        case 0:
            break;
        case 1:     //保留原图像
            //break;
        case 2:     //背景色清理
        case 3:     //上一次状态
            if(gfx.transparent){
                std::fill(image.begin(), image.end(), gfx.transparent_color);
            }
            else{
                std::fill(image.begin(), image.end(), header.background);
            }
            break;
        default:
            break;
        }

        byte_t *buf = &image[0];
        while(begin < end){
            switch(*begin){
            case 0x00:  // do nothing
                ++begin;
                //CGL_LOG("gif: 0x%x record.", 0x00);
                break;
            case 0x21:  //'!' application ext
                ++begin;
                switch(*begin){
                case 0xF9:
                    //CGL_LOG("graphics control extension");
                    ++begin;
                    //gfx = (gif_graphic_control*)begin;
                    memcpy(&gfx, begin, sizeof(gif_graphic_control));
                    begin += sizeof(gif_graphic_control);
                    break;
                case 0xFE:
                    CGL_LOG("comments extension");
                    ++begin;
                    block = (gif_block*)begin;
                    begin += block->size;
                    //print((wchar_t*)block->data);
                    begin += 1;  //0x00 BlockTerminator
                    break;
                case 0xFF:
                    //CGL_LOG("application extension");
                    ++begin;
                    app = (gif_application*)begin;
                    begin += sizeof(gif_application);
                    block = (gif_block*)begin;//appData
                    begin += 1 + block->size + 1;
                    break;
                default:
                    CGL_LOG("gif: unknown ext record 0x%x.", *begin);
                    return -1;
                }
                break;
            case 0x2c:  //','image descriptor
                ++begin;
                desc = (gif_desc*)begin;
                begin += sizeof(gif_desc);

                //CGL_LOG("image descriptor x=%i y=%i w=%i h=%i", desc->x, desc->y, desc->width, desc->height);
                if (desc->color_table == 1) {
                    size_t palsize = 1 << (desc->color_table_size + 1);
                    //CGL_LOG("local color table %i", palsize);
                    localpal.resize(palsize);
                    palsize *= sizeof(vec3ub);
                    memcpy(&localpal[0], begin, palsize);
                    begin += palsize;
                }
                else{
                    localpal.resize(0);
                }

                buf += desc->y * header.width;
                buf += desc->x;

                begin = lzw.decode(buf, begin, end - begin, desc->width, desc->height, header.width, &gfx);

                return begin - data;
            case 0x3B:  //';' 结束符
                CGL_LOG_DEBUG("gif: decompress end.");
                ++begin;
                return 0;
            default:
                // consider data block and skip
                CGL_LOG("gif: unknown record 0x%x.", *begin++);
                return -1;
            }
        }

        return -1;
    }

    //index to RGBA
    void draw_image(uint32_t* p, buffer_t& buf, const palette_type& pal)
    {
        vec3ub c;
        for(size_t i=0; i<buf.size(); ++i){
            //if (gfx.disposal_method == 1 && gfx.transparent_color == buf[i]) && gfx.transparent){
            if (gfx.disposal_method == 1 && gfx.transparent_color == buf[i]){
                *p++ = RGBA(255, 255, 255, 0);
                //++p;
                continue;
            }
            c = pal[ buf[i] ];
            *p++ = RGBA(c.r, c.g, c.b, 0xFF);
        }
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //GIF_HPP_20200613161522

/*
    gif.open("a.gif");
//    gif.open("本地调色板.gif");
//    gif.open_memory(gif_angel, sizeof(gif_angel));
//    gif.open("局部更新.gif");

    Image1->Picture->Bitmap->AlphaFormat = afDefined;
    //Image1->Picture->Bitmap->TransparentMode = tmFixed;
    //Image1->Picture->Bitmap->Transparent = true;
    Image1->Transparent = true;
    TImage_Resize(Image1, gif.width(), gif.height());


    ListBox1->Items->Clear();
    for(size_t i=0; i<gif.images.size(); ++i){
        ListBox1->Items->Add(i);
    }
*/
