//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
 Region=new TRegion(Handle);
 X=1;
 Y=1;
 dX=Width/(2*Timer1->Interval);
 dY=Height/(2*Timer1->Interval);
 Region->Active=true;
// Region->Active=false;
 Clicked=false;
 Path=ExtractFilePath(Application->ExeName)+"\\";

 OpenDialog1->InitialDir=Path;
 SaveDialog1->InitialDir=Path;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
 if(Region)
  delete Region;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Clear()
{
 Canvas->Brush->Color=clBtnFace;
 Canvas->Brush->Style=bsSolid;
 Canvas->Pen->Color=clBtnFace;
 Canvas->Rectangle(0,0,Width-1,Height-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PaintRegion()
{
 Region->Brush->Color=clBlue;
 Region->Brush->Style=bsSolid;
 Region->Paint();

 Region->FrameBrush->Color=clRed;
 Region->FrameBrush->Style=bsSolid;
 Region->Frame(1,1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Button1Click(TObject *Sender)
{
 Region->Rectangle(10,10,200,100);
 PaintRegion();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button5Click(TObject *Sender)
{
 Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button2Click(TObject *Sender)
{
 Region->Ellipse(10,10,200,100);
 PaintRegion();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button3Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
  {
   Region->SaveToFile(SaveDialog1->FileName);
  } 
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button4Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
  {
   Region->LoadFromFile(OpenDialog1->FileName);
//   PaintRegion();
   Region->Active=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Timer1Timer(TObject *Sender)
{
// Region->Rectangle(Width/2-X,Height/2-Y,Width/2+X,Height/2+Y);
 Region->Ellipse(Width/2-X,Height/2-Y,Width/2+X,Height/2+Y);
/*
 Points[0]=TPoint(0,0);
 Points[1]=TPoint(X,0);
 Points[2]=TPoint(0,Y);
 Region->Polygon(Points,3,::fmWinding);
*/
 Region->Active=true;
 X+=dX;
 Y+=dY;
 if((X>=Width)&&(Y>=Height))
  Timer1->Enabled=false;    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button6Click(TObject *Sender)
{
 if(OpenPictureDialog1->Execute())
  {
   Graphics::TBitmap *Bitmap=new Graphics::TBitmap();
   Bitmap->LoadFromFile(OpenPictureDialog1->FileName);

   Region->LoadFromBMP(Bitmap,clWhite);
   PaintRegion();

   if(Bitmap)
    delete Bitmap;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Clicked=true;
 LX=X;
 LY=Y;       
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 if(Clicked)
  {
   Left+=X-LX;
   Top+=Y-LY;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Image1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Clicked=false;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Active1Click(TObject *Sender)
{
 Region->Active=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NotActive1Click(TObject *Sender)
{
 Region->Active=false;
}
//---------------------------------------------------------------------------

