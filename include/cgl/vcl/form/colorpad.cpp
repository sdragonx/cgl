//---------------------------------------------------------------------------
//imgColorpad->Tag	mousedown

#include <vcl.h>
#pragma hdrstop

#include "colorpad.h"
//#include "public.h"
#include <cgl/graphics/color.hpp>
using namespace cgl;
using namespace graphics;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma resource "colorpad.dfm"
//TfrmColorPad *frmColorPad;

cgl::vec4ub ShowColorpad(TComponent* Owner, cgl::vec4ub color)
{
	TfrmColorPad* form = new TfrmColorPad(Owner);
	form->VecColor = color;
	form->ShowModal();
	if(form->Finish()){
		color = form->VecColor;
	}
	delete form, form = null;
	return color;
}

DWORD ShowColorpad(TComponent* Owner, DWORD color)
{
	TfrmColorPad* form = new TfrmColorPad(Owner);
	form->Color = color;
	form->ShowModal();
	if(form->Finish()){
		color = form->Color;
	}
	delete form, form = null;
	return color;
}

void draw_colorpad(TCanvas* canvas, TRect rc)
{
	for(int x=0; x < rc.Width(); ++x)
	{
		for(int y=0; y < rc.Height(); ++y){
			DWORD c = graphics::colorpad_color(rc.Width(), rc.Height(), x, y);
			canvas->Pixels[rc.left + x][rc.top + y] = TColor(c & 0x00FFFFFF);
		}
	}
}
//---------------------------------------------------------------------------
__fastcall TfrmColorPad::TfrmColorPad(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::FormCreate(TObject *Sender)
{
	imgColorpad->Picture->Bitmap->PixelFormat = pf24bit;
	imgColorpad->Picture->Bitmap->Width = imgColorpad->Width;
	imgColorpad->Picture->Bitmap->Height = imgColorpad->Height;
	draw_colorpad(imgColorpad->Canvas, imgColorpad->ClientRect);

}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::edtRedChange(TObject *Sender)
{
	tkbRed->Position = edtRed->Text.ToInt();
	tkbGreen->Position = edtGreen->Text.ToInt();
	tkbBlue->Position = edtBlue->Text.ToInt();
	tkbAlpha->Position = edtAlpha->Text.ToInt();
	shpColor->Brush->Color = TColor(RGB(tkbRed->Position, tkbGreen->Position, tkbBlue->Position));
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::tkbRedChange(TObject *Sender)
{
	edtRed->Text = tkbRed->Position;
	edtGreen->Text = tkbGreen->Position;
	edtBlue->Text = tkbBlue->Position;
	edtAlpha->Text = tkbAlpha->Position;
}
//---------------------------------------------------------------------------

void TfrmColorPad::SetColorEx(DWORD color, bool alpha)
{
	tkbRed->Position = cgl::graphics::RGBA_RED(color);
	tkbGreen->Position = cgl::graphics::RGBA_GREEN(color);
	tkbBlue->Position = cgl::graphics::RGBA_BLUE(color);
	if(alpha)tkbAlpha->Position = cgl::graphics::RGBA_ALPHA(color);
	shpColor->Brush->Color = TColor(color & 0x00FFFFFF);
}

void TfrmColorPad::SetColor(DWORD color)
{
	SetColorEx(color, true);
}

DWORD TfrmColorPad::GetColor()
{
	return RGBA(tkbRed->Position, tkbGreen->Position, tkbBlue->Position, tkbAlpha->Position);
}

void TfrmColorPad::SetVecColor(cgl::vec4ub color)
{
	tkbRed->Position = color.red;
	tkbGreen->Position = color.green;
	tkbBlue->Position = color.blue;
	tkbAlpha->Position = color.alpha;
	shpColor->Brush->Color = TColor(RGBA(color.red, color.green, color.blue, 0));
}

cgl::vec4ub TfrmColorPad::GetVecColor()
{
	return vec4ub(tkbRed->Position, tkbGreen->Position, tkbBlue->Position, tkbAlpha->Position);
}

//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::imgColorpadMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	imgColorpad->Tag = 1;
	SetColorEx(imgColorpad->Canvas->Pixels[X][Y], false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::imgColorpadMouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	shpPreview->Brush->Color = imgColorpad->Canvas->Pixels[X][Y];
	if(imgColorpad->Tag){
		SetColorEx(imgColorpad->Canvas->Pixels[X][Y], false);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::imgColorpadMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	imgColorpad->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::ccbColorChange(TObject *Sender)
{
	SetColorEx(ccbColor->Selected, false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::FormShow(TObject *Sender)
{
	this->Tag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::btnOKClick(TObject *Sender)
{
	this->Tag = 1;
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::btnCancelClick(TObject *Sender)
{
	this->Tag = 0;
	this->Close();
}
//---------------------------------------------------------------------------
//按下了确定按钮
bool TfrmColorPad::Finish()
{
	return Tag;
}

//---------------------------------------------------------------------------

void __fastcall TfrmColorPad::ccbColorGetColors(TCustomColorBox *Sender, TStrings *Items)
{
    Items->Clear();
    int size = sizeof(graphics::color::colors) / sizeof(graphics::color::color_item);
    for(int i=0;i < size; ++i){
        vec4ub c = graphics::color::colors[i].color;
        DWORD t = RGB(c.r, c.g, c.b);
        Items->AddObject(graphics::color::colors[i].title, (TObject*)t);
    }
}
//---------------------------------------------------------------------------


