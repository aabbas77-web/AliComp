//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "ALabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TALabel *)
{
    new TALabel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TALabel::TALabel(TComponent* Owner)
    : TLabel(Owner)
{
 FFontOnMouse=new TFont();
 FFontOnMouseExit=new TFont();
 FAbout=new TAboutALabel();
 FAbout->Programmer="Ali Abbas";
 EnabledMouseFriendy=true;
 ColorOnMouse=clBtnFace;
 FFontOnMouse->Color=clYellow;
}
//---------------------------------------------------------------------------
__fastcall TALabel::~TALabel()
{
 if(FFontOnMouse)
  delete FFontOnMouse;
 if(FFontOnMouseExit)
  delete FFontOnMouseExit;
 if(FAbout)
  delete FAbout;
}
//---------------------------------------------------------------------------
namespace Alabel
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TALabel)};
         RegisterComponents("AliSoft", classes, 0);
         RegisterPropertyEditor(__typeinfo(TAboutALabel),NULL,"",__classid(TAboutALabelProperty));
    }
}
//---------------------------------------------------------------------------
void __fastcall TALabel::SetFont(TFont *Value)
{
 FFontOnMouse->Assign(Value);
}
//---------------------------------------------------------------------------
void __fastcall TALabel::MouseEnterEvent()
{
 if(Enabled && FMouseFriendy)
  {
   FOnMouseExitColor=Color;
   FFontOnMouseExit->Assign(Font);
   Color=FOnMouseColor;
   Font->Assign(FFontOnMouse);
   if(FOnMouseEnterEvent)
    FOnMouseEnterEvent(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TALabel::MouseExitEvent()
{
 if(Enabled && FMouseFriendy)
  {
   Color=FOnMouseExitColor;
   Font->Assign(FFontOnMouseExit);
   if(FOnMouseExitEvent)
    FOnMouseExitEvent(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TALabel::CMMouseEnter(TMessage &Message)
{
 MouseEnterEvent();
}
//---------------------------------------------------------------------------
void __fastcall TALabel::CMMouseExit(TMessage &Message)
{
 MouseExitEvent();
}
//---------------------------------------------------------------------------
//-------------------TAboutMouseFriendly-------------------------------------
//---------------------------------------------------------------------------
void __fastcall TAboutALabelProperty::Edit()
{
 AnsiString Msg;
 Msg=AnsiString("AliSoft Magic Components V1.0 \n")+
     AnsiString("* TALabel * \n")+
     AnsiString("By Ali Abbas \n")+
     AnsiString("~ a.abbas@netcourrier.com ~ \n")+
     AnsiString("Syria - Damascus in 1/9/2001 \n")+
     GetStrValue();
 Application->MessageBox(Msg.c_str(),"AliSoft <FreeWare>",MB_ICONASTERISK);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TAboutALabelProperty::GetValue()
{
 return "About TALabel";
}
//---------------------------------------------------------------------------
TPropertyAttributes __fastcall TAboutALabelProperty::GetAttributes()
{
 return TPropertyAttributes()<<paMultiSelect<<paDialog<<paReadOnly;
}
//---------------------------------------------------------------------------
bool __fastcall TAboutALabelProperty::AllEqual()
{
 return true;
}
//---------------------------------------------------------------------------

