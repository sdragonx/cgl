/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 d2dcontext.hpp

 2020-11-20 10:32:09

*/
#ifndef D2DCONTEXT_HPP_20201120103209
#define D2DCONTEXT_HPP_20201120103209

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <cgl/idmap.hpp>

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "psdk/d2d1.lib")
#pragma comment(lib, "psdk/dwrite.lib")
#pragma comment(lib, "winmm.lib")

namespace cgl{


class d2dfont
{
private:
    IDWriteTextFormat* m_textformat;

public:
    vec4ub color;

public:
    d2dfont() : m_textformat(), color()
    {
    }

    ~d2dfont()
    {
    }

    IDWriteTextFormat* handle()const
    {
        return m_textformat;
    }

    int create(const wchar_t* fontname, int fontsize, bool bold = false, bool italic = false, bool underline = false)
    {
        IDWriteFactory *pDWriteFactory;

        // Create a DirectWrite factory.
        if(FAILED(DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown **>(&pDWriteFactory)))
            ){

            CGL_LOG_ERROR("d2dfont: DWriteCreateFactory error.");
            return CGL_ERROR;
        }

        // Create a DirectWrite text format object.
        if(FAILED(pDWriteFactory->CreateTextFormat(
                fontname,
                NULL,
                bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
                italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                fontsize, //font size
                L"", //locale
                &m_textformat
            ))){

            CGL_LOG_ERROR("d2dfont: CreateTextFormat error.");
            return CGL_ERROR;
        }

        pDWriteFactory->Release();

        this->align(CGL_CENTER);
        this->wordwrap(true);

        return CGL_OK;
    }

    void dispose()
    {
        safe_release(m_textformat);
    }

    //文本对齐
    void align(int value)
    {
        //水平对齐
        if((value & CGL_HORIZONTAL) == CGL_HORIZONTAL){
            m_textformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        }
        else if(value & CGL_RIGHT){
            m_textformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
        }
        else{
            m_textformat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        }

        //垂直对齐
        if((value & CGL_VERTICAL) == CGL_VERTICAL){
            m_textformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
        else if(value & CGL_BOTTOM){
            m_textformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
        }
        else{
            m_textformat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }
    }

    //是否换行
    void wordwrap(bool value)
    {
        m_textformat->SetWordWrapping(value ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
    }

};

class d2dtexture
{
private:
    ID2D1RenderTarget* m_renderer;
    IWICImagingFactory* m_imaging;

    typedef idmap<ID2D1Bitmap*> imagelist;

    imagelist m_images;

    friend class d2dcontext;

public:
    d2dtexture() : m_renderer(), m_imaging()
    {
    }

    int init(ID2D1RenderTarget* pRenderTarget)
    {
        // Direct2D factory
        ID2D1Factory *pD2DFactory;
        HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

        if (FAILED(hr)){
            CGL_LOG_ERROR("D2D1CreateFactory error.");
            return CGL_ERROR;
        }

        // Create WIC factory.
        hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_imaging));

        if (FAILED(hr)){
            CGL_LOG_ERROR("D2D1CreateFactory error.");
            return CGL_ERROR;
        }

        m_renderer = pRenderTarget;

        return CGL_OK;
    }

    int open(const wchar_t* filename, int destinationWidth = 0, int destinationHeight = 0)
    {
        int id = 0;
        IWICBitmapDecoder *pDecoder = NULL;
        if(SUCCEEDED(m_imaging->CreateDecoderFromFilename(
            filename,
            NULL,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder))){

            ID2D1Bitmap* pBitmap = this->open(pDecoder, destinationWidth, destinationHeight);

            if(pBitmap){
                id = m_images.allocate();
                m_images[id] = pBitmap;
            }

            safe_release(pDecoder);
        }

        return id;
    }

    ID2D1Bitmap* open(IStream* pStream, int destinationWidth = 0, int destinationHeight = 0)
    {
        ID2D1Bitmap* pBitmap = NULL;
        IWICBitmapDecoder *pDecoder = NULL;

        if(SUCCEEDED(m_imaging->CreateDecoderFromStream(
            pStream,
            NULL,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder))){

            pBitmap = this->open(pDecoder, destinationWidth, destinationHeight);

            safe_release(pDecoder);
        }

        return pBitmap;
    }

    ID2D1Bitmap* items(int index)
    {

    }

    void dispose()
    {
        imagelist::iterator i;
        for(i = m_images.begin(); i!=m_images.end(); ++i){
            safe_release(i->second);
        }
        m_images.clear();
    }

protected:
    ID2D1Bitmap* open(IWICBitmapDecoder* pDecoder, int destinationWidth = 0, int destinationHeight = 0)
    {
        IWICBitmapFrameDecode *pSource = NULL;
        IWICFormatConverter *pConverter = NULL;
        ID2D1Bitmap* pBitmap = NULL;

        // Create the initial frame.
        HRESULT hr = pDecoder->GetFrame(0, &pSource);

        if (SUCCEEDED(hr)){
            // Convert the image format to 32bppPBGRA
            // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
            hr = m_imaging->CreateFormatConverter(&pConverter);

        }

        if (SUCCEEDED(hr)){
            // If a new width or height was specified, create an
            // IWICBitmapScaler and use it to resize the image.
            if (destinationWidth != 0 || destinationHeight != 0){
                UINT originalWidth, originalHeight;
                hr = pSource->GetSize(&originalWidth, &originalHeight);
                if (SUCCEEDED(hr)){
                    if (destinationWidth == 0){
                        FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                        destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
                    }
                    else if (destinationHeight == 0){
                        FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                        destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
                    }

                    IWICBitmapScaler *pScaler = NULL;
                    hr = m_imaging->CreateBitmapScaler(&pScaler);
                    if (SUCCEEDED(hr)){
                        hr = pScaler->Initialize(
                            pSource,
                            destinationWidth,
                            destinationHeight,
                            WICBitmapInterpolationModeCubic
                        );
                    }
                    if (SUCCEEDED(hr)){
                        hr = pConverter->Initialize(
                            pScaler,
                            GUID_WICPixelFormat32bppPBGRA,
                            WICBitmapDitherTypeNone,
                            NULL,
                            0.f,
                            WICBitmapPaletteTypeMedianCut
                        );
                    }

                    safe_release(pScaler);
                }
            }
            else{ // Don't scale the image.
                hr = pConverter->Initialize(
                    pSource,
                    GUID_WICPixelFormat32bppPBGRA,
                    WICBitmapDitherTypeNone,
                    NULL,
                    0.f,
                    WICBitmapPaletteTypeMedianCut
                );
            }
        }

        if (SUCCEEDED(hr)){
            // Create a Direct2D bitmap from the WIC bitmap.
            hr = m_renderer->CreateBitmapFromWicBitmap(pConverter, NULL, &pBitmap);
        }

        safe_release(pConverter);
        safe_release(pSource);

        return pBitmap;
    }
};


class d2dcontext
{
private:
    HWND window;
    ID2D1HwndRenderTarget   *pRenderTarget;
    ID2D1SolidColorBrush* m_brush;

    float m_pen_width;

public:
    d2dfont font;
    vec4ub color;

    d2dtexture images;

public:

    int create(HWND hwnd)
    {
        // Direct2D factory
        ID2D1Factory *pD2DFactory;
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory))){
            CGL_LOG_ERROR("d2dcontext: D2D1CreateFactory error.");
            return CGL_ERROR;
        }

        RECT rect;
        GetClientRect(hwnd, &rect);

        // Create a Direct2D render target.
        D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);
        if (FAILED(pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &pRenderTarget))){

            CGL_LOG_ERROR("d2dcontext: CreateHwndRenderTarget error.");
            return CGL_ERROR;
        }

        pD2DFactory->Release();

        window = hwnd;

        init_context();

        return CGL_OK;
    }

    int create(HDC hdc)
    {

    }

    void dispose()
    {
        safe_release(m_brush);

        font.dispose();
        images.dispose();

        safe_release(pRenderTarget);

    }

    void clear()
    {
        pRenderTarget->BeginDraw();
        pRenderTarget->Clear(D2D1::ColorF(0x0, 1.0f));
    }

    void swap_buffers()
    {
        pRenderTarget->EndDraw();
    }

public:
    void pen_width(float value)
    {
        m_pen_width = value;
    }

public:
    void draw_line(float x1, float y1, float x2, float y2)
    {
        D2D1_POINT_2F p1 = {x1, y1};
        D2D1_POINT_2F p2 = {x2, y2};

        update_brush();
        pRenderTarget->DrawLine(p1, p2, m_brush, m_pen_width, NULL);
    }

    void draw_rect(const D2D_RECT_F& rect)
    {
        update_brush();
        pRenderTarget->DrawRectangle(rect, m_brush, m_pen_width, NULL);
    }

    void draw_rect(float x, float y, float width, float height)
    {
        D2D_RECT_F rect = {x, y, x + width, y + height};
        update_brush();
        pRenderTarget->DrawRectangle(rect, m_brush, m_pen_width, NULL);
    }

    void draw_image(ID2D1Bitmap* bitmap, float x, float y, float width, float height)
    {
        D2D_RECT_F rect = {x, y, x + width, y + height};
        pRenderTarget->DrawBitmap(bitmap, rect);
    }

    void draw_image(int bitmapID, float x, float y, float width, float height)
    {
        D2D_RECT_F rect = {x, y, x + width, y + height};
        ID2D1Bitmap* bitmap = images.m_images[bitmapID];
        pRenderTarget->DrawBitmap(bitmap, rect);
    }

    void print(int x, int y, const wchar_t* text, int length)
    {
        D2D_RECT_F rect = {x, y, x + 300, y + 40};

        ID2D1SolidColorBrush* brush;
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

        draw_rect(rect);

        pRenderTarget->DrawText(
            text,           // Text to render
            length,         // Text length
            font.handle(),  // Text format
            rect,           // The region of the window where the text will be rendered
            brush           // The brush used to draw the text
        );

        brush->Release();
    }

protected:
    void init_context()
    {
        pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_brush);

        images.init(pRenderTarget);
        m_pen_width = 1.0f;
    }

    void update_brush()
    {
        D3DCOLORVALUE c = { color.r, color.g, color.b, color.a };
        m_brush->SetColor(c);
    }

};

}//end namespace cgl

#endif //D2DCONTEXT_HPP_20201120103209
