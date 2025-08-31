//---------------------------------------------------------------------------

#ifndef AButtonH
#define AButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAButton : public TGraphicControl
{
private:
        Graphics::TBitmap *FNormalBitmap;
        Graphics::TBitmap *FOverBitmap;
        Graphics::TBitmap *FClickBitmap;
        bool FMouseFriendy;
        HRGN FNormalRegion;
        HRGN FOverRegion;
        HRGN FClickRegion;
        bool FTransparent;
        void __fastcall SetNormalBitmap(Graphics::TBitmap *Value);
        void __fastcall SetOverBitmap(Graphics::TBitmap *Value);
        void __fastcall SetClickBitmap(Graphics::TBitmap *Value);
        void __fastcall SetTransparent(bool Value);
protected:
        void __fastcall Paint();
        DYNAMIC void __fastcall MouseDown(TMouseButton Button,Classes::TShiftState Shift,int X,int Y);
        DYNAMIC void __fastcall MouseUp(TMouseButton Button,Classes::TShiftState Shift,int X,int Y);
        void __fastcall CMMouseEnter(TMessage &Message);
        void __fastcall CMMouseExit(TMessage &Message);
        BEGIN_MESSAGE_MAP
          MESSAGE_HANDLER(CM_MOUSEENTER,TMessage,CMMouseEnter)
          MESSAGE_HANDLER(CM_MOUSELEAVE,TMessage,CMMouseExit)
        END_MESSAGE_MAP(TGraphicControl)
public:
        __fastcall TAButton(TComponent* Owner);
        __fastcall ~TAButton();
        void __fastcall MouseEnterEvent();
        void __fastcall MouseExitEvent();
        HRGN RegionFromBMP(Graphics::TBitmap *Mask,TColor BackColor);
__published:
        __property Graphics::TBitmap *NormalBitmap={read=FNormalBitmap,write=SetNormalBitmap};
        __property Graphics::TBitmap *OverBitmap={read=FOverBitmap,write=SetOverBitmap};
        __property Graphics::TBitmap *ClickBitmap={read=FClickBitmap,write=SetClickBitmap};
        __property bool MouseFriendy={read=FMouseFriendy,write=FMouseFriendy};
        __property bool Transparent={read=FTransparent,write=FTransparent};
};
//---------------------------------------------------------------------------
#endif
 