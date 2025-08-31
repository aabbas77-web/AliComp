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
 Gif=new TAGif(Application);
 Gif->Parent=this;
 Gif->Left=0;
 Gif->Top=0;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
// if(Gif)
//  delete Gif;        
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
  {
   Gif->Load(OpenDialog1->FileName);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button2Click(TObject *Sender)
{
 Gif->Draw();        
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button3Click(TObject *Sender)
{
 Gif->Stop();        
}
//---------------------------------------------------------------------------

