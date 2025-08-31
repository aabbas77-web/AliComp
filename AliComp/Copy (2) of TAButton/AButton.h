//---------------------------------------------------------------------------
#ifndef AButtonH
#define AButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAButton : public TCustomControl
{
private:
        Graphics::TBitmap *FNormalBitmap;
        Graphics::TBitmap *FOverBitmap;
        Graphics::TBitmap *FClickBitmap;
        bool FMouseFriendy;
        HRGN FRegion;
        bool FActive;
        TColor FTransparentColor;
        TNotifyEvent FOnMouseEnterEvent;
        TNotifyEvent FOnMouseExitEvent;
        void __fastcall SetNormalBitmap(Graphics::TBitmap *Value);
        void __fastcall SetOverBitmap(Graphics::TBitmap *Value);
        void __fastcall SetClickBitmap(Graphics::TBitmap *Value);
        void __fastcall SetActive(bool Value);
        void __fastcall SetTransparentColor(TColor Value);
protected:
        void __fastcall Paint();
        DYNAMIC void __fastcall MouseDown(TMouseButton Button,Classes::TShiftState Shift,int X,int Y);
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
        __property Graphics::TBitmap *NormalBitmap={read=FNormalBitmap,write=SetNormalBitmap};
        __property Graphics::TBitmap *OverBitmap={read=FOverBitmap,write=SetOverBitmap};
        __property Graphics::TBitmap *ClickBitmap={read=FClickBitmap,write=SetClickBitmap};
        __property bool MouseFriendy={read=FMouseFriendy,write=FMouseFriendy};
        __property bool Active={read=FActive,write=SetActive};
        __property TColor TransparentColor={read=FTransparentColor,write=SetTransparentColor};
};
//---------------------------------------------------------------------------
#endif

