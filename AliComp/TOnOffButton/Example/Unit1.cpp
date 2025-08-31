//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "OnOffButton"
#pragma link "Selecter"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnOffButton1Click(TObject *Sender)
{
 if(OnOffButton1->IsUp)
  Label1->Caption="Up";
 else
  Label1->Caption="Down";
}
//---------------------------------------------------------------------------







