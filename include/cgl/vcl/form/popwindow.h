//---------------------------------------------------------------------------

#ifndef popwindowH
#define popwindowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "main.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAskBox : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblText;
	TLabel *lblOK;
	TLabel *lblCancel;
	TLabel *lblCheck;
	TTimer *Timer1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall lblOKMouseEnter(TObject *Sender);
	void __fastcall lblOKMouseLeave(TObject *Sender);
	void __fastcall lblOKClick(TObject *Sender);
	void __fastcall lblCancelClick(TObject *Sender);
	void __fastcall lblCheckClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
	int Command;
public:		// User declarations
	__fastcall TfrmAskBox(TComponent* Owner);
	int Popup(int cmd);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAskBox *frmAskBox;
//---------------------------------------------------------------------------
#endif
