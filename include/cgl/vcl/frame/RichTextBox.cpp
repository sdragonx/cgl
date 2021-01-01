//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RichTextBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRichEdit *frmRichEdit;
//---------------------------------------------------------------------------
__fastcall TfrmRichEdit::TfrmRichEdit(TComponent* Owner)
    : TFrame(Owner)
{
    cbbFonts->Items = Screen->Fonts;
    RichEdit->Align = alClient;
//    RichEdit->DefAttributes->Pitch = 0;// RichEdit->Font->Pitch;
//    RichEdit->DefAttributes->Charset = GB2312_CHARSET;
//    RichEdit->DefAttributes->Name = "Î¢ÈíÑÅºÚ";// RichEdit->Font->Name;
    RichEdit->HideSelection = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::cbbFontsChange(TObject *Sender)
{//
    RichEdit->SelAttributes->Name = cbbFonts->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::cbbSizeChange(TObject *Sender)
{//
    RichEdit->SelAttributes->Size = cbbSize->Text.ToInt();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnSmallClick(TObject *Sender)
{//

}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnLargeClick(TObject *Sender)
{//

}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::ColorBoxChange(TObject *Sender)
{//
    RichEdit->SelAttributes->Color = ColorBox->Selected;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnFontClick(TObject *Sender)
{//

}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnBoldClick(TObject *Sender)
{//
    if(btnBold->Down){
        RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style<<fsBold;
    }
    else{
        RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style>>fsBold;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnItalicClick(TObject *Sender)
{//
    if(btnItalic->Down){
        RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style<<fsItalic;
    }
    else{
        RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style>>fsItalic;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnUnderlineClick(TObject *Sender)
{//
    if(btnUnderline->Down){
        RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style<<fsUnderline;
    }
    else{
        RichEdit->SelAttributes->Style = RichEdit->SelAttributes->Style>>fsUnderline;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnLeftClick(TObject *Sender)
{//
    RichEdit->Paragraph->Alignment = taLeftJustify;
    //RichEdit->Paragraph->FirstIndent = 10;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnCenterClick(TObject *Sender)
{//
    RichEdit->Paragraph->Alignment = taCenter;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnRightClick(TObject *Sender)
{//
    RichEdit->Paragraph->Alignment = taRightJustify;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::btnImageClick(TObject *Sender)
{//

}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::RichEditChange(TObject *Sender)
{//

}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::RichEditProtectChange(TObject *Sender, int StartPos, int EndPos, bool &AllowChange)
{//

}
//---------------------------------------------------------------------------

void __fastcall TfrmRichEdit::RichEditSelectionChange(TObject *Sender)
{//
    cbbFonts->Text = RichEdit->SelAttributes->Name;
    cbbSize->Text = RichEdit->SelAttributes->Size;
    ColorBox->Selected = RichEdit->SelAttributes->Color;
    btnBold->Down = RichEdit->SelAttributes->Style.Contains(fsBold);
    btnBold->Down = RichEdit->SelAttributes->Style.Contains(fsBold);
    btnItalic->Down = RichEdit->SelAttributes->Style.Contains(fsItalic);
    btnUnderline->Down = RichEdit->SelAttributes->Style.Contains(fsUnderline);
    switch(RichEdit->Paragraph->Alignment)
    {
    case taLeftJustify:
        btnLeft->Down = true;
        break;
    case taRightJustify:
        btnRight->Down = true;
        break;
    case taCenter:
        btnCenter->Down = true;
        break;
    }
}
//---------------------------------------------------------------------------

