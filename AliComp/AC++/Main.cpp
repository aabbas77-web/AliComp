//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit1Change(TObject *Sender)
{
 if(IsValidIdent(Edit1->Text))
  {
   Label2->Caption="Valid C++ Identifier";
   Label2->Color=clLime;
  }
 else
  {
   Label2->Caption="Invalid C++ Identifier";
   Label2->Color=clRed;
  }
}
//---------------------------------------------------------------------------
