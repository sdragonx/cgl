//---------------------------------------------------------------------------

#ifndef EditPlusH
#define EditPlusH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------


class TSCIEditor : public TWinControl
{
public:
    typedef sptr_t (*SCI_DIRECT)(sptr_t ptr, unsigned int iMessage, uptr_t wParam, sptr_t lParam);

private:
    SCI_DIRECT    sci_send;
    sptr_t        sci_ptr;

protected:
    virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
    virtual void __fastcall WndProc(Messages::TMessage &Message);

public:
    __fastcall virtual TSCIEditor(TComponent* Owner);
    __fastcall TSCIEditor(HWND ParentWindow) : TWinControl(ParentWindow) { }

    void Init();

    sptr_t send_message(unsigned int iMessage, uptr_t wParam = 0, sptr_t lParam = 0)
    {
        if(sci_send){
            return sci_send(sci_ptr, iMessage, wParam, lParam);
        }
        return 0;
    }

__published:
    __property OnMouseDown;
    __property OnMouseMove;
    __property OnMouseUp;
};
//---------------------------------------------------------------------------

class TEditPlus : public TFrame
{
__published:	// IDE-managed Components
private:	// User declarations
    TSCIEditor* FEditor;
    TLabel* FLabel;
    TStatusBar* FStatusBar;
    String FCaption;
    bool FInternalMenu;
    bool FReadOnly;
    bool FShowStatusBar;

    Classes::TNotifyEvent FOnChange;
    Classes::TNotifyEvent FOnEnter;
    Classes::TNotifyEvent FOnExit;

    int LastProcessedChar;//�����Զ�����
    int LastPos;
    int LastWordWidth;

    keywords_query m_keywords;



protected:
    virtual void __fastcall SetParent(TWinControl* AParent);

    //����
//    TFont* __fastcall GetFont();
//    void __fastcall SetFont(TFont* value);
    void __fastcall FontOnChange(TObject* Sender);//����䶯�¼�
    void SetFont(const char* name, int size);//��������

    //״̬��
    void __fastcall SetShowStatusBar(bool n);
    void ShowState();

    //����
    void __fastcall SetCaption(String text);

    //���ò˵�
    void __fastcall ShowInternalMenu(bool show);

    //ֻ��
    void __fastcall SetReadOnly(bool n);

    //�Ʊ�����
    void SetTabWidth(int n) { send_message(SCI_SETTABWIDTH, n); }
    int GetTabWidth() { return send_message(SCI_GETTABWIDTH); }

    //���ִ���
    int GetText(char* text, size_t size) { return send_message(SCI_GETTEXT, size, (sptr_t)text); }
    SCIString GetText(int start, int end);
    int GetLine(int line, char* text) { return send_message(SCI_GETLINE, line, (sptr_t)text); }

    //�Զ���ɣ��Զ���ʾ
    SCIString GetWord(int pos);
    void on_completed();
    void AutoComplete();
    void AutoTips(int pos);
    void AutoIndent();

public:		// User declarations
    __fastcall TEditPlus(TComponent* Owner);

    virtual void __fastcall WndProc(Messages::TMessage &Message);

    void Init();
    void LoadConfig(String filename);

    void OnNotify(LPARAM param);

    sptr_t send_message(unsigned int iMessage, uptr_t wParam = 0, sptr_t lParam = 0)
    {
        if(FEditor){
            return FEditor->send_message(iMessage, wParam, lParam);
        }
        return 0;
    }

};
//---------------------------------------------------------------------------
extern PACKAGE TEditPlus *EditPlus;
//---------------------------------------------------------------------------
#endif
