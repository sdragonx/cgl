/*
Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

d3dcontext.hpp

2020-11-16 06:34:25

*/
#ifndef D3DCONTEXT_HPP_20201116063425
#define D3DCONTEXT_HPP_20201116063425

//#include <cgl/public.h>

#pragma once

//#include <cgl/public.h>
//#include <cgl/vec.hpp>

#include <gl/GL.h>
#include <d3d9.h>
#include <d3dx9math.h>

#include <algorithm>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "d3d9math.lib")

namespace cgl {
namespace graphics {

struct Vertex //顶点结构
{
    Vertex() {}
    Vertex(float vx, float vy, float vz,
        float s, float t, 
        DWORD c, 
        float nx, float ny, float nz) : position(vx, vy, vz), u(s), v(t), color(c)
    {
    }

    glm::vec3 position;
    glm::vec3 normal;
    DWORD color;
    float u, v;

    static const DWORD format;
};
const DWORD Vertex::format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

class d3dtexture
{
private:
    friend class d3dcontext;
    IDirect3DDevice9* device;
    typedef std::map<int, LPDIRECT3DTEXTURE9>::iterator iterator;
    std::map<int, LPDIRECT3DTEXTURE9> items;

public:
    d3dtexture() : device()
    {

    }

    ~d3dtexture()
    {
        this->dispose();
    }

    int loadimage(const char* filename)
    {
        LPDIRECT3DTEXTURE9 tex;
        HRESULT hr = D3DXCreateTextureFromFileExA(device,
            filename,
            D3DX_DEFAULT_NONPOW2,
            D3DX_DEFAULT_NONPOW2,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,//D3DFMT_DXT5 D3DFMT_A8B8G8R8,// D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            NULL,
            NULL, &tex);

        if (hr == D3D_OK) {
            // Set the image states to get a good quality image.
            // 设置放大过滤器为线性过滤器
            device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            // 设置缩小过滤器为线性过滤器
            device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

            D3DSURFACE_DESC desc;
            tex->GetLevelDesc(0, &desc);

            int n = gen_id();
            items[n] = tex;

            return n;
        }
        else {
            return 0;
        }
    }

    void dispose()
    {
        std::map<int, LPDIRECT3DTEXTURE9>::iterator itr;
        for (itr = items.begin(); itr != items.end(); ++itr) {
            itr->second->Release();
        }
        items.clear();
    }

private:
    int gen_id()
    {
        int n = 1 + rand();
        while (items.find(n) != items.end()) {
            n = 1 + rand();
        }
        return n;
    }
};

class dxutils
{
public:
    static D3DPRIMITIVETYPE dxPrimitiveTypeOf(GLenum e)
    {
        switch (e) {
        case GL_POINTS:
            return D3DPT_POINTLIST; //不支持
        case GL_LINES:
            return D3DPT_LINELIST;
        case GL_LINE_STRIP:
            return D3DPT_LINESTRIP;
        case GL_TRIANGLES:
            return D3DPT_TRIANGLELIST;
        case GL_TRIANGLE_STRIP:
            return D3DPT_TRIANGLESTRIP;
        case GL_TRIANGLE_FAN:
            return D3DPT_TRIANGLEFAN;
        default:
            return D3DPRIMITIVETYPE(0);
        }
    }

    static size_t dxPrimitiveCount(GLenum primitive_type, size_t size)
    {
        switch (primitive_type) {
        case GL_POINTS:
            return size;
        case GL_LINES:
            return size / 2;
        case GL_LINE_STRIP:
            return size - 1;
        case GL_TRIANGLES:
            return size / 3;
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
            return size - 2;
        default:
            return 0;
        }
    }

    static size_t dxVertexCount(uint16_t* indices, size_t size)
    {
        return *std::max_element(indices, indices + size) + 1;
    }

    static size_t dxVertexCount(uint32_t* indices, size_t size)
    {
        return *std::max_element(indices, indices + size) + 1;
    }

    static DWORD dxBlendModeOf(GLenum e)
    {
        switch (e) {
        case GL_ZERO:
            return D3DBLEND_ZERO;
        case GL_ONE:
            return D3DBLEND_ONE;
        case GL_SRC_COLOR:
            return D3DBLEND_SRCCOLOR;
        case GL_ONE_MINUS_SRC_COLOR:
            return D3DBLEND_INVSRCCOLOR;
        case GL_SRC_ALPHA:
            return D3DBLEND_SRCALPHA;
        case GL_ONE_MINUS_SRC_ALPHA:
            return D3DBLEND_INVSRCALPHA;
        case GL_DST_ALPHA:
            return D3DBLEND_DESTALPHA;
        case GL_ONE_MINUS_DST_ALPHA:
            return D3DBLEND_INVDESTALPHA;
        case GL_DST_COLOR:
            return D3DBLEND_DESTCOLOR;
        case GL_ONE_MINUS_DST_COLOR:
            return D3DBLEND_INVDESTCOLOR;
        case GL_SRC_ALPHA_SATURATE:
            return D3DBLEND_SRCALPHASAT;
        default:
            return 0;
        }
    }
};

class d3dbuffer
{
public:
    /*
    //顶点缓存
    IDirect3DVertexBuffer9* vbo = nullptr;

    //创建顶点缓存和索引缓存
    device->CreateVertexBuffer(sizeof(Vertex) * size, D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &vbo, 0);

    //访问顶点缓存，将Cube顶点数据写入
    Vertex* vertices;
    vbo->Lock(0, 0, (void**)&vertices, 0);
    memcpy(vertices, vs, sizeof(Vertex) * size);
    vbo->Unlock();

    device->SetStreamSource(0, vbo, 0, sizeof(Vertex));
    device->SetFVF(Vertex::FVF);

    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, size, 0, size / 3);

    vbo->Release();
    */
    /*
    //顶点缓存
    IDirect3DVertexBuffer9* vbo = nullptr;

    //索引缓存
    IDirect3DIndexBuffer9*  ibo = nullptr;

    //查找最大索引值，得到需要的vs大小
    size_t vs_size = max_element(ids, size);

    //创建顶点缓存和索引缓存
    device->CreateVertexBuffer(sizeof(Vertex) * vs_size, D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &vbo, 0);
    device->CreateIndexBuffer(sizeof(WORD) * size, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ibo, 0);

    //访问顶点缓存，将Cube顶点数据写入
    Vertex* vertices;
    vbo->Lock(0, 0, (void**)&vertices, 0);
    memcpy(vertices, vs, sizeof(Vertex) * vs_size);
    vbo->Unlock();

    //访问索引缓存，将三角形单元的顶点构成数据写入
    WORD* indices = 0;
    ibo->Lock(0, 0, (void**)&indices, 0);
    memcpy(indices, ids, sizeof(uint16_t) * size);
    ibo->Unlock();

    device->SetStreamSource(0, vbo, 0, sizeof(Vertex));
    device->SetIndices(ibo);
    device->SetFVF(Vertex::FVF);

    D3DXMATRIX m;
    D3DXMatrixMultiply(&m, &matView, &matCamera);
    device->SetTransform(D3DTS_WORLD, &m);

    device->BeginScene();

    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vs_size, 0, size / 3);

    device->EndScene();

    vbo->Release();
    ibo->Release();
    */
};

class d3dcontext
{
public:
    IDirect3D9 *d3d9;
    IDirect3DDevice9 *device;

    SIZE m_size;

    D3DXMATRIX matView;
    std::vector<D3DXMATRIX> viewStack;

    D3DXMATRIX matCamera;

    d3dtexture textures;

public:
    d3dcontext() : d3d9(), device()
    {

    }

    ~d3dcontext()
    {
        this->dispose();
    }

    int create(HWND hwnd, D3DDEVTYPE deviceType = D3DDEVTYPE_HAL, bool fullscreen = false)
    {
        d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

        D3DCAPS9 caps;
        d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
        int vp = 0;
        if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT){
            vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }
        else{
            vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }

        RECT rect;
        GetClientRect(hwnd, &rect);
        m_size.cx = rect.right - rect.left;
        m_size.cy = rect.bottom - rect.top;

        D3DPRESENT_PARAMETERS d3dpp;
        d3dpp.BackBufferWidth = m_size.cx;
        d3dpp.BackBufferHeight = m_size.cy;
        d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
        d3dpp.BackBufferCount = 1;
        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
        d3dpp.MultiSampleQuality = 0;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = hwnd;
        d3dpp.Windowed = fullscreen ? FALSE : TRUE;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
        d3dpp.Flags = 0;
        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        HRESULT hr = d3d9->CreateDevice(
            D3DADAPTER_DEFAULT,
            deviceType,
            hwnd,
            vp,
            &d3dpp,
            &device);

        if (FAILED(hr)){
            d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
            hr = d3d9->CreateDevice(
                D3DADAPTER_DEFAULT,
                deviceType,
                hwnd,
                vp,
                &d3dpp,
                &device);

            if (FAILED(hr)){
                d3d9->Release();
                MessageBox(0, TEXT("CreateDevice() - FAILED"), 0, 0);
                return -1;
            }
        }

        textures.device = device;

        init_context();

        return 0;
    }

    void dispose()
    {
        textures.dispose();

        if (device) {
            device->Release();
            device = nullptr;
        }

        if (d3d9) {
            d3d9->Release();
            d3d9 = nullptr;
        }
    }

    void init_context()
    {
        //设置填充状态
        this->fill_mode(GL_FILL);

        //开启混合
        device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

        blendfunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_COLOR, GL_SRC_ALPHA);

        this->perspective(60, m_size.cx, m_size.cy, 1.0f, 1000.0f);

        this->identity();

        //纹理过滤
        device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

        // Set default rendering states.
        device->SetRenderState(D3DRS_LIGHTING, FALSE);

        //顶点颜色
        device->SetRenderState(D3DRS_COLORVERTEX, TRUE);
        //顶点材质
        device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

        //device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        //device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }

    void cull_face(bool value)
    {
        device->SetRenderState(D3DRS_CULLMODE, value ? D3DCULL_CW : D3DCULL_NONE);
    }

    void fill_mode(GLenum mode)
    {
        //设置绘制状态
        switch (mode){
        case GL_POINT:
            device->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
            break;
        case GL_LINE:
            device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
            break;
        case GL_FILL:
            device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
            break;
        default:
            break;
        }
    }

    void blendfunc(GLenum dest, GLenum source)
    {
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
        device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
        device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA);
    }

    void blendfunc(GLenum destColor, GLenum destAlpha, GLenum sourceColor, GLenum sourceAlpha)
    {
        device->SetRenderState(D3DRS_DESTBLEND, dxutils::dxBlendModeOf(destColor));
        device->SetRenderState(D3DRS_DESTBLENDALPHA, dxutils::dxBlendModeOf(destAlpha));
        device->SetRenderState(D3DRS_SRCBLEND, dxutils::dxBlendModeOf(sourceColor));
        device->SetRenderState(D3DRS_SRCBLENDALPHA, dxutils::dxBlendModeOf(sourceAlpha));
    }

    /*
    纹理坐标通常在0-1之间，当处理那些不在这个范围内的纹理坐标时的策略就是address mode

    IDirect3DDevice9::SetSamplerState(D3DSAMP_ADDRESSU/D3DSAMP_ADDRESSV/D3DSAMP_ADDRESSW,)

    IDirect3DDevice9::SetSamplerState（0-7)，场合参数，filter参数）

    场合参数

    D3DSAMP_MAGFILTER,一个纹理点（texel）需要被映射到很多pixel的时候放大
    D3DSAMP_MINFILTER,一个pixel采样到很多纹理点的时候，放小

    D3DSAMP_MIPFILTER mipmap的时候


    //平面着色（flat shading）
    Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);

    //Gouraud着色（smooth shading）
    Device->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);


    最近点采样
    g_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    g_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

    线性纹理过滤
    g_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    g_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

    各项异性纹理过滤
    g_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);
    DWORD get_max_anisotropy(IDirect3DDevice9* device)
    {
    D3DCAPS9 caps;
    device->GetDeviceCaps(&caps);
    return caps.MaxAnisotropy;
    }

    */
protected://draw
    void begin_paint()
    {
        device->SetFVF(Vertex::format);

        D3DXMATRIX m;
        D3DXMatrixMultiply(&m, &matView, &matCamera);
        device->SetTransform(D3DTS_WORLD, &m);

        device->BeginScene();
    }

    void end_paint()
    {
        device->EndScene();
    }

public://draw
    void clear()
    {
        device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF007FFF, 1.0f, 0);
    }

    void swap_buffers()
    {
        device->Present(0, 0, 0, 0);
    }

    void draw_arrays(GLenum shape, Vertex* vs, size_t size)
    {
        this->begin_paint();
        device->DrawPrimitiveUP(
            dxutils::dxPrimitiveTypeOf(shape),
            dxutils::dxPrimitiveCount(shape, size),
            vs,
            sizeof(Vertex));
        this->end_paint();
    }

    void draw_elements(GLenum shape, Vertex* vs, uint16_t *indices, size_t size)
    {
        this->begin_paint();
        device->DrawIndexedPrimitiveUP(
            dxutils::dxPrimitiveTypeOf(shape),
            0,
            dxutils::dxVertexCount(indices, size),
            dxutils::dxPrimitiveCount(shape, size),
            indices,
            D3DFMT_INDEX16,
            vs,
            sizeof(Vertex));
        this->end_paint();
    }


public://matrix
    void identity()
    {
        D3DXMatrixIdentity(&matView);
    }

    void translate(float x, float y, float z)
    {
        D3DXMATRIX m;
        D3DXMatrixTranslation(&m, x, y, z);
        D3DXMatrixMultiply(&matView, &matView, &m);
    }

    void scale(float x, float y, float z)
    {
        D3DXMATRIX m;
        D3DXMatrixScaling(&m, x, y, z);
        D3DXMatrixMultiply(&matView, &matView, &m);
    }

    void rotate(float angle, float x, float y, float z)
    {
        D3DXVECTOR3 v(x, y, z);
        D3DXMATRIX m;
        D3DXMatrixRotationAxis(&m, &v, angle);
        D3DXMatrixMultiply(&matView, &matView, &m);
    }

    void lookat(float atX, float atY, float atZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ)
    {
        return this->lookat(D3DXVECTOR3(atX, atY, atZ), D3DXVECTOR3(targetX, targetY, targetZ), D3DXVECTOR3(upX, upY, upZ));
    }

    void lookat(const D3DXVECTOR3& at, const D3DXVECTOR3& target, const D3DXVECTOR3& up)
    {
        D3DXMatrixLookAtLH(&matCamera, &at, &target, &up);
    }

    void push_matrix()
    {
        viewStack.push_back(matView);
    }

    void pop_matrix()
    {
        if (!viewStack.empty()) {
            matView = viewStack.back();
            viewStack.pop_back();
        }
    }

    void perspective(float fov, int width, int height, float zNear = 1.0f, float zFar = 1000.0f)
    {
        D3DXMATRIX m;
        D3DXMatrixPerspectiveFovLH(&m, D3DX_PI * fov / 180.0f, (float)width / (float)height, zNear, zFar);
        device->SetTransform(D3DTS_PROJECTION, &m);
    }

    void ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        D3DXMATRIX m;
        D3DXMatrixOrthoLH(&m, right - left, bottom - top, zNear, zFar);
        device->SetTransform(D3DTS_PROJECTION, &m);
    }

public://texture
    void bind_texture(int tex)
    {
        d3dtexture::iterator itr = textures.items.find(tex);
        if (itr != textures.items.end()) {
            device->SetTexture(0, itr->second);
        }
    }
    
};


void draw_cube(d3dcontext& dc, float x, float y, float z, float size)
{
    typedef Vertex vertex_type;
    DWORD color = D3DCOLOR_RGBA(255, 255, 255, 64);
    Vertex vs[] = {
        //front
        vertex_type(x - size, y + size, z + size, 0.0f, 0.0f, color, 0.0f, 0.0f, 1.0f),
        vertex_type(x - size, y - size, z + size, 0.0f, 1.0f, color, 0.0f, 0.0f, 1.0f),
        vertex_type(x + size, y - size, z + size, 1.0f, 1.0f, color, 0.0f, 0.0f, 1.0f),
        vertex_type(x + size, y + size, z + size, 1.0f, 0.0f, color, 0.0f, 0.0f, 1.0f),

        //back
        vertex_type(x + size, y + size, z - size, 0.0f, 0.0f, color, 0.0f, 0.0f, -1.0f),
        vertex_type(x + size, y - size, z - size, 0.0f, 1.0f, color, 0.0f, 0.0f, -1.0f),
        vertex_type(x - size, y - size, z - size, 1.0f, 1.0f, color, 0.0f, 0.0f, -1.0f),
        vertex_type(x - size, y + size, z - size, 1.0f, 0.0f, color, 0.0f, 0.0f, -1.0f),

        //left
        vertex_type(x - size, y + size, z - size, 0.0f, 0.0f, color, -1.0f, 0.0f, 0.0f),
        vertex_type(x - size, y - size, z - size, 0.0f, 1.0f, color, -1.0f, 0.0f, 0.0f),
        vertex_type(x - size, y - size, z + size, 1.0f, 1.0f, color, -1.0f, 0.0f, 0.0f),
        vertex_type(x - size, y + size, z + size, 1.0f, 0.0f, color, -1.0f, 0.0f, 0.0f),

        //right
        vertex_type(x + size, y + size, z + size, 0.0f, 0.0f, color, 1.0f, 0.0f, 0.0f),
        vertex_type(x + size, y - size, z + size, 0.0f, 1.0f, color, 1.0f, 0.0f, 0.0f),
        vertex_type(x + size, y - size, z - size, 1.0f, 1.0f, color, 1.0f, 0.0f, 0.0f),
        vertex_type(x + size, y + size, z - size, 1.0f, 0.0f, color, 1.0f, 0.0f, 0.0f),

        //top
        vertex_type(x - size, y + size, z - size, 0.0f, 0.0f, color, 0.0f, 1.0f, 0.0f),
        vertex_type(x - size, y + size, z + size, 0.0f, 1.0f, color, 0.0f, 1.0f, 0.0f),
        vertex_type(x + size, y + size, z + size, 1.0f, 1.0f, color, 0.0f, 1.0f, 0.0f),
        vertex_type(x + size, y + size, z - size, 1.0f, 0.0f, color, 0.0f, 1.0f, 0.0f),

        //bottom
        vertex_type(x - size, y - size, z + size, 0.0f, 0.0f, color, 0.0f, -1.0f, 0.0f),
        vertex_type(x - size, y - size, z - size, 0.0f, 1.0f, color, 0.0f, -1.0f, 0.0f),
        vertex_type(x + size, y - size, z - size, 1.0f, 1.0f, color, 0.0f, -1.0f, 0.0f),
        vertex_type(x + size, y - size, z + size, 1.0f, 0.0f, color, 0.0f, -1.0f, 0.0f),
    };

    static uint16_t CUBE_INDICES[36] = {
        0,1,2,0,2,3,  4,5,6,4,6,7,  8,9,10, 8,10,11,  12,13,14,12,14,15,  16,17,18,16,18,19,  20,21,22,20,22,23
    };

    dc.draw_elements(GL_TRIANGLES, vs, CUBE_INDICES, 36);
}

}//end namespace graphics
}//end namespace cgl

#endif //D3DCONTEXT_HPP_20201116063425