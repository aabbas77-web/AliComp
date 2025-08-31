//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AWav"
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
 WaveFile *Wav=new WaveFile();

 int Size=Edit1->Text.Length();
 Byte *Data=new Byte[Size+1];
 for(int i=0;i<Size;i++)
  Data[i]=Byte(Edit1->Text[i+1]);
 DDCRET Res;
 Res=Wav->OpenForWrite("Test.wav");
 MessageBox(0,DDCRET_String(Res),"Open for write",0);
 for(int i=0;i<1000;i++)
  Res=Wav->WriteData(Data,Size);
 MessageBox(0,DDCRET_String(Res),"Write data",0);
 Res=Wav->Close();
 MessageBox(0,DDCRET_String(Res),"Close",0);

 if(Wav)
  delete Wav;
}
//---------------------------------------------------------------------------
