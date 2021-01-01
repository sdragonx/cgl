//---------------------------------------------------------------------------

#ifndef iconeditH
#define iconeditH
//---------------------------------------------------------------------------
#include "public.h"
#include "icon.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmIconEdit : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TPanel *Panel2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *edtTitle;
	TEdit *edtComment;
	TPanel *Panel1;
	TOpenDialog *OpenDialog1;
	TSpeedButton *SpeedButton1;
	TImage *Image1;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TComboBox *cbbShell;
	TComboBox *cbbIcon;
	TLabel *Label4;
	TSpeedButton *SpeedButton2;
	TEdit *edtIndex;
	TLabel *Label5;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SpeedButton1Click(TObject *Sender);
	void __fastcall Image1Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall cbbShellSelect(TObject *Sender);
private:	// User declarations
	String GetShell();
	TWindow* IconWindow;
	icon_image image;
public:		// User declarations
	__fastcall TfrmIconEdit(TComponent* Owner);
	int Result;
	int Edit(TWindow* icon);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmIconEdit *frmIconEdit;
//---------------------------------------------------------------------------
#endif
