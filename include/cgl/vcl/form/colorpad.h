//---------------------------------------------------------------------------

#ifndef colorpadH
#define colorpadH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <cgl/vec.hpp>
//---------------------------------------------------------------------------
class TfrmColorPad : public TForm
{
__published:	// IDE-managed Components
	TButton *btnOK;
	TButton *btnCancel;
	TPanel *pnlPad;
	TShape *shpColor;
	TLabel *R;
	TLabel *G;
	TLabel *B;
	TLabel *A;
	TImage *imgColorpad;
	TShape *shpPreview;
	TTrackBar *tkbRed;
	TTrackBar *tkbGreen;
	TTrackBar *tkbBlue;
	TTrackBar *tkbAlpha;
	TEdit *edtRed;
	TEdit *edtGreen;
	TEdit *edtBlue;
	TEdit *edtAlpha;
	TColorBox *ccbColor;
	void __fastcall edtRedChange(TObject *Sender);
	void __fastcall tkbRedChange(TObject *Sender);
	void __fastcall imgColorpadMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall imgColorpadMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall imgColorpadMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ccbColorChange(TObject *Sender);
    void __fastcall ccbColorGetColors(TCustomColorBox *Sender, TStrings *Items);

private:	// User declarations
	void SetColorEx(DWORD color, bool alpha);
	void SetColor(DWORD color);
	DWORD GetColor();
	void SetVecColor(cgl::vec4ub color);
	cgl::vec4ub GetVecColor();
public:		// User declarations
	__fastcall TfrmColorPad(TComponent* Owner);
	__property DWORD Color = {read=GetColor, write=SetColor};
	__property cgl::vec4ub VecColor = {read=GetVecColor, write=SetVecColor};
	bool Finish();
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmColorPad *frmColorPad;
DWORD ShowColorpad(TComponent* Owner, DWORD color);
cgl::vec4ub ShowColorpad(TComponent* Owner, cgl::vec4ub color);
//---------------------------------------------------------------------------
#endif