//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "AButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TAButton *)
{
        new TAButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAButton::TAButton(TComponent* Owner)
        : TCustomControl(Owner)
{
 FNormalBitmap=new Graphics::TBitmap();
 FOverBitmap=new Graphics::TBitmap();
 FClickBitmap=new Graphics::TBitmap();

 Width=32;
 Height=32;
}
//---------------------------------------------------------------------------
__fastcall TAButton::~TAButton()
{
 DeleteObject(FRegion);

 if(FNormalBitmap)
  delete FNormalBitmap;
 if(FOverBitmap)
  delete FOverBitmap;
 if(FClickBitmap)
  delete FClickBitmap;
}
//---------------------------------------------------------------------------
namespace Abutton
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TAButton)};
                 RegisterComponents("AliSoft", classes, 0);
        }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::Paint()
{
 if(!FNormalBitmap->Empty)
  {
   Width=FNormalBitmap->Width;
   Height=FNormalBitmap->Height;
   Canvas->Draw(0,0,FNormalBitmap);
  }
 else
  {
   Canvas->Brush->Color=clWhite;
   Canvas->Brush->Style=bsDiagCross;
   Canvas->Pen->Color=clBlack;
   Canvas->Pen->Style=psDashDot;
   Canvas->Ellipse(0,0,Width-1,Height-1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::MouseDown(TMouseButton Button,Classes::TShiftState Shift,int X,int Y)
{
 if(!FClickBitmap->Empty)
  {
   Canvas->Draw(0,0,FClickBitmap);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::MouseUp(TMouseButton Button,Classes::TShiftState Shift,int X,int Y)
{
 if(!FNormalBitmap->Empty)
  {
   Canvas->Draw(0,0,FOverBitmap);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::CMMouseEnter(TMessage &Message)
{
 MouseEnterEvent();
}
//---------------------------------------------------------------------------
void __fastcall TAButton::CMMouseExit(TMessage &Message)
{
 MouseExitEvent();
}
//---------------------------------------------------------------------------
void __fastcall TAButton::MouseEnterEvent()
{
 if(Enabled && FMouseFriendy)
  {
   if(!FOverBitmap->Empty)
    {
     Canvas->Draw(0,0,FOverBitmap);
    }
/*
   FOnMouseExitColor=Color;
   FFontOnMouseExit->Assign(Font);
   Color=FOnMouseColor;
   Font->Assign(FFontOnMouse);
*/
   if(FOnMouseEnterEvent)
    FOnMouseEnterEvent(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::MouseExitEvent()
{
 if(Enabled && FMouseFriendy)
  {
   if(!FNormalBitmap->Empty)
    {
     Canvas->Draw(0,0,FNormalBitmap);
    }
/*
   Color=FOnMouseExitColor;
   Font->Assign(FFontOnMouseExit);
*/
   if(FOnMouseExitEvent)
    FOnMouseExitEvent(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::SetNormalBitmap(Graphics::TBitmap *Value)
{
 FNormalBitmap->Assign(Value);
 Width=FNormalBitmap->Width;
 Height=FNormalBitmap->Height;
 RegionFromBMP(FNormalBitmap,FTransparentColor);
}
//---------------------------------------------------------------------------
void __fastcall TAButton::SetOverBitmap(Graphics::TBitmap *Value)
{
 FOverBitmap->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TAButton::SetClickBitmap(Graphics::TBitmap *Value)
{
 FClickBitmap->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TAButton::SetActive(bool Value)
{
 FActive=Value;
 if(FActive)
  {
   SetWindowRgn(Handle,CopyRegion(FRegion),true);
  } 
 else
  SetWindowRgn(Handle,0,true);
}
//---------------------------------------------------------------------------
void __fastcall TAButton::SetTransparentColor(TColor Value)
{
 FTransparentColor=Value;
 if(FActive)
  {
   RegionFromBMP(FNormalBitmap,FTransparentColor);
   SetWindowRgn(Handle,FRegion,true);
  } 
}
//---------------------------------------------------------------------------
void TAButton::RegionFromBMP(Graphics::TBitmap *Mask,TColor BackColor)
{
 TList *List=new TList();
 Graphics::TBitmap *BMP=new Graphics::TBitmap();
 BMP->Assign(Mask);
 BMP->Mask(BackColor);
 BMP->Monochrome=true;
 BMP->PixelFormat=pf32bit;
 TColor *PColor;
 for(int Y=0;Y<BMP->Height;Y++)
  {
   PColor=(TColor *)BMP->ScanLine[Y];
   for(int X=0;X<BMP->Width;)
    {
     if(PColor[X]==clBlack)
      {
       TRect *R=new TRect();
       R->Left=X;
       while(PColor[X]==clBlack)
        {
         X++;
         if(X>=BMP->Width)
          break;
        }
       R->Right=X;
       if(R->Left==R->Right)
        R->Right=X+1;
       R->Top=Y;
       if(Y<BMP->Width-1)
        R->Bottom=Y+1;

       List->Add(R);
      }
     else
      {
       X++;
      }
    }
  }

 RGNDATA *Data;
 DWORD Size;
 Size=sizeof(RGNDATA)+(List->Count*sizeof(TRect));
 Data=(RGNDATA *)malloc(Size);
 Data->rdh.dwSize=sizeof(RGNDATA);
 Data->rdh.iType=RDH_RECTANGLES;
 Data->rdh.nCount=List->Count;
 Data->rdh.nRgnSize=sizeof(TRect);
 Data->rdh.rcBound.left=0;
 Data->rdh.rcBound.top=0;
 Data->rdh.rcBound.right=BMP->Width;
 Data->rdh.rcBound.bottom=BMP->Height;
 TRect *PRect;
 PRect=(TRect *)Data->Buffer;
 for(int i=0;i<List->Count;i++)
  {
   *PRect=*(TRect *)List->Items[i];
   PRect++;
  }

 FRegion=ExtCreateRegion(NULL,sizeof(RGNDATA)+(List->Count*sizeof(TRect)),Data);

 if(List)
  {
   for(int i=0;i<List->Count;i++)
    delete (TRect *)List->Items[i];
   delete List;
  }
 if(BMP)
  delete BMP;
}
//---------------------------------------------------------------------------
HRGN TAButton::CopyRegion(HRGN Value)
{
 HRGN hRgn;
 hRgn=CreateRectRgn(0, 0, 1, 1);
 CombineRgn(hRgn, Value, 0, RGN_COPY);
 return hRgn;
}
//---------------------------------------------------------------------------

