//---------------------------------------------------------------------------

#ifndef RichTextBoxH
#define RichTextBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmRichEdit : public TFrame
{
__published:	// IDE-managed Components
    TImageList *imlRichEdit;
    TToolBar *ToolBar1;
    TToolButton *btnFont;
    TToolButton *btnSmall;
    TToolButton *btnLarge;
    TToolButton *btnBold;
    TToolButton *btnItalic;
    TToolButton *btnUnderline;
    TToolButton *btnLeft;
    TToolButton *btnCenter;
    TToolButton *btnWhat;
    TToolButton *btnRight;
    TToolButton *s3;
    TToolButton *s2;
    TToolButton *s1;
    TToolButton *btnImage;
    TComboBox *cbbFonts;
    TColorBox *ColorBox;
    TComboBox *cbbSize;
    TRichEdit *RichEdit;
    void __fastcall cbbFontsChange(TObject *Sender);
    void __fastcall cbbSizeChange(TObject *Sender);
    void __fastcall btnSmallClick(TObject *Sender);
    void __fastcall btnLargeClick(TObject *Sender);
    void __fastcall ColorBoxChange(TObject *Sender);
    void __fastcall btnFontClick(TObject *Sender);
    void __fastcall btnBoldClick(TObject *Sender);
    void __fastcall btnItalicClick(TObject *Sender);
    void __fastcall btnUnderlineClick(TObject *Sender);
    void __fastcall btnLeftClick(TObject *Sender);
    void __fastcall btnCenterClick(TObject *Sender);
    void __fastcall btnRightClick(TObject *Sender);
    void __fastcall btnImageClick(TObject *Sender);
    void __fastcall RichEditChange(TObject *Sender);
    void __fastcall RichEditProtectChange(TObject *Sender, int StartPos, int EndPos, bool &AllowChange);
    void __fastcall RichEditSelectionChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmRichEdit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRichEdit *frmRichEdit;
//---------------------------------------------------------------------------
#endif