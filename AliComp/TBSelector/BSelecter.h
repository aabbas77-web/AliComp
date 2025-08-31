//---------------------------------------------------------------------------

#ifndef BSelecterH
#define BSelecterH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
//const float PI=3.1415926535897932384626433832795;
//---------------------------------------------------------------------------
typedef enum{st3,st4,st5,st8,st10,st12,st14,stArrows} TSelecterType;
//---------------------------------------------------------------------------
class PACKAGE TBSelecter : public TGraphicControl
{
private:
    TPen *FPen;
    TBrush *FBrush;
    TPen *FPenShadow;
    TBrush *FBrushShadow;
    TStringList *FList;
    int FIndex;
    Graphics::TBitmap *FBackground;
    int FCenterX;
    int FCenterY;
    int FRadius;
    int FTextX;
    int FTextY;
    int FTransX;
    int FTransY;
    bool FTransparent;
    TSelecterType FSelecterType;
    bool FActive;
    int FArrow;
    AnsiString FValue;

    TPoint P;
    TPoint *Points;

    TPoint V14[14];
    TPoint V12[12];
    TPoint V10[10];
    TPoint V8[8];
    TPoint V5[5];
    TPoint V4[4];
    TPoint V3[3];
    TPoint VArrows[8];

    void __fastcall SetPen(TPen *Value);
    void __fastcall SetBrush(TBrush *Value);
    void __fastcall SetPenShadow(TPen *Value);
    void __fastcall SetBrushShadow(TBrush *Value);
    void __fastcall SetBackground(Graphics::TBitmap *Value);
    void __fastcall SetList(TStringList *Value);
    void __fastcall SetIndex(int Value);
    void __fastcall SetCenterX(int Value);
    void __fastcall SetCenterY(int Value);
    void __fastcall SetRadius(int Value);
    void __fastcall SetTextX(int Value);
    void __fastcall SetTextY(int Value);
    void __fastcall SetTransX(int Value);
    void __fastcall SetTransY(int Value);
    void __fastcall SetTransparent(bool Value);
    void __fastcall SetSelecterType(TSelecterType Value);
    void __fastcall SetActive(bool Value);
    void __fastcall SetArrow(int Value);
    void __fastcall SetValue(AnsiString Value);
protected:
    void __fastcall Paint();
    void __fastcall WndProc(TMessage &Message);
public:
    __fastcall TBSelecter(TComponent* Owner);
    __fastcall ~TBSelecter();
    void __fastcall Changed(TObject *Sender);
    bool __fastcall PointIn(TPoint P,TRect R);
    int __fastcall Round(float f);
    float __fastcall Distance(TPoint P1,TPoint P2);
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
    __property TPen *Pen={read=FPen,write=SetPen};
    __property TBrush *Brush={read=FBrush,write=SetBrush};
    __property TPen *PenShadow={read=FPenShadow,write=SetPenShadow};
    __property TBrush *BrushShadow={read=FBrushShadow,write=SetBrushShadow};
    __property Graphics::TBitmap *Background={read=FBackground,write=SetBackground};
    __property TStringList *List={read=FList,write=SetList};
    __property int Index={read=FIndex,write=SetIndex};
    __property int CenterX={read=FCenterX,write=SetCenterX};
    __property int CenterY={read=FCenterY,write=SetCenterY};
    __property int Radius={read=FRadius,write=SetRadius};
    __property int TextX={read=FTextX,write=SetTextX};
    __property int TextY={read=FTextY,write=SetTextY};
    __property int TransX={read=FTransX,write=SetTransX};
    __property int TransY={read=FTransY,write=SetTransY};
    __property bool Transparent={read=FTransparent,write=SetTransparent};
    __property TSelecterType SelecterType={read=FSelecterType,write=SetSelecterType};
    __property bool Active={read=FActive,write=SetActive};
    __property int Arrow={read=FArrow,write=SetArrow};
    __property AnsiString Value={read=FValue,write=SetValue};
};
//---------------------------------------------------------------------------
#endif
