//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OnOffButton"
#pragma link "Selecter"
#pragma link "BSelecter"
#pragma link "AHMTLeds"
#pragma link "Rbutton"
#pragma link "Rbutton"
#pragma link "MShape"
#pragma link "MShape"
#pragma link "OnOffButton"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ApplicationEvents1Hint(TObject *Sender)
{
 StatusBar1->Panels->Items[0]->Text=Application->Hint;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
 BSelecter1->Value=Edit1->Text;    
}
//---------------------------------------------------------------------------





