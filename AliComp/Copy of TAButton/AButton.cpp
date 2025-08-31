//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAButton *)
{
        new TAButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAButton::TAButton(TComponent* Owner)
        : TGraphicControl(Owner)
{
 FNormalBitmap=new Graphics::TBitmap();
 FOverBitmap=new Graphics::TBitmap();
 FClickBitmap=new Graphics::TBitmap();

 FNormalRegion=NULL;
 FOverRegion=NULL;
 FClickRegion=NULL;

 Width=32;
 Height=32;

 FMouseFriendy=true;
}
//---------------------------------------------------------------------------
__fastcall TAButton::~TAButton()
{
 DeleteObject(FNormalRegion);
 DeleteObject(FOverRegion);
 DeleteObject(FClickRegion);

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
HRGN TAButton::RegionFromBMP(Graphics::TBitmap *Mask,TColor BackColor)
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
       R->Left=X-1;
       while(PColor[X]==clBlack)
        {
         X++;
         if(X>=BMP->Width)
          break;
        }
       R->Right=X+1;
       R->Top=Y-1;
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

 HRGN hRgn;
 hRgn=ExtCreateRegion(NULL,sizeof(RGNDATA)+(List->Count*sizeof(TRect)),Data);

 if(List)
  {
   for(int i=0;i<List->Count;i++)
    delete (TRect *)List->Items[i];
   delete List;
  } 
 if(BMP)
  delete BMP;

 return hRgn; 
}
//---------------------------------------------------------------------------
/*
void __fastcall TAButton::Draw()
{
 TransparentStretchBlt(ACanvas.FHandle, Left, Top, Right - Left,
            Bottom - Top, Canvas.FHandle, 0, 0, FDIB.dsbm.bmWidth,
            FDIB.dsbm.bmHeight, MaskDC, 0, 0);
}
*/
void __fastcall TAButton::Paint()
{
 if(!FNormalBitmap->Empty)
  {
   FNormalBitmap->Transparent=FTransparent;
   FNormalBitmap->TransparentColor=clWhite;
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
   FClickBitmap->Transparent=FTransparent;
   FClickBitmap->TransparentColor=clWhite;
   Canvas->Draw(0,0,FClickBitmap);
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
void __fastcall TAButton::MouseUp(TMouseButton Button,Classes::TShiftState Shift,int X,int Y)
{
 if(!FNormalBitmap->Empty)
  {
   FNormalBitmap->Transparent=FTransparent;
   FNormalBitmap->TransparentColor=clWhite;
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
     FOverBitmap->Transparent=FTransparent;
     FOverBitmap->TransparentColor=clWhite;
     Canvas->Draw(0,0,FOverBitmap);
    }
   else
    {
     Canvas->Brush->Color=clWhite;
     Canvas->Brush->Style=bsDiagCross;
     Canvas->Pen->Color=clBlue;
     Canvas->Pen->Style=psDashDot;
     Canvas->Ellipse(0,0,Width-1,Height-1);
    }
/*
   FOnMouseExitColor=Color;
   FFontOnMouseExit->Assign(Font);
   Color=FOnMouseColor;
   Font->Assign(FFontOnMouse);
   if(FOnMouseEnterEvent)
    FOnMouseEnterEvent(this);
*/
  }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::MouseExitEvent()
{
 if(Enabled && FMouseFriendy)
  {
   if(!FNormalBitmap->Empty)
    {
     FNormalBitmap->Transparent=FTransparent;
     FNormalBitmap->TransparentColor=clWhite;
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
/*
   Color=FOnMouseExitColor;
   Font->Assign(FFontOnMouseExit);
   if(FOnMouseExitEvent)
    FOnMouseExitEvent(this);
*/
  }
}
//---------------------------------------------------------------------------
void __fastcall TAButton::SetNormalBitmap(Graphics::TBitmap *Value)
{
 FNormalBitmap->Assign(Value);
 Width=FNormalBitmap->Width;
 Height=FNormalBitmap->Height;

 Invalidate();
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
void __fastcall TAButton::SetTransparent(bool Value)
{
 if(FTransparent!=Value)
  {
   FTransparent=Value;
   Invalidate(); 
  }
}
//---------------------------------------------------------------------------
/*
procedure TCanvas.Draw(X, Y: Integer; Graphic: TGraphic);
begin
  if (Graphic <> nil) and not Graphic.Empty then
  begin
    Changing;
    RequiredState([csHandleValid]);
    SetBkColor(FHandle, ColorToRGB(FBrush.Color));
    SetTextColor(FHandle, ColorToRGB(FFont.Color));
    Graphic.Draw(Self, Rect(X, Y, X + Graphic.Width, Y + Graphic.Height));
    Changed;
  end;
end;

procedure TBitmap.Draw(ACanvas: TCanvas; const Rect: TRect);
var
  OldPalette: HPalette;
  RestorePalette: Boolean;
  DoHalftone: Boolean;
  Pt: TPoint;
  BPP: Integer;
  MaskDC: HDC;
  Save: THandle;
begin
  with Rect, FImage do
  begin
    ACanvas.RequiredState(csAllValid);
    PaletteNeeded;
    OldPalette := 0;
    RestorePalette := False;

    if FPalette <> 0 then
    begin
      OldPalette := SelectPalette(ACanvas.FHandle, FPalette, True);
      RealizePalette(ACanvas.FHandle);
      RestorePalette := True;
    end;
    BPP := GetDeviceCaps(ACanvas.FHandle, BITSPIXEL) *
      GetDeviceCaps(ACanvas.FHandle, PLANES);
    DoHalftone := (BPP <= 8) and (BPP < (FDIB.dsbm.bmBitsPixel * FDIB.dsbm.bmPlanes));
    if DoHalftone then
    begin
      GetBrushOrgEx(ACanvas.FHandle, pt);
      SetStretchBltMode(ACanvas.FHandle, HALFTONE);
      SetBrushOrgEx(ACanvas.FHandle, pt.x, pt.y, @pt);
    end else if not Monochrome then
      SetStretchBltMode(ACanvas.Handle, STRETCH_DELETESCANS);
    try
      { Call MaskHandleNeeded prior to creating the canvas handle since
        it causes FreeContext to be called. }
      if Transparent then MaskHandleNeeded;
      Canvas.RequiredState(csAllValid);
      if Transparent then
      begin
        Save := 0;
        MaskDC := 0;
        try
          MaskDC := GDICheck(CreateCompatibleDC(0));
          Save := SelectObject(MaskDC, FMaskHandle);
          TransparentStretchBlt(ACanvas.FHandle, Left, Top, Right - Left,
            Bottom - Top, Canvas.FHandle, 0, 0, FDIB.dsbm.bmWidth,
            FDIB.dsbm.bmHeight, MaskDC, 0, 0);
        finally
          if Save <> 0 then SelectObject(MaskDC, Save);
          if MaskDC <> 0 then DeleteDC(MaskDC);
        end;
      end
      else
        StretchBlt(ACanvas.FHandle, Left, Top, Right - Left, Bottom - Top,
          Canvas.FHandle, 0, 0, FDIB.dsbm.bmWidth,
          FDIB.dsbm.bmHeight, ACanvas.CopyMode);
    finally
      if RestorePalette then
        SelectPalette(ACanvas.FHandle, OldPalette, True);
    end;
  end;
end;

function TransparentStretchBlt(DstDC: HDC; DstX, DstY, DstW, DstH: Integer;
  SrcDC: HDC; SrcX, SrcY, SrcW, SrcH: Integer; MaskDC: HDC; MaskX,
  MaskY: Integer): Boolean;
const
  ROP_DstCopy = $00AA0029;
var
  MemDC: HDC;
  MemBmp: HBITMAP;
  Save: THandle;
  crText, crBack: TColorRef;
  SavePal: HPALETTE;
begin
  Result := True;
  if (Win32Platform = VER_PLATFORM_WIN32_NT) and (SrcW = DstW) and (SrcH = DstH) then
  begin
    MemBmp := GDICheck(CreateCompatibleBitmap(SrcDC, 1, 1));
    MemBmp := SelectObject(MaskDC, MemBmp);
    try
      MaskBlt(DstDC, DstX, DstY, DstW, DstH, SrcDC, SrcX, SrcY, MemBmp, MaskX,
        MaskY, MakeRop4(ROP_DstCopy, SrcCopy));
    finally
      MemBmp := SelectObject(MaskDC, MemBmp);
      DeleteObject(MemBmp);
    end;
    Exit;
  end;
  SavePal := 0;
  MemDC := GDICheck(CreateCompatibleDC(0));
  try
    MemBmp := GDICheck(CreateCompatibleBitmap(SrcDC, SrcW, SrcH));
    Save := SelectObject(MemDC, MemBmp);
    SavePal := SelectPalette(SrcDC, SystemPalette16, False);
    SelectPalette(SrcDC, SavePal, False);
    if SavePal <> 0 then
      SavePal := SelectPalette(MemDC, SavePal, True)
    else
      SavePal := SelectPalette(MemDC, SystemPalette16, True);
    RealizePalette(MemDC);

    StretchBlt(MemDC, 0, 0, SrcW, SrcH, MaskDC, MaskX, MaskY, SrcW, SrcH, SrcCopy);
    StretchBlt(MemDC, 0, 0, SrcW, SrcH, SrcDC, SrcX, SrcY, SrcW, SrcH, SrcErase);
    crText := SetTextColor(DstDC, $0);
    crBack := SetBkColor(DstDC, $FFFFFF);
    StretchBlt(DstDC, DstX, DstY, DstW, DstH, MaskDC, MaskX, MaskY, SrcW, SrcH, SrcAnd);
    StretchBlt(DstDC, DstX, DstY, DstW, DstH, MemDC, 0, 0, SrcW, SrcH, SrcInvert);
    SetTextColor(DstDC, crText);
    SetBkColor(DstDC, crBack);

    if Save <> 0 then SelectObject(MemDC, Save);
    DeleteObject(MemBmp);
  finally
    if SavePal <> 0 then SelectPalette(MemDC, SavePal, False);
    DeleteDC(MemDC);
  end;
end;
*/
