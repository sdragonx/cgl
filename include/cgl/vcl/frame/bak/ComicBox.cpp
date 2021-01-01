//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ComicBox.h"
#include <cgl/algorithm/zoom2d.hpp>
#include <cgl/graphics/gdi/alphablend.hpp>
#include <cgl/graphics/gdi/bitmap.hpp>
#include <cgl/graphics/gdicontext.hpp>
#include <cgl/vcl.hpp>

using namespace cgl::graphics;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TComicBox *ComicBox;

void draw_polygon(TCanvas* dc, const std::vector< vec2i >& ls, int ox, int oy)
{
    if(ls.empty()){
        return ;
    }
    MoveToEx(dc->Handle, ox + ls[0].x, oy + ls[0].y, NULL);
    for(size_t i=1; i<ls.size(); ++i){
        LineTo(dc->Handle, ox + ls[i].x, oy + ls[i].y);
    }
}

bool IsValidIndex(int n)
{
    return n >= 0;
}
//---------------------------------------------------------------------------
__fastcall TComicBox::TComicBox(TComponent* Owner)
    : TFrame(Owner)
{
    //this->DoubleBuffered = true;
    pnlCanvas->DoubleBuffered = true;
    pnlCanvas->Align = alClient;
    pnlCanvas->WindowProc = PanelWndProc;
    pnlCanvas->Realign();
    PaintBox->Align = alClient;
    pntRuleLeft->Font->Orientation = 900;
    FSize = vec2i(512, 512);
    FZoom = 1.0f;
    FButton = TMouseButton(-1);
    FViewPort = vec4i(0, 0, PaintBox->ClientWidth, PaintBox->ClientHeight);
    FBounds = vec4i((PaintBox->ClientWidth - FSize.width) / 2, (PaintBox->ClientHeight - FSize.height) / 2, FSize.width, FSize.height);
//    UpdateBounds();
    FSurface = new Graphics::TBitmap;
    ResizeImage(FSize.width, FSize.height);
    PenMode = 0;

    PointIndex = -1;
    HotPoint = -1;
}
//---------------------------------------------------------------------------

void __fastcall TComicBox::WndProc(TMessage& msg)
{
    switch(msg.Msg){
    case WM_CREATE:
        break;
    case WM_DESTROY:
        delete FSurface;
        break;
    case WM_GETDLGCODE://让窗体接受方向键和TAB键
        msg.Result = DLGC_WANTALLKEYS|DLGC_WANTARROWS|DLGC_WANTTAB;
        break;
    case WM_ERASEBKGND:
        msg.Result = 0;
        return ;
    case WM_MOUSEWHEEL:
        //TPoint(msg.LParamLo, msg.LParamHi)屏幕坐标？
        this->DoMouseWheel(TShiftState(), msg.WParam, toTPoint(FMouse));
        break;
    }
    this->Dispatch(&msg);
}

void __fastcall TComicBox::PanelWndProc(TMessage& msg)
{
    switch(msg.Msg){
    case WM_CREATE:
        break;
    case WM_DESTROY:
        //delete FSurface;
        break;
    case WM_GETDLGCODE://让窗体接受方向键和TAB键
        //msg.Result = DLGC_WANTALLKEYS|DLGC_WANTARROWS|DLGC_WANTTAB;
        break;
    case WM_ERASEBKGND:
        msg.Result = 0;
        return ;
    case WM_MOUSEWHEEL:
        //TPoint(msg.LParamLo, msg.LParamHi)屏幕坐标？
        //this->DoMouseWheel(TShiftState(), msg.WParam, toTPoint(FMouse));
        break;
    }
    pnlCanvas->Dispatch(&msg);
}

void __fastcall TComicBox::Resize(void)
{
    vec2i oldCenter = FViewPort.center();
    FViewPort = vec4i(0, 0, pnlCanvas->ClientWidth, pnlCanvas->ClientHeight);
    vec2i newCenter = FViewPort.center();
    FBounds.x += newCenter.x - oldCenter.x;
    FBounds.y += newCenter.y - oldCenter.y;
    if(FCanvasUpdate)FCanvasUpdate(this);
//    UpdateBounds();
}

//转换窗口坐标为画板坐标
vec2i TComicBox::TransformPoint(int x, int y)
{
    x -= FBounds.x;
    y -= FBounds.y;
    x /= FZoom;
    y /= FZoom;
    return vec2i(x, y);
}

void TComicBox::ResizeImage(int Width, int Height)
{
    FSize = vec2i(Width, Height);
    FSurface->PixelFormat = pf32bit;
    FSurface->Width = Width;
    FSurface->Height = Height;
}

void TComicBox::DoZoomChanged()
{
    FPoints.resize(FOrgPoints.size());
    for(size_t i=0; i<FOrgPoints.size(); ++i){
        FPoints[i].x = FOrgPoints[i].x * FZoom;
        FPoints[i].y = FOrgPoints[i].y * FZoom;
    }
}

bool __fastcall TComicBox::DoMouseWheel(Classes::TShiftState Shift, int WheelDelta, const Types::TPoint &MousePos)
{
    vec3i mouse(MousePos.x, MousePos.y, WheelDelta);
    zoom_scene2D(FBounds, FZoom, FSize, mouse);
    bounds_range(FBounds, FViewPort);
//    UpdateBounds();
    DoZoomChanged();
    if(FCanvasUpdate)FCanvasUpdate(this);
    pntRuleLeft->Repaint();
    pntRuleTop->Repaint();
    PaintBox->Repaint();
    return true;
}
//---------------------------------------------------------------------------
//标尺
void __fastcall TComicBox::pntRuleTopPaint(TObject *Sender)
{
    pntRuleTop->Canvas->Brush->Color = clSilver;
    pntRuleTop->Canvas->FillRect(pntRuleTop->ClientRect);
	draw_rule_x(pntRuleTop->Canvas, pntRuleTop->Width, pntRuleLeft->Width + FBounds.x, FZoom);
}
//---------------------------------------------------------------------------

void __fastcall TComicBox::pntRuleLeftPaint(TObject *Sender)
{
    pntRuleLeft->Canvas->Brush->Color = clSilver ;
    pntRuleLeft->Canvas->FillRect(pntRuleLeft->ClientRect);
	pntRuleLeft->Canvas->Brush->Style = bsClear;
	draw_rule_y(pntRuleLeft->Canvas, pntRuleLeft->Height, FBounds.y, FZoom);
}
//---------------------------------------------------------------------------

void __fastcall TComicBox::PaintBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    FButton = Button;
    //转换画板坐标
    vec2i p = TransformPoint(X, Y);

    //选中点
    int id = HitTest(p.x, p.y);
    if(PointIndex != id){
        std::swap(PointIndex, id);
        if(IsValidIndex(id)){
            DrawPoint(id);
        }
        if(IsValidIndex(PointIndex)){
            DrawPoint(PointIndex);
            PaintBox->Cursor = crHandPoint;
            return ;
        }
        else{
            PaintBox->Cursor = crDefault;
        }
    }
    else{
        if(IsValidIndex(PointIndex)){
            return ;
        }
    }

    if(this->OnMouseDown)this->OnMouseDown(this, Button, Shift, p.x, p.y);
}
//---------------------------------------------------------------------------

void __fastcall TComicBox::PaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
    if(PenMode == 0){
        if(FBounds.contains(X, Y)){
            PaintBox->Cursor = crHandPoint;
        }
        else{
            PaintBox->Cursor = crDefault;
        }
    }

    //转换画板坐标
    vec2i p = TransformPoint(X, Y);

    //控制点热点
    int id = HitTest(p.x, p.y);
    if(HotPoint != id){
        std::swap(HotPoint, id);
        if(IsValidIndex(id)){
            DrawPoint(id);
        }
        if(IsValidIndex(HotPoint)){
            DrawPoint(HotPoint);
            PaintBox->Cursor = crHandPoint;
        }
        else{
            PaintBox->Cursor = crDefault;
        }
    }

    if(FButton == mbLeft){
        if(IsValidIndex(PointIndex)){//拖动控制点
            vec2i pos = GetPointPos(PointIndex);
            SetPointPos(PointIndex, pos.x + p.x - FComicMouse.x, pos.y + p.y - FComicMouse.y);
            PaintBox->Repaint();
        }
        else if(PenMode == 0){//移动画板
            FBounds.x += X - FMouse.x;
            FBounds.y += Y - FMouse.y;
            bounds_range(FBounds, FViewPort);
            if(FCanvasUpdate)FCanvasUpdate(this);
            pntRuleLeft->Repaint();
            pntRuleTop->Repaint();
            PaintBox->Repaint();
        }
    }

    FMouse = vec2i(X, Y);
    FComicMouse = p;

    if(this->OnMouseMove)this->OnMouseMove(this, Shift, p.x, p.y);
}
//---------------------------------------------------------------------------

void __fastcall TComicBox::PaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    FButton = TMouseButton(-1);
    //转换画板坐标
    vec2i p = TransformPoint(X, Y);
    if(this->OnMouseUp)this->OnMouseUp(this, Button, Shift, p.x, p.y);
}
//---------------------------------------------------------------------------

void TComicBox::DrawPoint(int i)
{
    TCanvas* dc = PaintBox->Canvas;
    dc->Pen->Color = clBlack;

    int x = FBounds.x + FPoints[i].x;
    int y = FBounds.y + FPoints[i].y;
    if(i == PointIndex){
        dc->Brush->Color = clRed;
    }
    else if(i == HotPoint){
        dc->Brush->Color = clSkyBlue;
    }
    else{
        dc->Brush->Color = clBlue;
    }
    dc->Ellipse(x - COMIC_POINT_SIZE, y - COMIC_POINT_SIZE, x + COMIC_POINT_SIZE, y + COMIC_POINT_SIZE);
}

void __fastcall TComicBox::PaintBoxPaint(TObject *Sender)
{
    TCanvas* c = PaintBox->Canvas;
    c->Brush->Bitmap = imgBackground->Picture->Bitmap;
    c->FillRect(PaintBox->ClientRect);

    c->Rectangle(toTRect(FBounds));

//    graphics::alphablend(dc->Handle,
//        FBounds.x, FBounds.y, FBounds.width, FBounds.height,
//        FSurface->Canvas->Handle, 0, 0, FSize.width, FSize.height, 255);
    //dc->StretchDraw(toTRect(FBounds), FSurface);

    int w = FSurface->Width * PaintBox->Width / FBounds.width;
    int h = FSurface->Height * PaintBox->Height / FBounds.height;

//    /*
    gdiplus::context dc(c);
    //Gdiplus::Image img(L"a.bmp");
    gdiplus::bitmap bmp(FSurface->Handle, null);

    //handle->SetCompositingQuality( Gdiplus::CompositingQualityHighSpeed );
//    g.SetCompositingMode( Gdiplus::CompositingModeSourceCopy );

    dc.effect_level(CGL_SPEED);
    dc.draw_image(bmp, FBounds.x, FBounds.y, FBounds.width, FBounds.height);
//    */

    //draw_polygon(dc, FPoints, FBounds.x, FBounds.y);

    //绘制控制点
    for(size_t i=0; i<FPoints.size(); ++i){
        DrawPoint(i);
    }
}
//---------------------------------------------------------------------------

TCanvas* __fastcall TComicBox::GetCanvas()
{
    return FSurface->Canvas;
}
//---------------------------------------------------------------------------

TPoint __fastcall TComicBox::GetCanvasOrg()
{
    return TPoint(FBounds.x, FBounds.y);
}

TComicBoxUpdateEvent __fastcall TComicBox::GetUpdateEvent()
{
    return FCanvasUpdate;
}

void __fastcall TComicBox::SetUpdateEvent(TComicBoxUpdateEvent value)
{
    FCanvasUpdate = value;
}
//---------------------------------------------------------------------------
//控制点操作

int TComicBox::AddPoint(int x, int y)
{
    FOrgPoints.push_back(TComicPoint(x, y));
    FPoints.push_back(vec2i(x * FZoom, y * FZoom));
    PaintBox->Repaint();
    return 0;
}

void TComicBox::ClearPoints()
{
    FOrgPoints.clear();
    FPoints.clear();
    PointIndex = -1;
    HotPoint = -1;
}

int TComicBox::HitTest(int x, int y)
{
    int id = -1;
    for(size_t i=0; i<FPoints.size(); ++i){
        if(math::distance<int>(x, y, FPoints[i].x, FPoints[i].y) < COMIC_POINT_SIZE){
            id = i;
            break;
        }
    }
    return id;
}

vec2i TComicBox::GetPointPos(int id)
{
    return vec2i(FOrgPoints[id].x, FOrgPoints[id].y);
}

void TComicBox::SetPointPos(int id, int x, int y)
{
    FOrgPoints[id].x = x;
    FOrgPoints[id].y = y;
    FPoints[id].x = x * FZoom;
    FPoints[id].y = y * FZoom;
}

TComicPoint __fastcall TComicBox::GetPoint(int index)
{
    return FOrgPoints[index];
}

void __fastcall TComicBox::SetPoint(int index, TComicPoint p)
{
    FOrgPoints[index] = p;
    FPoints[index].x = p.x * FZoom;
    FPoints[index].y = p.y * FZoom;
}
//---------------------------------------------------------------------------

int TComicBox::LoadImage(String filename)
{
    FSurface->Handle = graphics::bm_open(filename.c_str());
    FSurface->PixelFormat = pf32bit;
    FSize = vec2i(FSurface->Width, FSurface->Height);
    zoom_scene2D(FBounds, FZoom, FSize, vec3i(0, 0));
    bounds_range(FBounds, FViewPort);
    PaintBox->Repaint();
    return 0;
}
