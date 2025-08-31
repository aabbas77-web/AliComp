//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "APanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAPanel *)
{
    new TAPanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAPanel::TAPanel(TComponent* Owner)
    : TPanel(Owner)
{
 FOnColor=clLime;
 FOffColor=clRed;
 Color=FIsOn?FOnColor:FOffColor;
}
//---------------------------------------------------------------------------
namespace Apanel
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAPanel)};
         RegisterComponents("AliSoft", classes, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAPanel::SetOnColor(TColor Value)
{
 if(FOnColor!=Value)
  {
   FOnColor=Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAPanel::SetOffColor(TColor Value)
{
 if(FOffColor!=Value)
  {
   FOffColor=Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAPanel::SetIsOn(bool Value)
{
 if(FIsOn!=Value)
  {
   FIsOn=Value;
   if(FIsOn)
    Color=FOnColor;
   else
    Color=FOffColor; 
  }
}
//---------------------------------------------------------------------------
