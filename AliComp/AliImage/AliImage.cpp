//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AliImage.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAliImage *)
{
        new TAliImage(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAliImage::TAliImage(TComponent* Owner)
        : TGraphicControl(Owner)
{
 FBitmap=new Graphics::TBitmap();
 FBitmap->Width=32;
 FBitmap->Height=32;

 FBitmap->Canvas->Brush->Style=bsDiagCross;
 FBitmap->Canvas->Brush->Color=clBlack;
 FBitmap->Canvas->Ellipse(0,0,FBitmap->Width,FBitmap->Height);

 Width=FBitmap->Width;
 Height=FBitmap->Height;
}
//---------------------------------------------------------------------------
__fastcall TAliImage::~TAliImage()
{
 if(FBitmap)
  delete FBitmap;
}
//---------------------------------------------------------------------------
namespace Aliimage
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TAliImage)};
                 RegisterComponents("AliSoft", classes, 0);

                 TPropInfo* PropInfo=::GetPropInfo(__typeinfo(TAliImage),"FileName");
                 RegisterPropertyEditor(*PropInfo->PropType,
                                        __classid(TAliImage),"FileName",
                                        __classid(TFileNameProperty));
        }
}
//---------------------------------------------------------------------------
void __fastcall TAliImage::Paint()
{
 Canvas->Draw(0,0,FBitmap);
}
//---------------------------------------------------------------------------
void __fastcall TAliImage::SetBitmap(Graphics::TBitmap *Value)
{
 FBitmap->Assign(Value);
 Width=FBitmap->Width;
 Height=FBitmap->Height;
 Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TAliImage::SetFileName(AnsiString Value)
{
 FFileName=Value;
 FBitmap->LoadFromFile(FFileName);
 Width=FBitmap->Width;
 Height=FBitmap->Height;
 Invalidate();
}
//---------------------------------------------------------------------------
//-------------------TFileName-----------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFileNameProperty::Edit()
{
 TOpenDialog *OpenDialog=new TOpenDialog(Application);
 OpenDialog->DefaultExt="bmp";
 OpenDialog->Filter="Bitmap Image (*.bmp)|*.bmp";
 OpenDialog->FileName=GetValue();
 OpenDialog->Options<<ofShowHelp<<ofPathMustExist<<ofFileMustExist;
 try
  {
   if(OpenDialog->Execute())
    SetValue(OpenDialog->FileName);
  }
 __finally
  {
   if(OpenDialog)
    delete OpenDialog;
  }
}
//---------------------------------------------------------------------------
TPropertyAttributes __fastcall TFileNameProperty::GetAttributes()
{
 return TPropertyAttributes()<<paDialog<<paRevertable;
}
//---------------------------------------------------------------------------
