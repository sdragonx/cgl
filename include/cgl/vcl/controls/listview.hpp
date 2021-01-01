/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 TListView

 2020-02-16 23:49:27

设置图标大小？
SendMessage(ListView1->Handle, LVM_SETWORKAREA, 1, (LONG)&rc);
SendMessage(ListView1->Handle, LVM_SETICONSPACING, 0, 0x10000*64 + 80);

*/
#ifndef TLISTVIEW_20200216234927
#define TLISTVIEW_20200216234927

#include <cgl/public.h>

namespace cgl{
namespace vcl{

//取得Column的Left值
//2007-1-20 13:43:30 by sdragon
int TListView_ColumnLeft(TCustomListView* CustomListView, int ColumnIndex)
{
    TListView* ListView = dynamic_cast<TListView*>(CustomListView);
    if(ListView && ColumnIndex<ListView->Columns->Count)
    {
        int nLeft = 0;
        for(int i=0; i<ColumnIndex; ++i)
        {
            nLeft += ListView->Column[i]->Width;
        }
        return nLeft;
    }
    else
        return -1;
}

//取得X坐标处的Column索引，失败返回-1
int TListView_ColumnIndex(TCustomListView* CustomListView, int X)
{
    TListView* ListView = dynamic_cast<TListView*>(CustomListView);
    if(ListView)
    {
        int nLeft = 0;
        X += GetScrollPos(ListView->Handle, SB_HORZ);
        for(int i=0; i<ListView->Columns->Count; ++i)
        {
            if( (X>=nLeft) && ( X<(nLeft += ListView->Column[i]->Width) ) )
            {
                return i;
            }
        }
        return -1;
    }
    else
        return -1;
}

//返回SubItem范围，失败返回-1
int SubItemRect(TListItem* Item, int SubItemIndex, TRect& Rect)
{
    /*if(SubItem == 0)
    {
        Rect = Item->DisplayRect(drLabel);
    }
    else*/
    if(SubItemIndex<Item->SubItems->Count)
    {
        Rect = Item->DisplayRect(drSelectBounds);
        Rect.left  = TListView_ColumnLeft(Item->ListView, SubItemIndex+1);
        Rect.right = Rect.left + Item->ListView->Column[SubItemIndex+1]->Width;
        return 0;
    }
    else
        return -1;
}


//获得列表模式下子项的边界范围
TRect TListView_SubItemRect(TListView* ListView, TListItem* Item, int SubItem)
{
    TRect rc = TRect();
    if(ListView->ViewStyle != vsReport){
        rc.right = -1;
        return rc;
    }
    rc = Item->DisplayRect(drBounds);
    if(ListView->Columns->Count){
        rc.left += ListView->Columns->Items[0]->Width;
    }
    for(int i=1; i<ListView->Columns->Count; ++i)
    {
        if(i-1 == SubItem){
            rc.right = rc.left + ListView->Columns->Items[i]->Width;
            break;
        }
        rc.left += ListView->Columns->Items[i]->Width;
    }
    TPoint pt = ListView->ClientToParent(TPoint(0, 0), ListView->Parent);
    OffsetRect(rc, pt.x, 0);
    return rc;
}

//点击标头排序
int TListView_ColumnSort(TListItem *Item1, TListItem *Item2, int ColumnIndex, bool less = true)
{
    String str1, str2;
    if(ColumnIndex == 0){
        str1 = Item1->Caption;
        str2 = Item2->Caption;
    }
    else{
        str1 = Item1->SubItems->Strings[ColumnIndex - 1];
        str2 = Item2->SubItems->Strings[ColumnIndex - 1];
    }

    return less ? str1 < str2 : str1 > str2;
}


DWORD ListView_ColumnAlignment(TCustomListView* Sender, int id)
{
    switch(Sender->Column[id]->Alignment){
    case taLeftJustify:
        return DT_LEFT;
    case taRightJustify:
        return DT_RIGHT;
    case taCenter:
        return DT_CENTER;
    default:
        return DT_LEFT;
    }
}

//ListView自绘
void ListReport_CustomDraw(TCustomListView *Sender, TListItem *Item, TCustomDrawState State, bool &DefaultDraw)
{
    TListView* ListView = dynamic_cast<TListView*>(Sender);
    if(!ListView && ListView->ViewStyle != vsReport){
        return ;
    }

    if(State.ToInt() == 0){
        DefaultDraw = false;
        return ;
    }

	TRect  rc = Item->DisplayRect(drBounds);
	int nColumnLeft;
	int nSpacing = Sender->Canvas->TextWidth("A");

    //Item->Handle
    //绘制背景
//    if(State.Contains(cdsSelected)){//错误
//    if(Item->Selected){
    if(State.Contains(cdsFocused)){
		Sender->Canvas->Brush->Color = clSkyBlue;
        Sender->Canvas->FillRect(rc);
	}
    else{
        Sender->Canvas->Brush->Color = Item->Index & 1 ? TColor(0xFFF0E0) : clWhite;
        Sender->Canvas->FillRect(rc);
    }

    if(State.Contains(cdsHot)){
        Sender->Canvas->Pen->Color = clHighlight;
        Sender->Canvas->Brush->Style = bsClear;
        Sender->Canvas->Rectangle(rc);
    }

	//绘制CheckBox
    /*
	if(Sender->Checkboxes){
		rc = Item->DisplayRect(drBounds);
		rc.right = Item->Left;
		Sender->Canvas->Brush->Color = clBtnFace;
		if(!State.Contains(cdsSelected) && !State.Contains(cdsHot))
		{
			Sender->Canvas->FillRect(rc);
			Sender->Canvas->Pen->Color = clSilver;
			Sender->Canvas->MoveTo(rc.right-1, rc.top);
			Sender->Canvas->LineTo(rc.right-1, rc.bottom);
		}
		if(ListItem_IsPrimary(Item)){
			ImageList1->Draw(Sender->Canvas,
				rc.left,
				rc.top + ((rc.Height()-ImageList1->Height)>>1),
				(Item->Checked ? 1: 0));
		}
	}
    */

    //图标
    //lvwTask->LargeImages
//	if(Item->Indent<=0){
//		ImageList1->Draw(Sender->Canvas,
//			rc.right-ImageList1->Width,
//			rc.top+((rc.Height()-ImageList1->Height)>>1),
//			(Item->Indent==0 ? 2: 3));
//	}

    HDC dc = Sender->Canvas->Handle;
    DWORD alignment;
	//绘制Caption
	SetBkMode(dc, TRANSPARENT);
	if(!Item->Caption.IsEmpty()){
        alignment = ListView_ColumnAlignment(Sender, 0);
		rc = Item->DisplayRect(drLabel);
        ++rc.left;
		DrawTextW(dc, Item->Caption.c_str(), -1, &rc,
			alignment | DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//绘制SubItems
	rc = Item->DisplayRect(drBounds);
	nColumnLeft = rc.left + ListView_GetColumnWidth(Sender->Handle, 0);
	rc.right = nColumnLeft;
	for(int i=0; i<Item->SubItems->Count; ++i){
        alignment = ListView_ColumnAlignment(Sender, i + 1);
		rc.left  = nColumnLeft;
		rc.right = rc.left + ListView_GetColumnWidth(Sender->Handle, i + 1);
		nColumnLeft = rc.right;
		rc.left  += nSpacing;
		rc.right -= nSpacing;
		DrawTextW(dc, Item->SubItems->Strings[i].c_str(), -1, &rc,
			alignment | DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	DefaultDraw = false;
}



/*

bool TListView_AdvancedCustomDraw(TCustomListView *Sender, const TRect &ARect, 
    TCustomDrawStage Stage, bool &DefaultDraw);


//完美自绘cb6
void __fastcall TForm1::ListView1AdvancedCustomDraw(
      TCustomListView *Sender, const TRect &ARect, TCustomDrawStage Stage,
      bool &DefaultDraw)
{
    if(Stage == cdPrePaint)//背景？
    {
        PaintDesktop(Sender->Canvas->Handle);
        //Sender->Canvas->FillRect(ARect);
        Sender->Canvas->Rectangle(0, 0, random(500), random(500));
    }

}

//图标透明
void __fastcall TForm1::ListView1AdvancedCustomDrawItem(
      TCustomListView *Sender, TListItem *Item, TCustomDrawState State,
      TCustomDrawStage Stage, bool &DefaultDraw)
{
    ::SetBkMode(Sender->Canvas->Handle, TRANSPARENT);
    ::SendMessage(Sender->Handle, LVM_SETTEXTBKCOLOR, 0, (LPARAM)CLR_NONE);
    ::SendMessage(Sender->Handle, LVM_SETBKCOLOR, 0, (LPARAM)CLR_NONE);

}
*/

}//end namespace vcl
}//end namespace cgl

#endif //TLISTVIEW_20200216234927