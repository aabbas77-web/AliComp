//---------------------------------------------------------------------------

#ifndef ARoundButtonH
#define ARoundButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
const int DefaultWidth=40;
const int DefaultHeight=40;

typedef enum {rtRegular,rtInset} TRButtonType;
//---------------------------------------------------------------------------
class PACKAGE TARoundButton : public TGraphicControl
{
private:
    Graphics::TBitmap *FGlyph;
    int FNumGlyphs;
    bool FDown;
    TColor FTransparentColor;
    bool FMouseDown;
    bool FMouseInside;
    TRButtonType FRButtonType;
    TNotifyEvent FOnClick;
    TNotifyEvent FOnMouseEnter;
    TNotifyEvent FOnMouseExit;
    TCursor FOverCursor;
    TCursor TempCursor;
    TTimer *Timer;
    int FMargin;
    int FPenWidth;
    TColor FColor;
    int FClickTime;

    bool __fastcall Assigned(void *P);
    void __fastcall DoTimer(TObject *Sender);
protected:
    void __fastcall Paint();
    DYNAMIC void __fastcall MouseDown(TMouseButton Button,TShiftState Shift,int X,int Y);
    DYNAMIC void __fastcall MouseMove(TShiftState Shift,int X,int Y);
    DYNAMIC void __fastcall MouseUp(TMouseButton Button,TShiftState Shift,int X,int Y);
    bool __fastcall IsInsideButton(int X,int Y);
    void __fastcall SetGlyph(Graphics::TBitmap *newGlyph);
    void __fastcall SetNumGlyphs(int newNumGlyphs);
    void __fastcall SetTransparentColor(TColor newTransparentColor);
    void __fastcall SetRButtonType(TRButtonType newRButtonType);
    void __fastcall SetOverCursor(TCursor Value);
    void __fastcall SetMargin(int Value);
    void __fastcall SetColor(TColor Value);
    void __fastcall SetClickTime(int Value);
    void __fastcall PaintButton();
public:
    __fastcall TARoundButton(TComponent* Owner);
    __fastcall ~TARoundButton();
    __inline int __fastcall Round(float f);
    void __fastcall DoClick();
__published:
    __property TRButtonType ButtonType={read=FRButtonType,write=SetRButtonType};
    __property Enabled;
    __property Graphics::TBitmap *Glyph={read=FGlyph,write=SetGlyph};
    __property int NumGlyphs={read=FNumGlyphs,write=SetNumGlyphs,default=1};
    __property ParentShowHint;
    __property ShowHint;
    __property TColor TransparentColor={read=FTransparentColor,write=SetTransparentColor};
    __property Visible;
    __property TNotifyEvent OnClick={read=FOnClick,write=FOnClick};
    __property TCursor OverCursor={read=FOverCursor,write=SetOverCursor};
    __property int Margin={read=FMargin,write=SetMargin};
    __property TColor Color={read=FColor,write=SetColor};
    __property int ClickTime={read=FClickTime,write=SetClickTime};
    __property OnMouseDown;
    __property OnMouseMove;
    __property OnMouseUp;
    __property TNotifyEvent OnMouseEnter={read=FOnMouseEnter,write=FOnMouseEnter};
    __property TNotifyEvent OnMouseExit={read=FOnMouseExit,write=FOnMouseExit};
};
//---------------------------------------------------------------------------
#endif
 