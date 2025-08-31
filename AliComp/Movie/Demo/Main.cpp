//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Movie"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Open1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
 {
  FileName=OpenDialog1->FileName;
  Movie1->LoadFromFile(FileName);
 }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ToolButton1Click(TObject *Sender)
{
 Movie1->Active=true;        
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ToolButton2Click(TObject *Sender)
{
 Movie1->Active=false;        
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ToolButton3Click(TObject *Sender)
{
 Movie1->Active=false;
 Movie1->Index=0;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ToolButton4Click(TObject *Sender)
{
 Movie1->Active=false;
 Movie1->Index=Movie1->Count-1;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::CSpinEdit1Change(TObject *Sender)
{
 Movie1->Interval=CSpinEdit1->Value;        
}
//---------------------------------------------------------------------------
