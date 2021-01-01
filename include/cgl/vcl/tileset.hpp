/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 tileset.hpp

 2019-05-01 10:02:32

*/
#ifndef TILESET_HPP_20190501100232
#define TILESET_HPP_20190501100232

#include <cgl/public.h>
#include <vector>

struct TTileSetItem
{
    int ID;
    TObject* Object;
};

class TTileSet : public TGraphicControl
{
private:
    TPoint FGrid;
    int FGridSize;
    TPoint FHot;
    TPoint FSelected;
    TPoint FDrag;
    TPoint FLastPos;
    std::vector<TTileSetItem> FItems;
    bool FDown;

public:
    __fastcall virtual TTileSet(Classes::TComponent* AOwner) : TGraphicControl(AOwner)
    {
        Width = 64;
        Height = 64;
        setGrid(2, 2);
        GridSize = 16;
        //Canvas->Pen->Color = clWhite;
        //Canvas->Brush->Color = clGray;

        //this->AutoSize = true;

        //this->DragMode = dmAutomatic;
        //this->DragKind = dkDock;
        //this->DragCursor = crDrag;
    }

    void setGrid(int x, int y)
    {
        FGrid = TPoint(x, y);
        FItems.resize(x * y);
        for(size_t i=0; i<FItems.size(); ++i){
            FItems[i].ID = i;
        }

        if(this->AutoSize){
        }

    }

    bool is_hotspot(int x, int y)
    {
        return x == FHot.x && y == FHot.y;
    }

    bool is_selected(int x, int y)
    {
        return x == FSelected.x && y == FSelected.y;
    }

    void (__closure *OnDrawItem)(TObject* Sender, TCanvas* Canvas, int Index, TRect ARect);

    __property int GridSize={read=FGridSize,write=SetGridSize};

    __property Color;

private:
    void SetGridSize(int size)
    {
        FGridSize = size;
        if(this->AutoSize){
            this->Width = 0;
        }
    }

protected:

    virtual bool __fastcall CanResize(int &NewWidth, int &NewHeight)
    {
        if(this->AutoSize){
            NewWidth = FGrid.x * FGridSize;
            NewHeight = FGrid.y * FGridSize;
        }
    }

    virtual void __fastcall Paint(void)
    {
        Canvas->Brush->Color = this->Color;
        Canvas->FillRect(this->ClientRect);

        int px, py;
        int id;

        for(int y=0; y<FGrid.y; ++y)
        {
            for(int x=0; x<FGrid.x; ++x)
            {
                px = x * FGridSize;
                py = y * FGridSize;

                //TCustomDrawState
                if(is_hotspot(x, y)){
                    Canvas->Brush->Color = clSkyBlue;
                }
                else if(is_selected(x, y)){
                    Canvas->Brush->Color = clBlue;
                }
                else{
                    Canvas->Brush->Color = clSilver;
                }

                if(OnDrawItem){
                    OnDrawItem(this, Canvas, id, TRect(px, py, FGridSize, FGridSize));
                }
                else{
                    Canvas->Rectangle(px, py, px + FGridSize, py + FGridSize);
                    id = y * FGrid.x + x;
                    Canvas->TextOut(px + 2, py + 2, FItems[id].ID);
                }
            }
        }
    }

    DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
    {
        FDown = true;

        if(is_valid_area(X, Y)){
            FSelected = FHot;
        }

        FLastPos = TPoint(X, Y);

        this->Repaint();
    }

    DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y)
    {
        if(is_valid_area(X, Y)){
            FHot.x = X / FGridSize;
            FHot.y = Y / FGridSize;
            FDrag = FHot;
            //mouse offset 3
            if(FDown)
            if(abs(X - FLastPos.x) >= 3 || abs(Y - FLastPos.y) >= 3){
                print(FLastPos.x);
                //this->DragMode = dmAutomatic;
                this->BeginDrag(true);
            }
        }
        else{
            //this->DragMode = dmManual;
            FHot = TPoint(-1, -1);
            FLastPos = TPoint(X, Y);
        }

        this->Repaint();
    }

    DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
    {
        if(FDown){
            FDown = false;
        }
    }

    bool is_valid_area(int x, int y)
    {
        return 0 < x && 0 < y && x < FGrid.x*FGridSize && y < FGrid.y*FGridSize;
    }

    DYNAMIC void __fastcall DragOver(System::TObject* Source, int X, int Y, TDragState State, bool &Accept)
    {
        FHot.x = X / FGridSize;
        FHot.y = Y / FGridSize;

        this->Repaint();

        Accept = is_valid_area(X, Y);

        print("over");
    }

    DYNAMIC void __fastcall DoEndDrag(System::TObject* Target, int X, int Y)
    {
        if(is_valid_area(X, Y)){
            FHot.x = X / FGridSize;
            FHot.y = Y / FGridSize;
            int src = FDrag.y * FGrid.x + FDrag.x;
            int dst = FHot.y * FGrid.x + FHot.x;
            std::swap(FItems[dst], FItems[src]);
        }
    }

};



#endif //TILESET_HPP_20190501100232