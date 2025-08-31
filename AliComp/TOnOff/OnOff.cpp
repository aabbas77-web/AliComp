//---------------------------------------------------------------------------
//   Up
//   |        Down -- Up
//  Down
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "OnOff.h"
#include <Math.hpp>
#include "Math.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TOnOff *)
{
    new TOnOff(NULL);
}
//---------------------------------------------------------------------------
__fastcall TOnOff::TOnOff(TComponent* Owner)
    : TGraphicControl(Owner)
{
 FPen=new TPen();
 FPen->OnChange=Changed;
 FBrush=new TBrush();
 FBrush->OnChange=Changed;
 FFont=new TFont();
 FFont->OnChange=Changed;
 FActiveFont=new TFont();
 FActiveFont->OnChange=Changed;

 Width=75;
 Height=106;

 FFont->Name="Traditional Arabic";
 FFont->Size=14;
 FFont->Color=clBlack;
 FActiveFont->Name="Traditional Arabic";
 FActiveFont->Size=16;
 FFont->Color=clBlue;
 FOuterRadius=20;
 FInnerRadius=8;
 FAngle=40;
 HandleColor=clGray;
 FActive=false;
}
//---------------------------------------------------------------------------
__fastcall TOnOff::~TOnOff()
{
 if(FPen)
  delete FPen;
 if(FBrush)
  delete FBrush; 
 if(FFont)
  delete FFont;
 if(FActiveFont)
  delete FActiveFont;
}
//---------------------------------------------------------------------------
namespace Onoff
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TOnOff)};
         RegisterComponents("AliSoft", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::Changed(TObject *Sender)
{
 Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::Paint()
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
   if(FUpLabel)
    FUpLabel->Font=FActiveFont;
   if(FDownLabel)
    FDownLabel->Font=FFont;
  }
 else
  {
   if(FUpLabel)
    FUpLabel->Font=FFont;
   if(FDownLabel)
    FDownLabel->Font=FActiveFont;
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
void __fastcall TOnOff::WndProc(TMessage &Message)
 {
  switch(Message.Msg)
   {
    case WM_LBUTTONDOWN:
     {
      if(!FActive)
       break;
      FIsUp=!FIsUp;
      Changed(this);
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

      if((FIsUp) &&(FUpLabel))
       Hint=FUpLabel->Hint;
      else
      if((!FIsUp) &&(FDownLabel))
       Hint=FDownLabel->Hint;
      break;
     }
   }
  TControl::WndProc(Message);
 }
//---------------------------------------------------------------------------
bool __fastcall TOnOff::PointIn(TPoint P,TRect R)
{
 if((P.x>=R.Left)&&(P.x<=R.Right)&&(P.y>=R.Top)&&(P.y<=R.Bottom))
  return true;
 else
  return false; 
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetPen(TPen *Value)
{
 FPen->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetBrush(TBrush *Value)
{
 FBrush->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetFont(TFont *Value)
{
 FFont->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetActiveFont(TFont *Value)
{
 FActiveFont->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetInnerRadius(int Value)
{
 if(FInnerRadius!=Value)
  {
   FInnerRadius=Value;
   Changed(this);
  } 
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetOuterRadius(int Value)
{
 if(FOuterRadius!=Value)
  {
   FOuterRadius=Value;
   Changed(this);
  } 
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetIsUp(bool Value)
{
 if(FIsUp!=Value)
  {
   FIsUp=Value;
   Changed(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetAngle(float Value)
{
 if(FAngle!=Value)
  {
   FAngle=Value;
   Changed(this);
  } 
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetHandleColor(TColor Value)
{
 if(FHandleColor!=Value)
  {
   FHandleColor=Value;
   Changed(this);
  } 
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetActive(bool Value)
{
 if(FActive!=Value)
  {
   FActive=Value;
   Changed(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetOrientation(bool Value)
{
 if(FOrientation!=Value)
  {
   FOrientation=Value;
   Changed(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetUpLabel(TLabel *Value)
{
 FUpLabel=Value;
 if(FUpLabel)
  FUpLabel->OnClick=FOnUpClick;
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::SetDownLabel(TLabel *Value)
{
 FDownLabel=Value;
 if(FDownLabel)
  FDownLabel->OnClick=FOnDownClick;
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::FOnUpClick(TObject *Sender)
{
 if(!FActive)
  return;
 FIsUp=true;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TOnOff::FOnDownClick(TObject *Sender)
{
 if(!FActive)
  return;
 FIsUp=false;
 Changed(this);
}
//---------------------------------------------------------------------------


