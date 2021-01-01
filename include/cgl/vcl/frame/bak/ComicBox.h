//---------------------------------------------------------------------------

#ifndef ComicBoxH
#define ComicBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <pngimage.hpp>
#include <Graphics.hpp>

#include <cgl/graphics/gdi/gdiplus.hpp>
#include <cgl/vec.hpp>
#include <cgl/vcl/ctrlpoint.hpp>
#include <vector>
using namespace cgl;

typedef void __fastcall (__closure *TComicBoxUpdateEvent)(System::TObject* Sender);

enum {
    COMIC_LINES,
    COMIC_CURVE
};

struct TComicPoint
{
    int x;
    int y;
    int style;

    TComicPoint() {}
    TComicPoint(int _x, int _y) : x(_x), y(_y), style() {}
};

const int COMIC_POINT_SIZE = 4;

//---------------------------------------------------------------------------
class TComicBox : public TFrame
{
__published:	// IDE-managed Components
    TPaintBox *pntRuleTop;
    TPaintBox *pntRuleLeft;
    TPanel *pnlCanvas;
    TPaintBox *PaintBox;
    TImage *imgBackground;
    void __fastcall pntRuleTopPaint(TObject *Sender);
    void __fastcall pntRuleLeftPaint(TObject *Sender);
    void __fastcall PaintBoxPaint(TObject *Sender);
    void __fastcall PaintBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall PaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall PaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations
    Graphics::TBitmap* FSurface;    //图像缓存
    vec4i FViewPort;                //视口
    vec4i FBounds;                  //图像显示范围
    vec2i FSize;                    //图像原始大小
    float FZoom;                    //缩放因子
    vec2i FMouse;
    vec2i FComicMouse;              //本地鼠标位置
    TMouseButton FButton;

    std::vector<TComicPoint> FOrgPoints;//控制点
    std::vector<vec2i> FPoints;

    vec2i TransformPoint(int x, int y);
    //void UpdateBounds();
    void DoZoomChanged();

    TCanvas* __fastcall GetCanvas();
    TPoint __fastcall GetCanvasOrg();

    TComicBoxUpdateEvent FCanvasUpdate;
    TComicBoxUpdateEvent __fastcall GetUpdateEvent();
    void __fastcall SetUpdateEvent(TComicBoxUpdateEvent value);

    TComicPoint __fastcall GetPoint(int index);
    void __fastcall SetPoint(int index, TComicPoint p);
    void DrawPoint(int index);

protected:
    void __fastcall WndProc(TMessage& msg);
    void __fastcall PanelWndProc(TMessage& msg);
    DYNAMIC void __fastcall Resize(void);
    DYNAMIC bool __fastcall DoMouseWheel(Classes::TShiftState Shift, int WheelDelta, const Types::TPoint &MousePos);

public:		// User declarations
    int PenMode;

    __fastcall TComicBox(TComponent* Owner);
    void ResizeImage(int Width, int Height);

    __property TCanvas* Canvas = {read=GetCanvas};
    __property TPoint CanvasOrg = {read=GetCanvasOrg};
    __property float Zoom = {read=FZoom};

    __property TComicPoint Points[int n] = {read=GetPoint, write=SetPoint};

    int PointIndex;//索引
    int HotPoint;

    int AddPoint(int x, int y);
    void ClearPoints();
    int HitTest(int x, int y);
    vec2i GetPointPos(int id);
    void SetPointPos(int id, int x, int y);

    int LoadImage(String filename);

__published:
    __property TComicBoxUpdateEvent OnUpdate={read=GetUpdateEvent, write=SetUpdateEvent};
};
//---------------------------------------------------------------------------
//extern PACKAGE TComicBox *ComicBox;
//---------------------------------------------------------------------------
#endif
