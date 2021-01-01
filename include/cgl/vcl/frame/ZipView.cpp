//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZipView.h"
#include <cgl/vcl.hpp>
#include <windowsx.h>
#include <commctrl.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma resource "zipview.dfm"
TZipView *ZipView;
extern void print(String);
//---------------------------------------------------------------------------
__fastcall TZipView::TZipView(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
// ����bug���˳����ڴ汨����
//
//

int LoadZipArchive(TTreeView* TreeView, String filename, bool DirOnly = false)
{
    io::zip_archive<char> zip;
    int n = zip.open(filename.c_str(), 0, 0);
    if(n != CGL_OK){
        ShowMessage("err");
        return -1;
    }

    io::ls_data<char> ls;
    std::map<unistring, TTreeNode*> itembuf;
    n = 0;
    TreeView->Items->Clear();
    TreeView->Items->BeginUpdate();
    while(zip.ls(ls)){
        if(DirOnly && ls.name.back() != '/'){
            continue;
        }
        TTreeView_CreatePath(itembuf, TreeView, ls.name.c_str());
        ++n;
    }
    TreeView->Items->EndUpdate();

    itembuf.clear();
    zip.close();

    return n;
}

int TZipView::LoadArchive(String filename)
{
    return LoadZipArchive(tvwFiles, filename, false);
}

void TZipView::CloseArchive()
{
    m_archive.close();
}

void __fastcall TZipView::FrameClick(TObject *Sender)
{
    LoadArchive("icons.zip");
}
//---------------------------------------------------------------------------
