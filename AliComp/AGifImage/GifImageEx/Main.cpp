//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AGifImage"
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Button1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
  {
   FileName=OpenDialog1->FileName;
   AGIFImage1->Load(FileName.c_str());
   AGIFImage1->Draw();       
  }
}
//---------------------------------------------------------------------------

