//---------------------------------------------------------------------------

#ifndef AliImageH
#define AliImageH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include <Dsgnintf.hpp>
//---------------------------------------------------------------------------
class PACKAGE TFileNameProperty : public TStringProperty
{
public:
    __fastcall TFileNameProperty(const _di_IFormDesigner ADesigner, int APropCount)
                                    :TStringProperty(ADesigner,APropCount){};
    __fastcall ~TFileNameProperty(){};
    virtual TPropertyAttributes __fastcall GetAttributes();
    virtual void __fastcall Edit();
};
//---------------------------------------------------------------------------
class PACKAGE TAliImage : public TGraphicControl
{
private:
        Graphics::TBitmap *FBitmap;
        AnsiString FFileName;
        void __fastcall SetBitmap(Graphics::TBitmap *Value);
        void __fastcall SetFileName(AnsiString Value);
protected:
        void __fastcall Paint();
public:
        __fastcall TAliImage(TComponent* Owner);
        __fastcall ~TAliImage();
__published:
        __property Graphics::TBitmap *Bitmap={read=FBitmap,write=SetBitmap};    
        __property AnsiString FileName={read=FFileName,write=SetFileName};
};
//---------------------------------------------------------------------------
#endif
