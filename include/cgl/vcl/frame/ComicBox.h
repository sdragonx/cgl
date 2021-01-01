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
    {ComicPenNull,      "��"},
    {ComicPenHand,      "��"},
    {ComicPenLine,      "ֱ��"},
    {ComicPenRect,      "����"},
    {ComicPenRoundRect, "Բ�Ǿ���"},
    {ComicPenEllipse,   "��Բ"},
    {ComicPenLines,     "�߶�"},
    {ComicPenCurve,     "����"},
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
    ilimage FImage;                 //ͼ�񻺴�
    Graphics::TBitmap* FSurface;    //��ͼ����
    vec4i FViewport;                //�ӿ�
    vec4i FBounds;                  //ͼ����ʾ��Χ
    vec2i FSize;                    //ͼ��ԭʼ��С
    float FZoom;                    //��������
    vec2i FMouse;                   //�������λ��
    vec2i FComicMouse;              //�������λ��
    TMouseButton FMouseButton;

    pixelbuffer<uint32_t> pixelbuf;
    pixelbuffer<uint32_t> image;

    int FCurrentImage;              //��ǰͼ�� gif

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
    bool AutoPlay;      //�Զ����Ŷ���
    bool DebugMode;

    __fastcall TComicBox(TComponent* Owner);

    //__property ilimage Image = {read=FImage};
    ilimage& Image;

    void ResizeImage(int Width, int Height);

    __property TCanvas* Canvas = {read=GetCanvas};

    __property TPoint CanvasOrg = {read=GetCanvasOrg};
    __property float Zoom = {read=FZoom};
    __property vec4i Viewport = {read=GetViewport};

    graphics::path_command Path;//����·��

    TControlPoints Points;      //���Ƶ�

    int LoadImage(String filename);
    void CloseImage();

    //���Ŷ���
    void OnLoop();

__published:
    __property TComicBoxUpdateEvent OnUpdate={read=GetUpdateEvent, write=SetUpdateEvent};
};
//---------------------------------------------------------------------------
//extern PACKAGE TComicBox *ComicBox;
//---------------------------------------------------------------------------
#endif
