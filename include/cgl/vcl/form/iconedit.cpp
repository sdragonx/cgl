//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "iconedit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmIconEdit *frmIconEdit;

//---------------------------------------------------------------------------
__fastcall TfrmIconEdit::TfrmIconEdit(TComponent* Owner)
	: TForm(Owner)
{
	Result = -1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmIconEdit::FormCreate(TObject *Sender)
{
	this->Position = poScreenCenter;
	Image1->Hint = L"点击更改图标";
	Image1->ShowHint = true;
	Image1->Cursor = crHandPoint;

	for(int i=0; i<CSIDL_SIZE; ++i)
	{
        cbbShell->Items->Add(CSIDL_LIST[i].name);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmIconEdit::FormShow(TObject *Sender)
{
	if(!IconWindow)return ;
	edtTitle->Text = IconWindow->Caption;
	cbbShell->Text = IconWindow->shell;
	edtComment->Text = IconWindow->Hint;
	CheckBox1->Checked = IconWindow->lock;
	cbbIcon->Text = IconWindow->image.get_image();
	edtIndex->Text = IconWindow->image.get_index();
	//CheckBox2->Checked = IconWindow->Visible;
	image.open_image(IconWindow->image.get_image(), IconWindow->image.get_index());
	Image1->Canvas->FillRect(Image1->ClientRect);
	image.paint(Image1->Canvas->Handle,
		(Image1->Width-ICON_SIZE)/2, (Image1->Height-ICON_SIZE)/2,
		ICON_SIZE, ICON_SIZE);
	Image1->Repaint();
}
//---------------------------------------------------------------------------

const String FILTER_ALL = L"所有文件(*.*)|*.*";
const String FILTER_ICON = L"图标文件|*.exe;*.dll;*.png;*.ico|图标|*.png;*.ico|程序文件|*.exe;*.dll";

void __fastcall TfrmIconEdit::SpeedButton1Click(TObject *Sender)
{
	String file;
	String name;
	String ext;

	OpenDialog1->Filter = FILTER_ALL;
	if(!OpenDialog1->Execute(this->Handle))
	{
		return ;
	}

	file = OpenDialog1->FileName;
	name = ExtractFileName(file);
	ext = ExtractFileExt(name).LowerCase();
	ChangeFileExt(name, STRING_NULL);
	edtTitle->Text = name;
	cbbShell->Text = file;
	image.open_image(file, -1);
	Image1->Canvas->FillRect(Image1->ClientRect);
	image.paint(Image1->Canvas->Handle,
		(Image1->Width-ICON_SIZE)/2, (Image1->Height-ICON_SIZE)/2,
		ICON_SIZE, ICON_SIZE);
	Image1->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TfrmIconEdit::Image1Click(TObject *Sender)
{
	WCHAR path[MAX_PATH] = {0};
	String file;
	String name;
	String ext;
	int icon = -1;
	OpenDialog1->Filter = FILTER_ICON;
	if(!OpenDialog1->Execute(this->Handle))
	{
		return ;
	}

	file = OpenDialog1->FileName;
	name = ExtractFileName(file);
	ext = ExtractFileExt(name).LowerCase();
	if(ext==L".dll" || ext==L".exe")
	{
		wcscpy(path, file.c_str());
		if(!CallPickIconDlg(Handle, path, MAX_PATH, &icon))
			return ;
	}
	else
	{
		//ShowMessage(file);
	}
	cbbIcon->Text = file;
	edtIndex->Text = icon;
	image.open_image(file, icon);
	Image1->Canvas->FillRect(Image1->ClientRect);
	image.paint(Image1->Canvas->Handle,
		(Image1->Width-ICON_SIZE)/2, (Image1->Height-ICON_SIZE)/2,
		ICON_SIZE, ICON_SIZE);
	Image1->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TfrmIconEdit::Button1Click(TObject *Sender)
{//保存设置
	if(!IconWindow)return ;
	auto_ptr<TIniFile> ini(new TIniFile(CONFIG_FILE));
	IconWindow->Caption = edtTitle->Text;
	IconWindow->shell = GetShell();
	IconWindow->Hint = edtComment->Text;
	IconWindow->lock = CheckBox1->Checked;
	IconWindow->image.open_image(cbbIcon->Text, edtIndex->Text.ToInt());
	IconWindow->config_save(ini.get());
	IconWindow->Parent->Perform(WM_PAINT, 0, 0);
	Result = 0;
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmIconEdit::Button2Click(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------

String TfrmIconEdit::GetShell()
{
	if(cbbShell->ItemIndex < 0)
	{
		return cbbShell->Text;
	}
	else
	{
		return CSIDL_LIST[cbbShell->ItemIndex].cmd;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmIconEdit::cbbShellSelect(TObject *Sender)
{
	SHFILEINFO fi = {0};
	LPITEMIDLIST id;
	SHGetSpecialFolderLocation(NULL, CSIDL_LIST[cbbShell->ItemIndex].id, &id);
	SHGetFileInfo((TCHAR*)id, 0, &fi, sizeof(SHFILEINFO), SHGFI_PIDL|SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
	ILFree(id);
	edtTitle->Text = fi.szDisplayName;
	edtIndex->Text = -1;
	cbbIcon->Text = CSIDL_LIST[cbbShell->ItemIndex].cmd;
	image.open_image("", fi.iIcon);
	Image1->Canvas->FillRect(Image1->ClientRect);
	image.paint(Image1->Canvas->Handle,
		(Image1->Width-ICON_SIZE)/2, (Image1->Height-ICON_SIZE)/2,
		ICON_SIZE, ICON_SIZE);
	Image1->Repaint();
}
//---------------------------------------------------------------------------

int TfrmIconEdit::Edit(TWindow* icon)
{
	IconWindow = icon;
	this->ShowModal();
	return Result;
}
