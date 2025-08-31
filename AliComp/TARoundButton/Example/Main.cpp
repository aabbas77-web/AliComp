//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ARoundButton"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
 ARoundButton1->DoClick();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ARoundButton1Click(TObject *Sender)
{
 Color=(TColor)RGB(random(255),random(255),random(255));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
 ARoundButton2->DoClick();    
}
//---------------------------------------------------------------------------


