//---------------------------------------------------------------------------

#ifndef ComicBoxH
#define ComicBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>

#include <cgl/graphics/ilimage.hpp>
#include <cgl/graphics/gdi/gdiplus.hpp>
#include <cgl/graphics/gdi/gdiplus_context.hpp>
#include <cgl/graphics/path.hpp>
#include <cgl/graphics/pixel.hpp>
#include <cgl/vec.hpp>
#include <cgl/vcl.hpp>
#include <cgl/vcl/ctrlpoint.hpp>
#include <vector>

typedef void __fastcall (__closure *TComicBoxUpdateEvent)(System::TObject* Sender);

enum {
    COMIC_LINES,
    COMIC_CURVE
};

enum ComicPen{
    ComicPenNull,
    ComicPenHand,
    ComicPenLine,
    ComicPenRect,
    ComicPenRoundRect,
    ComicPenEllipse,
    ComicPenLines,
    ComicPenCurve,
};

struct ComicPenItem
{
    ComicPen pen;
    const char* name;
};

static ComicPenItem ComicPens[] = {
    {ComicPenNull,      "无"},
    {ComicPenHand,      "手"},
    {ComicPenLine,      "直线"},
    {ComicPenRect,      "矩形"},
    {ComicPenRoundRect, "圆角矩形"},
    {ComicPenEllipse,   "椭圆"},
    {ComicPenLines,     "线段"},
    {ComicPenCurve,     "曲线"},
};

struct TComicPoint
{
    int x;
    int y;
    int style;

    TComicPoint() {}
    TComicPoint(int _x, int _y) : x(_x), y(_y), style() {}
};

//---------------------------------------------------------------------------
class TComicBox : public TFrame
{
__published:	// IDE-managed Components
    TPaintBox *pntRuleTop;
    TPaintBox *pntRuleLeft;
    TPanel *pnlBackground;
    TPaintBox *ComicView;
    TImage *imgBackground;
    TStatusBar *StatusBar;
    void __fastcall pntRuleTopPaint(TObject *Sender);
    void __fastcall pntRuleLeftPaint(TObject *Sender);
    void __fastcall ComicViewPaint(TObject *Sender);
    void __fastcall ComicViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ComicViewMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall ComicViewMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall pnlBackgroundResize(TObject *Sender);

private:	// User declarations
    ilimage FImage;                 //图像缓存
    Graphics::TBitmap* FSurface;    //绘图缓存
    vec4i FViewport;                //视口
    vec4i FBounds;                  //图像显示范围
    vec2i FSize;                    //图像原始大小
    float FZoom;                    //缩放因子
    vec2i FMouse;                   //窗口鼠标位置
    vec2i FComicMouse;              //本地鼠标位置
    TMouseButton FMouseButton;

    pixelbuffer<uint32_t> pixelbuf;
    pixelbuffer<uint32_t> image;

    int FCurrentImage;              //当前图层 gif

    vec2i TransformPoint(int x, int y);
    void DoZoomChanged();

    TCanvas* __fastcall GetCanvas();
    TPoint __fastcall GetCanvasOrg();
    vec4i __fastcall GetViewport();
    void ResizeSurface(int width, int height);

    TComicBoxUpdateEvent FCanvasUpdate;
    TComicBoxUpdateEvent __fastcall GetUpdateEvent();
    void __fastcall SetUpdateEvent(TComicBoxUpdateEvent value);

protected:
    void __fastcall WndProc(TMessage& msg);
    void __fastcall PanelWndProc(TMessage& msg);
    DYNAMIC void __fastcall Resize(void);
    DYNAMIC bool __fastcall DoMouseWheel(Classes::TShiftState Shift, int WheelDelta, const Types::TPoint &MousePos);

    void __fastcall OnPaintBoundsChange(bool repaint_rule = true);

    void StatusText(int id, String Text) { StatusBar->Panels->Items[id]->Text = Text; }

public:		// User declarations
    int PenMode;
    bool AutoPlay;      //自动播放动画
    bool DebugMode;

    __fastcall TComicBox(TComponent* Owner);

    //__property ilimage Image = {read=FImage};
    ilimage& Image;

    void ResizeImage(int Width, int Height);

    __property TCanvas* Canvas = {read=GetCanvas};

    __property TPoint CanvasOrg = {read=GetCanvasOrg};
    __property float Zoom = {read=FZoom};
    __property vec4i Viewport = {read=GetViewport};

    graphics::path_command Path;//测试路径

    TControlPoints Points;      //控制点

    int LoadImage(String filename);
    void CloseImage();

    //播放动画
    void OnLoop();

__published:
    __property TComicBoxUpdateEvent OnUpdate={read=GetUpdateEvent, write=SetUpdateEvent};
};
//---------------------------------------------------------------------------
//extern PACKAGE TComicBox *ComicBox;
//---------------------------------------------------------------------------
#endif
