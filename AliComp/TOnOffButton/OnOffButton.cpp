//---------------------------------------------------------------------------
//   Up
//   |        Down -- Up
//  Down
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "OnOffButton.h"
#include <Math.hpp>
#include "Math.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TOnOffButton *)
{
    new TOnOffButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TOnOffButton::TOnOffButton(TComponent* Owner)
    : TGraphicControl(Owner)
{
 FPen=new TPen();
 FPen->OnChange=Changed;
 FBrush=new TBrush();
 FBrush->OnChange=Changed;
 FFont=new TFont();
 FFont->OnChange=Changed;
 FUpFont=new TFont();
 FUpFont->OnChange=Changed;
 FDownFont=new TFont();
 FDownFont->OnChange=Changed;

 Width=75;
 Height=106;
/*
 FPen->Width=5;
 FPen->Color=clGray;
 FBrush->Style=bsSolid;
 FBrush->Color=clGreen;
*/
 FFont->Name="Traditional Arabic";
 FFont->Size=14;
 FUpFont->Color=clBlue;
 FUpFont->Style=TFontStyles()<<fsBold;
 FUpFont->Name="Traditional Arabic";
 FUpFont->Size=14;
 FDownFont->Color=clRed;
 FDownFont->Style=TFontStyles()<<fsBold;
 FDownFont->Name="Traditional Arabic";
 FDownFont->Size=14;
 FOuterRadius=20;
 FInnerRadius=8;
 FUpText="ÇáäÕ ÇáÚáæí";
 FDownText="ÇáäÕ ÇáÓÝáí";
 FAngle=40;
 HandleColor=clGray;
 FActive=false;
}
//---------------------------------------------------------------------------
__fastcall TOnOffButton::~TOnOffButton()
{
 if(FPen)
  delete FPen;
 if(FBrush)
  delete FBrush; 
 if(FFont)
  delete FFont;
 if(FUpFont)
  delete FUpFont;
 if(FDownFont)
  delete FDownFont;
}
//---------------------------------------------------------------------------
namespace Onoffbutton
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TOnOffButton)};
         RegisterComponents("AliSoft", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::Changed(TObject *Sender)
{
 Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::Paint()
{
 TPoint C;
 int X,Y;
 int X1,Y1,X2,Y2,X3,Y3,X4,Y4;
 C=Point(Width/2,Height/2);

 // Attributes
 Canvas->Pen=FPen;
 Canvas->Brush=FBrush;

 // Circles
 Canvas->Ellipse(C.x-FOuterRadius,C.y-FOuterRadius,C.x+FOuterRadius,C.y+FOuterRadius);
 Canvas->Ellipse(C.x-FInnerRadius,C.y-FInnerRadius,C.x+FInnerRadius,C.y+FInnerRadius);

 // Texts
 Canvas->Brush->Style=bsClear;
 if(FIsUp)
  {
   if(FOrientation)
    {
     // Up Text
     Canvas->Font=FUpFont;
     X=(Width-Canvas->TextWidth(FUpText))/2;
     Y=0;
     UpRect=Rect(0,0,Width,-FDownFont->Height+2*3);
     Canvas->TextOut(X,Y,FUpText);

     // Down Text
     Canvas->Font=FFont;
     X=(Width-Canvas->TextWidth(FDownText))/2;
     Y=Height-(-FDownFont->Height)-2*3;
     DownRect=Rect(0,Height-(-FDownFont->Height)-2*3,Width,Height);
     Canvas->TextOut(X,Y,FDownText);
    }
   else
    {
     // Up Text
     Canvas->Font=FUpFont;
     X=Width-Canvas->TextWidth(FUpText);
     Y=(Height-Canvas->TextHeight(FUpText))/2;
     UpRect=Rect(X,0,Width,Height);
     Canvas->TextOut(X,Y,FUpText);

     // Down Text
     Canvas->Font=FFont;
     X=0;
     Y=(Height-Canvas->TextHeight(FDownText))/2;
     DownRect=Rect(0,Y,Canvas->TextWidth(FDownText),Y+Canvas->TextHeight(FDownText));
     Canvas->TextOut(X,Y,FDownText);
    }
  }
 else
  {
   if(FOrientation)
    {
     // Up Text
     Canvas->Font=FFont;
     X=(Width-Canvas->TextWidth(FUpText))/2;
     Y=0;
     UpRect=Rect(0,0,Width,-FDownFont->Height+2*3);
     Canvas->TextOut(X,Y,FUpText);

     // Down Text
     Canvas->Font=FDownFont;
     X=(Width-Canvas->TextWidth(FDownText))/2;
     Y=Height-(-FDownFont->Height)-2*3;
     DownRect=Rect(0,Height-(-FDownFont->Height)-2*3,Width,Height);
     Canvas->TextOut(X,Y,FDownText);
    }
   else
    {
     // Up Text
     Canvas->Font=FFont;
     X=Width-Canvas->TextWidth(FUpText);
     Y=(Height-Canvas->TextHeight(FUpText))/2;
     UpRect=Rect(X,0,Width,Height);
     Canvas->TextOut(X,Y,FUpText);

     // Down Text
     Canvas->Font=FDownFont;
     X=0;
     Y=(Height-Canvas->TextHeight(FDownText))/2;
     DownRect=Rect(0,Y,Canvas->TextWidth(FDownText),Y+Canvas->TextHeight(FDownText));
     Canvas->TextOut(X,Y,FDownText);
    }
  }

 // Handle
 Canvas->Brush->Color=FHandleColor;
 Canvas->Brush->Style=bsSolid;
 Canvas->Pen->Color=FHandleColor;
 Canvas->Pen->Width=5;
 X1=C.x-FOuterRadius-3-FPen->Width/2;
 Y1=C.y-FOuterRadius-3-FPen->Width/2;
 X2=C.x+FOuterRadius+3+FPen->Width/2;
 Y2=C.y+FOuterRadius+3+FPen->Width/2;
 if(FIsUp)
  {
   if(FOrientation)
    {
     X3=C.x+FOuterRadius*sin(DegToRad(FAngle/2.0));
     Y3=C.y-FOuterRadius*cos(DegToRad(FAngle/2.0));
     X4=C.x-FOuterRadius*sin(DegToRad(FAngle/2.0));
     Y4=C.y-FOuterRadius*cos(DegToRad(FAngle/2.0));
    }
   else
    {
     X3=C.x+FOuterRadius*cos(DegToRad(FAngle/2.0));
     Y3=C.y+FOuterRadius*sin(DegToRad(FAngle/2.0));
     X4=C.x+FOuterRadius*cos(DegToRad(FAngle/2.0));
     Y4=C.y-FOuterRadius*sin(DegToRad(FAngle/2.0));
    }
  }
 else
  {
   if(FOrientation)
    {
     X3=C.x-FOuterRadius*sin(DegToRad(FAngle/2.0));
     Y3=C.y+FOuterRadius*cos(DegToRad(FAngle/2.0));
     X4=C.x+FOuterRadius*sin(DegToRad(FAngle/2.0));
     Y4=C.y+FOuterRadius*cos(DegToRad(FAngle/2.0));
    }
   else
    {
     X3=C.x-FOuterRadius*cos(DegToRad(FAngle/2.0));
     Y3=C.y-FOuterRadius*sin(DegToRad(FAngle/2.0));
     X4=C.x-FOuterRadius*cos(DegToRad(FAngle/2.0));
     Y4=C.y+FOuterRadius*sin(DegToRad(FAngle/2.0));
    }  
  }
 Canvas->Pie(X1,Y1,X2,Y2,X3,Y3,X4,Y4);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::WndProc(TMessage &Message)
 {
  switch(Message.Msg)
   {
    case WM_LBUTTONDOWN:
     {
      P.x=LOWORD(Message.LParam);
      P.y=HIWORD(Message.LParam);
      if(!FActive)
       break;
      if(PointIn(P,UpRect))
       {
        FIsUp=true;
        Changed(this);
       }
      else
      if(PointIn(P,DownRect))
       {
        FIsUp=false;
        Changed(this);
       }
      else
       {
        FIsUp=!FIsUp;
        Changed(this);
       }
      break;
     }
    case WM_LBUTTONUP:
     {
      break;
     }
    case WM_RBUTTONDOWN:
     {
      FActive=!FActive;
      Changed(this);
      break;
     }
    case WM_RBUTTONUP:
     {
      break;
     }
    case WM_MOUSEMOVE:
     {
      P.x=LOWORD(Message.LParam);
      P.y=HIWORD(Message.LParam);
      if(PointIn(P,UpRect))
       {
        Hint=FUpHint;
       }
      else
      if(PointIn(P,DownRect))
       {
        Hint=FDownHint;
       }
      else
       {
        if(FIsUp)
         Hint=FUpHint;
        else
         Hint=FDownHint;
       }  
      break;
     }
   }
  TControl::WndProc(Message);
 }
//---------------------------------------------------------------------------
bool __fastcall TOnOffButton::PointIn(TPoint P,TRect R)
{
 if((P.x>=R.Left)&&(P.x<=R.Right)&&(P.y>=R.Top)&&(P.y<=R.Bottom))
  return true;
 else
  return false; 
}
//---------------------------------------------------------------------------void __fastcall TOnOffButton::SetPen(TPen *Value)
void __fastcall TOnOffButton::SetPen(TPen *Value)
{
 FPen->Assign(Value);
}
//---------------------------------------------------------------------------void __fastcall TOnOffButton::SetPen(TPen *Value)
void __fastcall TOnOffButton::SetBrush(TBrush *Value)
{
 FBrush->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetFont(TFont *Value)
{
 FFont->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetUpFont(TFont *Value)
{
 FUpFont->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetDownFont(TFont *Value)
{
 FDownFont->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetInnerRadius(int Value)
{
 FInnerRadius=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetOuterRadius(int Value)
{
 FOuterRadius=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetUpText(AnsiString Value)
{
 FUpText=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetDownText(AnsiString Value)
{
 FDownText=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetIsUp(bool Value)
{
 FIsUp=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetAngle(float Value)
{
 FAngle=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetHandleColor(TColor Value)
{
 FHandleColor=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetActive(bool Value)
{
 FActive=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetUpHint(AnsiString Value)
{
 FUpHint=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetDownHint(AnsiString Value)
{
 FDownHint=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOffButton::SetOrientation(bool Value)
{
 if(FOrientation!=Value)
  {
   FOrientation=Value;
   Changed(this);
  }
}
//---------------------------------------------------------------------------void __fastcall TOnOffButton::SetPen(TPen *Value)

