//---------------------------------------------------------------------------

#ifndef AKochH
#define AKochH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAKoch : public TGraphicControl
{
private:
        Graphics::TBitmap *FBitmap;
        int FLevel;
        bool FTransparent;
        void __fastcall SetLevel(int Value);
        void __fastcall SetTransparent(bool Value);
protected:
        void __fastcall Paint();
public:
        __fastcall TAKoch(TComponent* Owner);
        __fastcall ~TAKoch();
        void __fastcall Koch(int depth,int level,double x1,double y1,double x2,double y2 );
__published:
        __property int Level={read=FLevel,write=SetLevel};
        __property bool Transparent={read=FTransparent,write=SetTransparent};
};
//---------------------------------------------------------------------------
#endif
 