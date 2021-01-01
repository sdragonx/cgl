/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vcl_controls.hpp

 2017-01-26 11:01:30

 VCL�ؼ���һЩ��չ����������

*/
#ifndef VCL_CONTROLS_HPP_20170126110130
#define VCL_CONTROLS_HPP_20170126110130

#include "base.hpp"
#include <cgl/system/keyboard.hpp>
#include <cgl/windows/controls.hpp>

#include "controls/listview.hpp"
#include "controls/treeview.hpp"

namespace cgl{
namespace vcl{

//
// ͨ�ô���ؼ�
//

class TWinControlHelper : public Controls::TWinControl
{
public:
    void Send(TMessage& msg)
    {
        TWinControl::WndProc(msg);
    }
};

class TGraphicControlHalper : public TGraphicControl
{
__published:
    __property Canvas;
};



HDC GetHDC(TComponent* obj)
{
    TGraphicControlHalper* graph = static_cast<TGraphicControlHalper*>(obj);
    if(graph){
        return graph->Canvas->Handle;
    }
    TCustomForm* form = dynamic_cast<TCustomForm*>(obj);
    if(form){
        return form->Canvas->Handle;
    }
    return null;
}

HWND GetHWND(TComponent* obj)
{
    TWinControl* c = dynamic_cast<TWinControl*>(obj);
    if(c){
        return c->Handle;
    }
    return null;
}

void SendTMessage(TWinControl* obj, TMessage& msg)
{
    ((TWinControlHelper*)obj)->Send(msg);
}



//�ڴ����������������͵ģ�����TTreeView
template<typename T>
T* GetControl(TWinControl* WinControl, int Index = 0)
{
    if(WinControl)
        return dynamic_cast<T*>(WinControl->Controls[Index]);
    else
        return NULL;
}

template<typename T>
T* FindControl(TWinControl* WinControl)
{
    if(!WinControl){
        return NULL;
    }

    T* control = null;
    for(int i=0; i<WinControl->ControlCount; ++i){
        control = dynamic_cast<T*>(WinControl->Controls[i]);
        if(control)break;
    }
    return control;
}

template<typename T>
T* FindNextControl(TWinControl* control)
{
    T* next = null;
    do{
        next = dynamic_cast<T*>(control->GetNextInstance());
        if(next)break;
    }while(control);
    return next;
}

//��ʼ��Items
//������TList��TComboBox��TRadioGroup��
template<typename T, typename _char_t>
void InitItems(T* obj, const _char_t* list[], size_t size)
{
    obj->Items->Clear();
    for(size_t i=0; i<size; ++i){
        obj->Items->Add(list[i]);
    }
}

//
// �ڸ��ؼ��ڸ�����ʾһ���ؼ�����֤�ؼ����Ƴ����ؼ���Χ
//
void PopupClient(TControl* parent, TControl* obj, int x, int y, int offset = 4)
{
    if(x + offset + obj->Width < parent->ClientWidth){
        obj->Left = x + offset;
    }
    else{
        obj->Left = x - obj->Width - offset;
    }

    if(y + offset + obj->Height < parent->ClientHeight){
        obj->Top  = y + offset;
    }
    else{
        obj->Top  = y - obj->Height - offset;
    }
}

//void PopupScreen(...)

//
// ʹһ���ؼ�������ʾ
//
void SetCenterPos(TControl* obj)
{
    TWinControl* parent = obj->Parent;
    obj->Left = (parent->ClientWidth - obj->Width) / 2;
    obj->Top = (parent->ClientHeight - obj->Height) / 2;
}

//
//ȡ�ؼ����ĵ������
//
void SetPointPosition(TControl* obj, TPoint p)
{
    obj->Left = p.x - obj->Width / 2;
    obj->Top  = p.y - obj->Height / 2;
}

TPoint GetPointPosition(TControl* obj)
{
    return TPoint(obj->Left + obj->Width / 2, obj->Top + obj->Height / 2);
}

//
// TForm
//

//����һ���մ���
TForm* TForm_Create(String title, int width, int height, TBorderStyle style = bsSizeable, TPosition position = poDesktopCenter)
{
    TForm *form = new TForm(HWND(NULL));
    form->BorderIcons << biSystemMenu << biMinimize << biMaximize;
    form->BorderStyle = style;
    form->Caption = title;
    form->Font->Name = L"΢���ź�";
    form->Font->Size = 11;
    form->Position = position;
    form->Show();
    return form;
}


//TFrame* ShowFrame(

//
//TPageControl
//

//���TPageControl����������͵Ŀؼ�
template<typename T>
T* GetPageActiveControl(TPageControl* PageControl, int Index = 0)
{
    if(PageControl->ActivePage)
        return dynamic_cast<T*>(PageControl->ActivePage->Controls[Index]);
    else
        return NULL;
}

//
// TEdit
//

//ת�����ֲ���ʾ������ʾ��
int toInt(TCustomEdit* edit, int def = 0)
{
    try{
        return edit->Text.ToIntDef(def);
    }
    catch(EConvertError& err){
        //OutputDebugString(err.Message.c_str());
        return def;
    }
}

double toDouble(TCustomEdit* edit, double def = 0.0)
{
    try{
        return edit->Text.ToDouble();
    }
    catch(EConvertError& err){
        //OutputDebugString(err.Message.t_str());
        return def;
    }
}

//
//TMemo
//

//ȫѡ
//TMemo_KeyPress�¼��д���CTRL+A
void TMemo_KeyPressSelectAll(TObject *Sender, wchar_t &Key)
{
    if(Key == 1){
        TMemo* Memo = dynamic_cast<TMemo*>(Sender);
        if(Memo){
            Memo->SelectAll();
            Key = 0;//����Key=0������ε���
        }
    }
}

bool is_keyword_char(int ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '_';
}

void TMemo_KeyPress(TObject* Sender, wchar_t &Key)
{
    TMemo* Memo = dynamic_cast<TMemo*>(Sender);
    switch(Key){
    case 1://CTRL + A
        Memo->SelectAll();
        Key = 0;//����Key=0������ε���
        break;
    case VK_BACK://����CTRL + <-���뷽��
        if(key_ctrl_down()){
            Key = 0;
        }
        break;
    case 127://CTRL + <- ɾ������
        //delete selected
        if(Memo->SelLength != 0){
            Memo->SelText = String();
        }
        else{
            int i = Memo->SelStart;
            int start = Memo->SelStart;

            if(i <= 1){
                Key = VK_BACK;
                return ;
            }

            //skip space
            while(0 < i){
                if(!isspace(Memo->Text[i])){
                    break;
                }
                --i;
            }

            if(is_keyword_char(Memo->Text[i])){
                for( ; 0 < i; --i){
                    if(!is_keyword_char(Memo->Text[i])){
                        break;
                    }
                }
            }
            else{
                if(1 < i)--i;
            }
            if(i != start){
                Memo->SelLength = i - start;
                Memo->SelText = String();
            }
        }
        Key = 0;
        break;
    }
}

//˫��ѡ�񵥴�
void TMemo_DblClick(TObject *Sender)
{
    TMemo* memo = dynamic_cast<TMemo*>(Sender);
    if(!memo)return ;
    if(memo->SelLength){
    }

    String s = memo->SelText;
    int n = memo->SelLength;

    for(int i=s.Length() - 0; i > 0; --i){
        if(!isalpha(s[i])){//isspace(s[i])){
            --n;
        }
    }
    memo->SelLength = n;
}

//
//TStatusBar
//

//��������״̬��
void SetStatusText(int index, String text)
{
    TForm* form = Application->MainForm;
    if(form){
        TStatusBar* StatusBar = vcl::FindControl<TStatusBar>(form);
        if(StatusBar){
            if(index < StatusBar->Panels->Count){
                StatusBar->Panels->Items[index]->Text = text;
            }
        }
    }
}

//
// TComboBox
//

void TComboBox_Init(TComboBox* combo, const char* list[], size_t size)
{
    combo->Items->Clear();
    for(size_t i=0; i<size; ++i){
        combo->Items->Add(list[i]);
    }
}

//�����б��е��ַ������������ѡ��
int TComboBox_Select(TComboBox* box, String Text, int def_index = 0)
{
    if(box->Items->Count == 0){
        return -1;
    }

    int i;
    for(i=0; i<box->Items->Count; ++i){
        if(box->Items->Strings[i] == Text){
            box->ItemIndex = i;
            break;
        }
    }
    if(i == box->Items->Count){
        box->ItemIndex = def_index;
    }
    return box->ItemIndex;
}


//ģ������

//�༭
//cgl/windows/controls.hpp
void TControl_SelectAll(TControl* obj)
{
    if(!obj){
        return ;
    }

    TEdit* edit = dynamic_cast<TEdit*>(obj);
    if(edit){
        edit->SelectAll();
    }

    TMemo* memo = dynamic_cast<TMemo*>(obj);
    if(memo){
        memo->SelectAll();
        return ;
    }

    TRichEdit* rich = dynamic_cast<TRichEdit*>(obj);
    if(rich){
        rich->SelectAll();
    }

    TComboBox* combobox = dynamic_cast<TComboBox*>(obj);
    if(combobox){
        combobox->SelectAll();
    }
}


}//end namespace vcl
}//end namespace cgl

#endif //VCL_CONTROLS_HPP_20170126110130