//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ARoundButton.h"
#include "Math.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TARoundButton *)
{
    new TARoundButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TARoundButton::TARoundButton(TComponent* Owner)
    : TGraphicControl(Owner)
{
 FGlyph=new Graphics::TBitmap();
 FNumGlyphs=1;
 FDown=False;
 FMouseInside=False;
 Width=DefaultWidth;
 Height=DefaultHeight;
 FMouseDown=False;
 TempCursor=Cursor;
// FMargin=4;
 FPenWidth=3;
 FColor=clBtnFace;
 FClickTime=100;

 Timer=new TTimer(this);
 Timer->Interval=FClickTime;
 Timer->Enabled=false;
 Timer->OnTimer=DoTimer;
}
//---------------------------------------------------------------------------
__fastcall TARoundButton::~TARoundButton()
{
 if(FGlyph)
  delete FGlyph;
 if(Timer)
  delete Timer; 
}
//---------------------------------------------------------------------------
namespace Aroundbutton
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TARoundButton)};
         RegisterComponents("AliSoft", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::DoTimer(TObject *Sender)
{
 Timer->Enabled=false;
 FDown=False;// release button and repaint
 PaintButton();
 if(FOnClick) // issue event to application
  FOnClick(this);
}
//---------------------------------------------------------------------------
__inline int __fastcall TARoundButton::Round(float f)
{
 return ((f-int(f)>=0.5)?int(f)+1:int(f));
}
//---------------------------------------------------------------------------
bool __fastcall TARoundButton::Assigned(void *P)
{
 if(P)
  return true;
 else
  return false; 
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetGlyph(Graphics::TBitmap *newGlyph)
{
 if(Assigned(FGlyph))
  {
   FGlyph->Assign(newGlyph);
   if(ComponentState.Contains(csDesigning))
    {
     // bitmap 1: main, 2: disabled, 3: down;
     //  must have dimensions of (height * NoBitmaps) = width
     if((newGlyph->Width % newGlyph->Height)== 0)
      FNumGlyphs=newGlyph->Width / newGlyph->Height;
     else
      FNumGlyphs=1;
    }
   Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetNumGlyphs(int newNumGlyphs)
{
 if(FNumGlyphs!=newNumGlyphs)
  {
   FNumGlyphs=newNumGlyphs;
   Invalidate();
  } 
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetRButtonType(TRButtonType newRButtonType)
{
 if(FRButtonType!=newRButtonType)
  {
   FRButtonType=newRButtonType;
   Invalidate();
  } 
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetTransparentColor(TColor newTransparentColor)
{
 if(FTransparentColor!=newTransparentColor)
  {
   FTransparentColor=newTransparentColor;
   Invalidate();
  } 
}
//---------------------------------------------------------------------------
bool __fastcall TARoundButton::IsInsideButton(int X,int Y)
{
/*
 HRGN hRgn;
 bool Result;
 hRgn=CreateEllipticRgn(FMargin+FPenWidth,FMargin+FPenWidth,Width-FMargin-FPenWidth-1,Height-FMargin-FPenWidth-1);
 if(PtInRegion(hRgn,X,Y))
  Result=true;
 else
  Result=false;
 DeleteObject(hRgn);
 return Result;
*/

 int Hypotonuse;
 X=abs((Width >> 1)-FMargin-Canvas->Pen->Width-X);// calculate the (X,Y) distance
 Y=abs((Height >> 1)-FMargin-Canvas->Pen->Width-Y);
 Hypotonuse=Round(sqrt(abs((X * X) + (Y * Y))));
 // calculate hypotenuse (distance from center)
 return (Hypotonuse <= (Width >> 1)-FMargin-Canvas->Pen->Width-1);
 //return true if Hypotonuse less than half of width
 
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::Paint()
{
 Canvas->Brush->Color=FColor;
 switch(FRButtonType)
  {
   case rtRegular:
    {
     Canvas->Pen->Color=clBlack;
     Canvas->Ellipse(0,0,Width-1,Height-1);// fill inner button, black edge
     break;
    }
   case rtInset:
    {
     Canvas->Pen->Color=clBtnFace;
     Canvas->Ellipse(1,1,Width-2,Height-2);// fill inner button

     Canvas->Pen->Color=clBtnShadow;// shadowed edge
     Canvas->Arc(0,0,Width-1,Height-1,Width/5*4,Height/5,Width/5,Height/5*4);
     Canvas->Pen->Color=clBtnHighlight;// highlighted edge
     Canvas->Arc(0,0,Width-1,Height-1,Width/5,Height/5*4,Width/5*4,Height/5);
     break;
    }
  }
 PaintButton();// repaint rest
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::PaintButton()
{
 TRect Dest,Source;
 int outWidth,outHeight;

 if(Assigned(FGlyph))// if has a bitmap 
  {
   // setup bounding rectangle
   Source.Left=0;
   Source.Top=0;
   Source.Right=FGlyph->Width;
   Source.Bottom=FGlyph->Height;
   if(FNumGlyphs>0)
    Source.Right=Source.Right/FNumGlyphs;
  }

 Canvas->Pen->Width=FPenWidth;

 if(FDown)
  Canvas->Pen->Color=clBtnShadow;
 else
  Canvas->Pen->Color=clBtnHighlight;
 Canvas->Arc(FMargin+FPenWidth,FMargin+FPenWidth,Width-FMargin-FPenWidth-1,Height-FMargin-FPenWidth-1,Width/5*4,Height/5,Width/5,Height/5*4);
 // draw bottom right edge
 if(!FDown)
  Canvas->Pen->Color=clBtnShadow;
 else
  Canvas->Pen->Color=clBtnHighlight;
 Canvas->Arc(FMargin+FPenWidth,FMargin+FPenWidth,Width-FMargin-FPenWidth-1,Height-FMargin-FPenWidth-1,Width/5,Height/5*4,Width/5*4,Height/5);

 Canvas->Pen->Width=1;
//---place glyph---------------------------------------
 if(Assigned(FGlyph)&&(FNumGlyphs>0))
  {
   if((!Enabled)&&(FNumGlyphs>1))
    {
     Source.Left=FGlyph->Width/FNumGlyphs;
     Source.Right=Source.Left << 1;
    }
   // setup output image retangle
   outWidth=Source.Right-Source.Left;
   outHeight=Source.Bottom-Source.Top;
   // find center
   Dest.Left=((Width-outWidth) >> 1);
   Dest.Right=((Width-outWidth) >> 1)+outWidth;
   Dest.Top=((Height-outHeight) >> 1);
   Dest.Bottom=((Height-outHeight) >> 1)+outHeight;
   Canvas->Pen->Color=clBtnFace;
   if(FDown)
    {
     // shift image position down and right 1
     Dest.Left++;
     Dest.Right++;
     Dest.Top++;
     Dest.Bottom++;
     // clear ghost
     Canvas->MoveTo(Dest.Left-1,Dest.Bottom);
     Canvas->LineTo(Dest.Left-1,Dest.Top-1);
     Canvas->LineTo(Dest.Right,Dest.Top-1);
    }
   else
    {
     // clear ghost
     Canvas->MoveTo(Dest.Right,Dest.Top);
     Canvas->LineTo(Dest.Right,Dest.Bottom);
     Canvas->LineTo(Dest.Left,Dest.Bottom);
    }

   if(FDown && (FNumGlyphs > 2)) // show pushed image if available
    {
     Source.Left=FGlyph->Width/FNumGlyphs*2;
     Source.Right=FGlyph->Width/FNumGlyphs*3;
    }
   Canvas->Brush->Color=clBtnFace;// paint transparent image
   Canvas->BrushCopy(Dest,FGlyph,Source,FTransparentColor);
  }
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::MouseDown(TMouseButton Button,TShiftState Shift,int X,int Y)
{
 if(Enabled && IsInsideButton(X,Y))
  {
   FDown=true;// push button and repaint
   PaintButton();
  }
 FMouseDown=true;
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::MouseUp(TMouseButton Button,TShiftState Shift,int X,int Y)
{
 if(Enabled && IsInsideButton(X,Y))
  {
   FDown=False;// release button and repaint
   PaintButton();
   if(FOnClick) // issue event to application
    FOnClick(this);
  }
 FMouseDown=false;
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::MouseMove(TShiftState Shift,int X,int Y)
{
 if(IsInsideButton(X,Y))
  {
   TempCursor=Cursor;
   Cursor=FOverCursor;
  }
 else
  {
   Cursor=TempCursor;
   if(FMouseDown)
    {
     // mouse has slid off, so release
     if(FDown)
      {
       FDown=false;
       PaintButton();
      }
     else
      {
       // mouse has slid back on, so push
       FDown=true;
       PaintButton();
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::DoClick()
{
 Timer->Enabled=true;
 FDown=true;// push button and repaint
 PaintButton();
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetOverCursor(TCursor Value)
{
 if(FOverCursor!=Value)
  {
   FOverCursor=Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetMargin(int Value)
{
 if(FMargin!=Value)
  {
   FMargin=Value;
   Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetColor(TColor Value)
{
 if(FColor!=Value)
  {
   FColor=Value;
   Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TARoundButton::SetClickTime(int Value)
{
 if(FClickTime!=Value)
  {
   FClickTime=Value;
   Timer->Interval=FClickTime;
  }
}
//---------------------------------------------------------------------------

