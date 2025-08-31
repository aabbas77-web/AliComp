//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "BSelecter.h"
#include "Math.h"
#include <stdio.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TBSelecter *)
{
    new TBSelecter(NULL);
}
//---------------------------------------------------------------------------
__fastcall TBSelecter::TBSelecter(TComponent* Owner)
    : TGraphicControl(Owner)
{
 FPen=new TPen();
 FPen->OnChange=Changed;
 FBrush=new TBrush();
 FBrush->OnChange=Changed;
 FPenShadow=new TPen();
 FPenShadow->OnChange=Changed;
 FBrushShadow=new TBrush();
 FBrushShadow->OnChange=Changed;

 FBackground=new Graphics::TBitmap();
 FBackground->OnChange=Changed;
 FBackground->Width=32;
 FBackground->Height=32;
 FBackground->Canvas->Ellipse(0,0,FBackground->Width,FBackground->Height);
 Width=FBackground->Width;
 Height=FBackground->Height;

 FList=new TStringList();
 FList->OnChange=Changed;
 FList->Add("1");
 FSelecterType=st3;

 FIndex=0;
 Value=FList->Strings[FIndex];

 FPen->Color=clBlack;
 FBrush->Style=bsSolid;
 FBrush->Color=TColor(0x00D9D9D9);

 FPenShadow->Color=clBlack;
 FBrushShadow->Style=bsSolid;
 FBrushShadow->Color=TColor(0x00595959);

 FTransX=1;
 FTransY=1;
 FTransparent=false;
 FActive=false;

 Points=new TPoint[3];

 FArrow=4;

 // V3
 V3[0]=Point(-10,10);
 V3[1]=Point(0,-14);
 V3[2]=Point(10,10);

 // V4
 V4[0]=Point(-11,10);
 V4[1]=Point(-11,-10);
 V4[2]=Point(10,-10);
 V4[3]=Point(10,10);

 // V5
 V5[0]=Point(-14,2);;
 V5[1]=Point(-12,-8);
 V5[2]=Point(-5,-13);
 V5[3]=Point(5,-13);
 V5[4]=Point(12,-7);

 // V8
 V8[0]=Point(-10,10);
 V8[1]=Point(-14,2);
 V8[2]=Point(-12,-8);
 V8[3]=Point(-5,-13);
 V8[4]=Point(5,-13);
 V8[5]=Point(12,-7);
 V8[6]=Point(13,2);
 V8[7]=Point(10,10);

 // V10
 V10[0]=Point(-10,10);
 V10[1]=Point(-14,4);
 V10[2]=Point(-14,-4);
 V10[3]=Point(-11,-11);
 V10[4]=Point(-4,-14);
 V10[5]=Point(4,-14);
 V10[6]=Point(10,-10);
 V10[7]=Point(13,-4);
 V10[8]=Point(13,4);
 V10[9]=Point(10,10);

 // V12
 V12[0]=Point(-12,12);
 V12[1]=Point(-17,7);
 V12[2]=Point(-18,-1);
 V12[3]=Point(-16,-8);
 V12[4]=Point(-11,-14);
 V12[5]=Point(-4,-18);
 V12[6]=Point(4,-18);
 V12[7]=Point(11,-14);
 V12[8]=Point(16,-8);
 V12[9]=Point(18,-1);
 V12[10]=Point(17,7);
 V12[11]=Point(12,12);

 // V14
 V14[0]=Point(-13,13);
 V14[1]=Point(-16,7);
 V14[2]=Point(-18,1);
 V14[3]=Point(-18,-5);
 V14[4]=Point(-14,-11);
 V14[5]=Point(-10,-16);
 V14[6]=Point(-3,-18);
 V14[7]=Point(3,-18);
 V14[8]=Point(10,-16);
 V14[9]=Point(14,-12);
 V14[10]=Point(17,-5);
 V14[11]=Point(18,1);
 V14[12]=Point(17,7);
 V14[13]=Point(13,13);

 // VArrows
 VArrows[0]=Point(-10,10);
 VArrows[1]=Point(-14,0);
 VArrows[2]=Point(-11,-11);
 VArrows[3]=Point(0,-14);
 VArrows[4]=Point(10,-10);
 VArrows[5]=Point(13,0);
 VArrows[6]=Point(10,10);
 VArrows[7]=Point(0,13);
}
//---------------------------------------------------------------------------
__fastcall TBSelecter::~TBSelecter()
{
 if(FPen)
  delete FPen;
 if(FBrush)
  delete FBrush;
 if(FPenShadow)
  delete FPenShadow;
 if(FBrushShadow)
  delete FBrushShadow;
 if(FBackground)
  delete FBackground;
 if(FList)
  delete FList;
 if(Points)
  delete[] Points;
}
//---------------------------------------------------------------------------
namespace Bselecter
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TBSelecter)};
         RegisterComponents("AliSoft", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::Changed(TObject *Sender)
{
 Invalidate();
}
//---------------------------------------------------------------------------
int __fastcall TBSelecter::Round(float f)
{
 int Cut=int(f);
 float Frac=f-Cut;
 if(Frac>=0.5)
  return Cut+1;
 else
  return Cut;
}
//---------------------------------------------------------------------------
float __fastcall TBSelecter::Distance(TPoint P1,TPoint P2)
{
 return sqrt(pow(P2.x-P1.x,2)+pow(P2.y-P1.y,2));
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::Paint()
{
 // Background
 if(Width!=FBackground->Width)
  Width=FBackground->Width;
 if(Height!=FBackground->Height)
  Height=FBackground->Height;
 Canvas->Draw(0,0,FBackground);

 if(FList->Count<=0)
  return;

 TPoint C=Point(FCenterX,FCenterY);
 // Calculation
 float Kx,Ky;
 float kx,ky;
 float L;
 TPoint p,p1,p2;

 // Arrow
 switch(FSelecterType)
  {
   case st3:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(V3[FIndex].x+FCenterX,V3[FIndex].y+FCenterY));
     Kx=(V3[FIndex].x)/L;
     Ky=(V3[FIndex].y)/L;
     break;
    }
   case st4:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(V4[FIndex].x+FCenterX,V4[FIndex].y+FCenterY));
     Kx=(V4[FIndex].x)/L;
     Ky=(V4[FIndex].y)/L;
     break;
    }
   case st5:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(V5[FIndex].x+FCenterX,V5[FIndex].y+FCenterY));
     Kx=(V5[FIndex].x)/L;
     Ky=(V5[FIndex].y)/L;
     break;
    }
   case st8:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(V8[FIndex].x+FCenterX,V8[FIndex].y+FCenterY));
     Kx=(V8[FIndex].x)/L;
     Ky=(V8[FIndex].y)/L;
     break;
    }
   case st10:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(V10[FIndex].x+FCenterX,V10[FIndex].y+FCenterY));
     Kx=(V10[FIndex].x)/L;
     Ky=(V10[FIndex].y)/L;
     break;
    }
   case st12:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(V12[FIndex].x+FCenterX,V12[FIndex].y+FCenterY));
     Kx=(V12[FIndex].x)/L;
     Ky=(V12[FIndex].y)/L;
     break;
    }
   case st14:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(V14[FIndex].x+FCenterX,V14[FIndex].y+FCenterY));
     Kx=(V14[FIndex].x)/L;
     Ky=(V14[FIndex].y)/L;
     break;
    }
   case stArrows:
    {
     L=Distance(Point(FCenterX,FCenterY),Point(VArrows[FIndex].x+FCenterX,VArrows[FIndex].y+FCenterY));
     Kx=(VArrows[FIndex].x)/L;
     Ky=(VArrows[FIndex].y)/L;
     break;
    }
  }
 kx=-Ky;
 ky=Kx;

 p.x=FCenterX+FRadius*Kx;
 p.y=FCenterY+FRadius*Ky;

 p1.x=FCenterX+FArrow*kx;
 p1.y=FCenterY+FArrow*ky;

 p2.x=FCenterX-FArrow*kx;
 p2.y=FCenterY-FArrow*ky;

 Canvas->Pen=FPen;
 Canvas->Brush=FBrush;
 Points[0]=C;
 Points[1]=p;
 Points[2]=p1;
 Canvas->Polygon(Points,2);

 Canvas->Pen=FPenShadow;
 Canvas->Brush=FBrushShadow;
 Points[0]=C;
 Points[1]=p;
 Points[2]=p2;
 Canvas->Polygon(Points,2);

 // Text
 if(FSelecterType!=stArrows)
  {
   Canvas->Brush->Style=bsClear;
   Canvas->TextOut(FTextX,FTextY,FList->Strings[FIndex]);
  }
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::WndProc(TMessage &Message)
 {
  switch(Message.Msg)
   {
    case WM_LBUTTONDOWN:
     {
      P.x=LOWORD(Message.LParam);
      P.y=HIWORD(Message.LParam);
      if(!FActive)
       break;

      if(FList->Count<=0)
       break;

      float Min=float(MaxInt);
      float D;
      for(int i=0;i<FList->Count;i++)
       {
        switch(FSelecterType)
         {
          case st3:
           {
            D=Distance(P,Point(V3[i].x+FCenterX,V3[i].y+FCenterY));
            break;
           }
          case st4:
           {
            D=Distance(P,Point(V4[i].x+FCenterX,V4[i].y+FCenterY));
            break;
           }
          case st5:
           {
            D=Distance(P,Point(V5[i].x+FCenterX,V5[i].y+FCenterY));
            break;
           }
          case st8:
           {
            D=Distance(P,Point(V8[i].x+FCenterX,V8[i].y+FCenterY));
            break;
           }
          case st10:
           {
            D=Distance(P,Point(V10[i].x+FCenterX,V10[i].y+FCenterY));
            break;
           }
          case st12:
           {
            D=Distance(P,Point(V12[i].x+FCenterX,V12[i].y+FCenterY));
            break;
           }
          case st14:
           {
            D=Distance(P,Point(V14[i].x+FCenterX,V14[i].y+FCenterY));
            break;
           }
          case stArrows:
           {
            D=Distance(P,Point(VArrows[i].x+FCenterX,VArrows[i].y+FCenterY));
            break;
           }
         }
        if(D<Min)
         {
          Min=D;
          FIndex=i;
         }
       }
      FValue=FList->Strings[FIndex];
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
      break;
     }
    case WM_RBUTTONUP:
     {
      break;
     }
    case WM_MOUSEMOVE:
     {
      break;
     }
   }
  TControl::WndProc(Message);
 }
//---------------------------------------------------------------------------
bool __fastcall TBSelecter::PointIn(TPoint P,TRect R)
{
 if((P.x>=R.Left)&&(P.x<=R.Right)&&(P.y>=R.Top)&&(P.y<=R.Bottom))
  return true;
 else
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetPen(TPen *Value)
{
 FPen->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetBrushShadow(TBrush *Value)
{
 FBrushShadow->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetPenShadow(TPen *Value)
{
 FPenShadow->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetBrush(TBrush *Value)
{
 FBrush->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetBackground(Graphics::TBitmap *Value)
{
 FBackground->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetList(TStringList *Value)
{
 FList->Assign(Value);
 if(FList->Count<=0)
  {
   FList->Add(0);
  }
 FIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetIndex(int Value)
{
 if(FIndex==Value)
  return;
 if((Value<FList->Count)&&(Value>=0))
  {
   FIndex=Value;
   this->Value=FList->Strings[FIndex];
   Changed(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetCenterX(int Value)
{
 if(FCenterX==Value)
  return;
 FCenterX=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetCenterY(int Value)
{
 if(FCenterY==Value)
  return;
 FCenterY=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetRadius(int Value)
{
 if(FRadius==Value)
  return;
 FRadius=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetTextX(int Value)
{
 if(FTextX==Value)
  return;
 FTextX=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetTextY(int Value)
{
 if(FTextY==Value)
  return;
 FTextY=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetTransX(int Value)
{
 if(FTransX==Value)
  return;
 FTransX=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetTransY(int Value)
{
 if(FTransY==Value)
  return;
 FTransY=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetTransparent(bool Value)
{
 if(FTransparent==Value)
  return;
 FTransparent=Value;
 if(FTransparent)
  {
   FBackground->Transparent=FTransparent;
   FBackground->TransparentColor=FBackground->Canvas->Pixels[FTransX][FTransY];
  }
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetSelecterType(TSelecterType Value)
{
 if(FSelecterType==Value)
  return;
 FSelecterType=Value;

 switch(FSelecterType)
  {
   case st3:
    {
     FRadius=13;
     FCenterX=30;
     FCenterY=43;

     FTextX=10;
     FTextY=89;
     break;
    }
   case st4:
    {
     FRadius=13;
     FCenterX=30;
     FCenterY=33;

     FTextX=10;
     FTextY=79;
     break;
    }
   case st5:
    {
     FRadius=13;
     FCenterX=37;
     FCenterY=42;

     FTextX=17;
     FTextY=88;
     break;
    }
   case st8:
    {
     FRadius=13;
     FCenterX=37;
     FCenterY=42;

     FTextX=17;
     FTextY=88;
     break;
    }
   case st10:
    {
     FRadius=13;
     FCenterX=37;
     FCenterY=42;

     FTextX=17;
     FTextY=88;
     break;
    }
   case st12:
    {
     FRadius=13;
     FCenterX=39;
     FCenterY=44;

     FTextX=22;
     FTextY=89;
     break;
    }
   case st14:
    {
     FRadius=17;
     FCenterX=44;
     FCenterY=49;

     FTextX=30;
     FTextY=101;
     break;
    }
   case stArrows:
    {
     FRadius=13;
     FCenterX=64;
     FCenterY=66;

     FTextX=17;
     FTextY=88;
     break;
    }
  }
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetActive(bool Value)
{
 if(FActive==Value)
  return;
 FActive=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetArrow(int Value)
{
 if(FArrow==Value)
  return;
 FArrow=Value;
 Changed(this);
}
//---------------------------------------------------------------------------
void __fastcall TBSelecter::SetValue(AnsiString Value)
{
 for(int i=0;i<FList->Count;i++)
  {
   if(UpperCase(FList->Strings[i])==UpperCase(Value))
    {
     FValue=Value;
     FIndex=i;
     Changed(this);
     break;
    }
  }
}
//---------------------------------------------------------------------------

