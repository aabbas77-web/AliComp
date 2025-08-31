//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Movie.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMovie *)
{
        new TMovie(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMovie::TMovie(TComponent* Owner)
        : TGraphicControl(Owner)
{
 FImageList=new TImageList(this);;
 FTimer=new TTimer(this);
 FBitmap=new Graphics::TBitmap();
 FIndex=0;
 FCount=0;
 FSize=32;
 FTimer->OnTimer=DoTimer;
 FTimer->Interval=40;
 FTimer->Enabled=false;
}
//---------------------------------------------------------------------------
__fastcall TMovie::~TMovie()
{
 if(FImageList)
  delete FImageList;
 if(FTimer)
  delete FTimer;
 if(FBitmap)
  delete FBitmap; 
}
//---------------------------------------------------------------------------
namespace Movie
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TMovie)};
                 RegisterComponents("Mohcen", classes, 0);
        }
}
//---------------------------------------------------------------------------
void __fastcall TMovie::Paint()
{
 if(FCount<=0)
 {
  Width=FSize;
  Height=FSize;
  Canvas->Rectangle(0,0,Width,Height);
 }
 else
  Draw(); 
}
//---------------------------------------------------------------------------
void __fastcall TMovie::LoadFromFile(AnsiString FileName)
{
 FFileName=FileName;
 FBitmap->LoadFromFile(FFileName);
 int W=FBitmap->Width;
 int H=FBitmap->Height;
 if(FBitmap->Width<=FBitmap->Height)
  FSize=W;
 else
  FSize=H;
 Graphics::TBitmap *B=new Graphics::TBitmap();
 B->Width=FSize;
 B->Height=FSize;
 TRect Dest,Source;
 Dest=Rect(0,0,FSize,FSize);
 FCount=0;
 FImageList->Clear();
 FImageList->Width=FSize;
 FImageList->Height=FSize;
 for(int Y=0;Y<H;Y+=FSize)
 {
  for(int X=0;X<W;X+=FSize)
  {
   Source=Rect(X+0,Y+0,X+FSize,Y+FSize);
   B->Canvas->CopyRect(Dest,FBitmap->Canvas,Source);
   FImageList->Add(B,NULL);
   FCount++;
  }
 }
 if(B)
  delete B;
 FIndex=0;
 Draw();
}
//---------------------------------------------------------------------------
void __fastcall TMovie::DoTimer(TObject *Sender)
{
 FIndex++;
 if(FIndex>=FCount)
 {
  FIndex=0;
 }
 Draw();
}
//---------------------------------------------------------------------------
void __fastcall TMovie::SetInterval(int Value)
{
 if(Value!=int(FTimer->Interval))
 {
  FTimer->Interval=Value;
 }
}
//---------------------------------------------------------------------------
void __fastcall TMovie::SetIndex(int Value)
{
 if(Value!=FIndex)
 {
  FIndex=Value;
  Draw();
 }
}
//---------------------------------------------------------------------------
void __fastcall TMovie::Draw()
{
 Width=FSize;
 Height=FSize;
 FImageList->Draw(Canvas,0,0,FIndex,true);
}
//---------------------------------------------------------------------------
void __fastcall TMovie::SetActive(bool Value)
{
 if(Value!=FActive)
 {
  FActive=Value;
  FTimer->Enabled=FActive;
 }
}
//---------------------------------------------------------------------------
int __fastcall TMovie::GetInterval()
{
 return FTimer->Interval;
}
//---------------------------------------------------------------------------

