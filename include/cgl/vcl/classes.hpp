/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vcl_classes.hpp

 2020-02-10 15:52:24

*/
#ifndef VCL_CLASSES_HPP_20200210155224
#define VCL_CLASSES_HPP_20200210155224

#include "base.hpp"
#include "memlink.hpp"

namespace cgl{
namespace vcl{

String VCL_CLASSES[] = {
    "TLabel",
    "TEdit",
    "TMemo",
    "TButton",
    "TPanel",
};

//template<typename T>
//inline void class_reg(T* obj)
//{
//    Classes::RegisterClass(__classid(T));
//}

//��ѯ����Ƿ�ע��
TPersistentClass find_class(String name)
{
    try{
        return FindClass(name);
    }
    catch(Exception &e){
        //print("FindClass() failed.");
    }
    return null;
}

TPersistentClass get_class(String name)
{
    try{
        return GetClass(name);
    }
    catch(...){
        //print("GetClass failed.");
    }
    return null;
}

//ע����
void class_reg(TObject* obj)
{
    //print(obj->ClassName());
    if(!find_class(obj->ClassName())){
        Classes::RegisterClass(obj->ClassType());
    }
}

//��ע����
void class_unreg(TObject* obj)
{
    Classes::UnRegisterClass(obj->ClassType());
}

//ע���༰�ӿؼ�����
void component_reg(TObject* obj)
{
    class_reg(obj);
    TWinControl* winobj = dynamic_cast<TWinControl*>(obj);
    if(winobj){
        for_each(winobj, component_reg, true);
    }
}

//��ע���༰�ӿؼ�����
void component_unreg(TObject* obj)
{
    class_unreg(obj);
    TWinControl* winobj = dynamic_cast<TWinControl*>(obj);
    if(winobj){
        for_each(winobj, component_unreg, true);
    }
}

TComponent* __fastcall StringToComponent(String as)
{
    std::auto_ptr<TMemoryStream> pms(new TMemoryStream);
    std::auto_ptr<TStringStream> pss(new TStringStream(as));
    TComponent *pc;

    try{
        ObjectTextToBinary(pss.get(), pms.get());
        pms->Seek(0, soFromBeginning);
    }
    catch(...){
        ShowMessage("Streaming error.");
    }

    pc = pms->ReadComponent(NULL);
    return pc;
}

//�ؼ�ת�ַ���(dfm)
//û���ӿؼ����ӿؼ���OwnerҪ���Ǹ��ؼ���������ȷ���
String __fastcall ComponentToString(TComponent *c)
{
    std::auto_ptr<TMemoryStream> pms(new TMemoryStream);
    std::auto_ptr<TStringStream> pss(new TStringStream);

    try{
        pms->WriteComponent(c);
        pms->Seek(0, soFromBeginning);
        ObjectBinaryToText(pms.get(), pss.get());
        pss->Seek(0, soFromBeginning);
        return pss->DataString;
    }
    catch(...){
        ShowMessage("Streaming error.");
    }

    return String();
}

//bin dfm To String
String object_to_text(const void* data, size_t size)
{
    alone_ptr<TMemoryLink> stm(new TMemoryLink);
    alone_ptr<TStringStream> out(new TStringStream);

    stm->Link(const_cast<void*>(data), size);
    stm->Seek(0, soFromBeginning);
    ObjectBinaryToText(stm.get(), out.get());

    return out->DataString;
}

}//end namespace vcl
}//end namespace cgl

#endif //VCL_CLASSES_HPP_20200210155224

//---------------------------------------------------------------------------
#if 0

#include <Rtti.hpp>
//#pragma explicit_rtti methods(__published, public) fields(__published, public, protected, private)

void __fastcall TfrmMain::Button5Click(TObject *Sender)
{
//    EnumPropertyList(this);
//    EnumMethodList(this);
//    EnumFieldList(this);

    //InitClasses();
    //print((int)this->ClassType(), (int)__classid(TfrmMain));

    component_reg(this);

    String s = ComponentToString(this);
    print(s);

    TStrings* sl = new TStringList;
    sl->LoadFromFile("pnl.txt");
    s = sl->Text;
    delete sl;

    TComponent* p = StringToComponent(s);
    //p->Owner = this;
    TWinControl*c = (TWinControl*)p;
    //c->Left = 200;
    //c->Parent = this;
    c->Visible = true;
    c->Show();

    WriteComponentResFile("a.txt", this);

    component_unreg(this);

    //ObjectResourceToText


    //TComponent* c = ReadComponentResFile("a.txt", Panel1);
    //InternalReadComponentRes(

}

template<class T>
UnicodeString EnumToStr(T t)
{
    return Typinfo::GetEnumName(__delphirtti(T),(int)t);
}

void EnumPropertyList(TObject *lpObject) // ö�����Ժ��¼�
{
  TRttiContext context;
  TRttiInstanceType *cls = dynamic_cast<TRttiInstanceType*>(context.GetType(lpObject->ClassType()));
  if(cls)
   {
     System::DynamicArray<TRttiProperty*> PropArray = cls->GetProperties();
     for(int i=0; i<PropArray.Length; i++)
      {
        TRttiProperty *lpProperty = PropArray[i];
        UnicodeString sProp, sName, sKind, sValue;
        sName = lpProperty->Name;
        //__delphirtti(lpProperty->Name);
        //lpProperty->PropertyType->Handle-
        //sKind = EnumToStr(lpProperty->PropertyType->TypeKind);
        //sKind = Typinfo::GetEnumName(__delphirtti(TObject*), (int)t);
        sKind = Typinfo::GetEnumName(lpProperty->PropertyType->Handle, (int)lpProperty->PropertyType->TypeKind);
        sProp = lpProperty->PropertyType->TypeKind == tkMethod ? L"�¼�" : L"����";
        try
         {
           TValue vVal = lpProperty->GetValue(lpObject);
           sValue = vVal.ToString();
         }
        catch(Exception &e)
         {
           sValue = e.ClassName() + L": " + e.Message;
         }
        print(sProp + L": " + sName + L", ����: " + sKind + L", ֵ: " + sValue);
      }
   }
}

void EnumMethodList(TObject *lpObject) // ö�ٳ�Ա����
{
  TRttiContext context;
  TRttiInstanceType *cls = dynamic_cast<TRttiInstanceType*>(context.GetType(lpObject->ClassType()));
  if(cls)
   {
     System::DynamicArray<TRttiMethod*> MethodList = cls->GetMethods();
     for(int i=0; i<MethodList.Length; i++)
      {
        print(L"��Ա����: " + MethodList[i]->Name);
      }
   }
}

void EnumFieldList(TObject *lpObject) // ö�ٳ�Ա����
{
    TRttiContext context;
    TRttiInstanceType *cls = dynamic_cast<TRttiInstanceType*>(context.GetType(lpObject->ClassType()));
    if(cls){
        System::DynamicArray<TRttiField*> FieldList = cls->GetFields();
        for(int i=0; i<FieldList.Length; i++){
            TRttiField *lpField = FieldList[i];
            UnicodeString sName, sKind, sValue;
            sName = lpField->Name;
            //sKind = EnumToStr(lpField->FieldType->TypeKind);
            //sKind = lpField->FieldType->QualifiedName;
            try{
                TValue vVal = lpField->GetValue(lpObject);
                sValue = vVal.ToString();
            }
            catch(Exception &e){
                sValue = e.ClassName() + L": " + e.Message;
            }
            print(L"��Ա����: " + sName + L", ����: " + sKind + L", ֵ: " + sValue);
        }
    }
}

#endif