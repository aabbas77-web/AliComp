//---------------------------------------------------------------------------
#ifndef AButtonH
#define AButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include "AJPEG.hpp"
//---------------------------------------------------------------------------
class PACKAGE TAButton : public TCustomControl
{
private:
        TJPEGImage *FNormalImage;
        TJPEGImage *FOverImage;
        TJPEGImage *FClickImage;
        Graphics::TBitmap *FBitmap;
        bool FMouseFriendy;
        HRGN FRegion;
        bool FActive;
        bool Clicked;
        TColor FTransparentColor;
        TNotifyEvent FOnMouseEnterEvent;
        TNotifyEvent FOnMouseExitEvent;
        int FInterval;
        bool FAutoSize;
        void __fastcall SetNormalImage(TJPEGImage *Value);
        void __fastcall SetOverImage(TJPEGImage *Value);
        void __fastcall SetClickImage(TJPEGImage *Value);
        void __fastcall SetActive(bool Value);
        void __fastcall SetTransparentColor(TColor Value);
        void __fastcall SetInterval(int Value);
        void __fastcall SetAutoSize(bool Value);
protected:
        void __fastcall Paint();
        DYNAMIC void __fastcall MouseDown(TMouseButton Button,Classes::TShiftState Shift,int X,int Y);
        DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift,int X,int Y);
        DYNAMIC void __fastcall MouseUp(TMouseButton Button,Classes::TShiftState Shift,int X,int Y);
        void __fastcall CMMouseEnter(TMessage &Message);
        void __fastcall CMMouseExit(TMessage &Message);
        BEGIN_MESSAGE_MAP
          MESSAGE_HANDLER(CM_MOUSEENTER,TMessage,CMMouseEnter)
          MESSAGE_HANDLER(CM_MOUSELEAVE,TMessage,CMMouseExit)
        END_MESSAGE_MAP(TCustomControl)
public:
        __fastcall TAButton(TComponent* Owner);
        __fastcall ~TAButton();
        void __fastcall MouseEnterEvent();
        void __fastcall MouseExitEvent();
        void RegionFromBMP(Graphics::TBitmap *Mask,TColor BackColor);
        HRGN CopyRegion(HRGN Value);
        void __fastcall Monocrom(Graphics::TBitmap *BMP,TColor BackColor);

__published:
        __property OnClick;
        __property OnDblClick;
        __property OnMouseDown;
        __property OnMouseMove;
        __property OnMouseUp;
        __property Visible;
        __property Align;
        __property Anchors;
        __property BiDiMode;
        __property ShowHint;
        __property TNotifyEvent OnMouseEnter={read=FOnMouseEnterEvent,write=FOnMouseEnterEvent};
        __property TNotifyEvent OnMouseExit={read=FOnMouseExitEvent,write=FOnMouseExitEvent};
        __property TJPEGImage *NormalImage={read=FNormalImage,write=SetNormalImage};
        __property TJPEGImage *OverImage={read=FOverImage,write=SetOverImage};
        __property TJPEGImage *ClickImage={read=FClickImage,write=SetClickImage};
        __property bool MouseFriendy={read=FMouseFriendy,write=FMouseFriendy};
        __property bool Active={read=FActive,write=SetActive};
        __property TColor TransparentColor={read=FTransparentColor,write=SetTransparentColor};
        __property int Interval={read=FInterval,write=SetInterval};
        __property bool AutoSize={read=FAutoSize,write=SetAutoSize};
};
//---------------------------------------------------------------------------
#endif

