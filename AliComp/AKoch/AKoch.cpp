//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "AKoch.h"
#include <Math.h>
//---------------------------------------------------------------------------
static double cosa = 0.5;
static double sina = -sqrt(3.0)/2.0;
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TAKoch *)
{
        new TAKoch(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAKoch::TAKoch(TComponent* Owner)
        : TGraphicControl(Owner)
{
 Width=64;
 Height=64;
 FLevel=3;

 FBitmap=new Graphics::TBitmap();
 FBitmap->TransparentColor=clWhite;
}
//---------------------------------------------------------------------------
__fastcall TAKoch::~TAKoch()
{
 if(FBitmap)
  delete FBitmap;
}
//---------------------------------------------------------------------------
namespace Akoch
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TAKoch)};
                 RegisterComponents("AliSoft", classes, 0);
        }
}
//---------------------------------------------------------------------------
void __fastcall TAKoch::Paint()
{
 if((FBitmap->Width!=Width)||(FBitmap->Height!=Height))
  {
   FBitmap->Width=Width;
   FBitmap->Height=Height;
   FBitmap->Canvas->Pen->Color=clWhite;
   FBitmap->Canvas->Rectangle(0,0,Width,Height);

   Koch(0,FLevel,1,0,1,Height);
  }
 Canvas->Draw(0,0,FBitmap);
}
//---------------------------------------------------------------------------
void __fastcall TAKoch::Koch(int depth,int level,double x1,double y1,double x2,double y2 )
{
 if(level==depth)
  {
   FBitmap->Canvas->Pen->Color=clBlack;
   FBitmap->Canvas->MoveTo(int(x1),int(y1));
   FBitmap->Canvas->LineTo(int(x2),int(y2));
  }
 else
  {
   double dx = (x2 - x1)/3;
   double dy = (y2 - y1)/3;
   double xa = x1 + dx;
   double ya = y1 + dy;
   double xb = x2 - dx;
   double yb = y2 - dy;
   double xc = xa + dx*cosa - dy*sina;
   double yc = ya + dy*cosa + dx*sina;
   Koch(depth+1,level,x1,y1,xa,ya);
   Koch(depth+1,level,xa,ya,xc,yc);
   Koch(depth+1,level,xc,yc,xb,yb);
   Koch(depth+1,level,xb,yb,x2,y2);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAKoch::SetLevel(int Value)
{
 if((Value<0)||(Value>7))
  return;
 if(FLevel!=Value)
  {
   FLevel=Value;

   FBitmap->Width=Width;
   FBitmap->Height=Height;
   FBitmap->Canvas->Pen->Color=clWhite;
   FBitmap->Canvas->Rectangle(0,0,Width,Height);

   Koch(3,FLevel,1,0,1,Height);
   Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAKoch::SetTransparent(bool Value)
{
 if(FTransparent!=Value)
  {
   FTransparent=Value;
   FBitmap->Transparent=FTransparent;
   Invalidate();
  }
}
//---------------------------------------------------------------------------

