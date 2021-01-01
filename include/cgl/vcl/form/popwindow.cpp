//---------------------------------------------------------------------------
//2015-03-20 21:42:42
//弹窗

#include <vcl.h>
#pragma hdrstop

#include "popwindow.h"
#include "public.h"

const static int POPUP_WINDOW_SHOW = 0;
const static int POPUP_WINDOW_SHOWING = 2;
const static int POPUP_WINDOW_CLOSE = 3;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAskBox *frmAskBox;
//---------------------------------------------------------------------------
__fastcall TfrmAskBox::TfrmAskBox(TComponent* Owner)
	: TForm(Owner)
{
	Command = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::FormCreate(TObject *Sender)
{//
	this->BorderStyle = bsNone;
	this->FormStyle = fsStayOnTop;
	//SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	//::SetParent(Handle, NULL);
	//this->Parent = NULL;
	//this->ParentWindow = NULL;
	SetWindowLong(Handle, GWL_HWNDPARENT, (LONG)GetShellWindow());
	this->DoubleBuffered = true;


	lblText->Caption = STRING_NULL;

	lblCheck->Tag = false;
	lblCheckClick(lblCheck);

	FormPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::FormShow(TObject *Sender)
{//
    this->Left = Screen->WorkAreaLeft + Screen->WorkAreaWidth-Width;
	this->Top = Screen->WorkAreaTop+Screen->WorkAreaHeight-Height;
	AlphaBlend = true;
	AlphaBlendValue = 0;
	Tag = POPUP_WINDOW_SHOW;
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::FormClose(TObject *Sender, TCloseAction &Action)
{//

}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::FormDestroy(TObject *Sender)
{//

}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::FormPaint(TObject *Sender)
{
	Gdiplus::Graphics g(Canvas->Handle);
	Gdiplus::Image img(L"../images/ask.png");
	//g.Clear(Gdiplus::Color::White);
	g.DrawImage(&img, 0, 0, ClientWidth, ClientHeight);
	RECT rc = lblOK->BoundsRect;
	alpha_rect(Canvas->Handle, rc, 0xFFFFFFFF, lblOK->Tag);
	//lblOK->Repaint();
	rc = lblCancel->BoundsRect;
	alpha_rect(Canvas->Handle, rc, 0xFFFFFFFF, lblCancel->Tag);
	//lblCancel->Repaint();

}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::lblOKMouseEnter(TObject *Sender)
{
	TLabel* Label = dynamic_cast<TLabel*>(Sender);
	if(Label)
	{
		Label->Font->Color = clGreen;
		Label->Tag = 128;
		this->Repaint();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::lblOKMouseLeave(TObject *Sender)
{
	TLabel* Label = dynamic_cast<TLabel*>(Sender);
	if(Label)
	{
		Label->Font->Color = clMoneyGreen;
		Label->Tag = 64;
		this->Repaint();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmAskBox::lblOKClick(TObject *Sender)
{//
	switch(Command)
	{
	case CMD_EXIT:
		frmMain->CanExit = true;
		frmMain->Close();
		break;
	case CMD_ASKAUTORUN:
		if(lblCheck->Tag)
		{
			EnableAutorun(true);
			frmMain->mnuAutorun->Checked = true;
		}
		break;
	default:
		break;
	}
	Tag = POPUP_WINDOW_CLOSE;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::lblCancelClick(TObject *Sender)
{//
	Tag = 0;
	Tag = POPUP_WINDOW_CLOSE;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAskBox::lblCheckClick(TObject *Sender)
{
	lblCheck->Tag = !lblCheck->Tag;
	lblCheck->Caption = String(lblCheck->Tag ? TEXT("☑") : TEXT("☒")) + TEXT("设为开机启动项");
}
//---------------------------------------------------------------------------
int TfrmAskBox::Popup(int cmd)
{
	Command = cmd;
	this->Show();
	return 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAskBox::Timer1Timer(TObject *Sender)
{
	int step = 256/8;

	switch(this->Tag)
	{
	case POPUP_WINDOW_SHOW:
		if(AlphaBlendValue != 0xFF)
		{
			AlphaBlendValue = AlphaBlendValue + step <256 ? AlphaBlendValue + step : 0xFF;
		}
		else
		{
			PopupTime = GetTickCount();
			Tag = POPUP_WINDOW_SHOWING;
        }
		break;
	case POPUP_WINDOW_SHOWING:
		if(GetTickCount() - PopupTime >= ShowTime)
		{
			Tag = POPUP_WINDOW_CLOSE;
		}
		break;
	case POPUP_WINDOW_CLOSE:
		if(AlphaBlendValue!=0)
		{
            AlphaBlendValue = AlphaBlendValue - step > 0 ? AlphaBlendValue - step : 0;
		}
		else
		{
			this->Close();
        }
		break;
	default:
		break;
    }
}
//---------------------------------------------------------------------------

