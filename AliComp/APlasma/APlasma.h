//---------------------------------------------------------------------------
#ifndef APlasmaH
#define APlasmaH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
typedef enum {ccRed,ccGreen,ccBlue,ccMix} TColorComponent;
//---------------------------------------------------------------------------
class PACKAGE TAPlasma : public TGraphicControl
{
private:
        TColorComponent FColorComponent;
        TTimer *FTimer;
        Graphics::TBitmap *BackBuffer;
        Byte PlasmaTable[256];
        bool FActive;
        TNotifyEvent FOnTimer;
        bool IsFirst;
        void __fastcall SetColorComponent(TColorComponent Value);
        void __fastcall SetActive(bool Value);
protected:
        void __fastcall Paint();
        void __fastcall WndProc(TMessage &Message);
public:
        __fastcall TAPlasma(TComponent* Owner);
        __fastcall ~TAPlasma();
        void __fastcall DoTimer(TObject *Sender);
__published:
        __property OnClick;
        __property OnDblClick;
        __property OnMouseDown;
        __property OnMouseMove;
        __property OnMouseUp;
        __property OnResize;
        __property Visible;
        __property Align;
        __property Anchors;
        __property BiDiMode;
        __property ShowHint;
        __property TColorComponent ColorComponent={read=FColorComponent,write=SetColorComponent};
        __property bool Active={read=FActive,write=SetActive};
        __property TNotifyEvent OnTimer={read=FOnTimer,write=FOnTimer};
};
//---------------------------------------------------------------------------
#endif
