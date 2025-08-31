//---------------------------------------------------------------------------

#ifndef MovieH
#define MovieH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TMovie : public TGraphicControl
{
private:
        TImageList *FImageList;
        TTimer *FTimer;
        int FIndex;
        AnsiString FFileName;
        Graphics::TBitmap *FBitmap;
        int FSize;
        int FInterval;
        int FCount;
        bool FActive;
        void __fastcall DoTimer(TObject *Sender);
        void __fastcall SetInterval(int Value);
        void __fastcall SetIndex(int Value);
        void __fastcall SetActive(bool Value);
        int __fastcall GetInterval();
        void __fastcall Draw();
protected:
        void __fastcall Paint();
public:
        __fastcall TMovie(TComponent* Owner);
        __fastcall ~TMovie();

        void __fastcall LoadFromFile(AnsiString FileName);
__published:
        __property int Size={read=FSize};
        __property int Count={read=FCount};
        __property int Interval={read=GetInterval,write=SetInterval};
        __property int Index={read=FIndex,write=SetIndex};
        __property bool Active={read=FActive,write=SetActive};
};
//---------------------------------------------------------------------------
#endif
 