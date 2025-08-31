//---------------------------------------------------------------------------

#ifndef OnOffButtonH
#define OnOffButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TOnOffButton : public TGraphicControl
{
private:
    TPen *FPen;
    TBrush *FBrush;
    TFont *FFont;
    TFont *FUpFont;
    TFont *FDownFont;
    int FInnerRadius;
    int FOuterRadius;
    AnsiString FUpText;
    AnsiString FDownText;
    float FAngle;
    bool FIsUp;
    TColor FHandleColor;
    bool FActive;
    AnsiString FUpHint;
    AnsiString FDownHint;
    bool FOrientation;

    TPoint P;
    TRect UpRect,DownRect;
    void __fastcall SetPen(TPen *Value);
    void __fastcall SetBrush(TBrush *Value);
    void __fastcall SetFont(TFont *Value);
    void __fastcall SetUpFont(TFont *Value);
    void __fastcall SetDownFont(TFont *Value);
    void __fastcall SetInnerRadius(int Value);
    void __fastcall SetOuterRadius(int Value);
    void __fastcall SetUpText(AnsiString Value);
    void __fastcall SetDownText(AnsiString Value);
    void __fastcall SetIsUp(bool Value);
    void __fastcall SetAngle(float Value);
    void __fastcall SetHandleColor(TColor Value);
    void __fastcall SetActive(bool Value);
    void __fastcall SetUpHint(AnsiString Value);
    void __fastcall SetDownHint(AnsiString Value);
    void __fastcall SetOrientation(bool Value);
protected:
    void __fastcall Paint();
    void __fastcall WndProc(TMessage &Message);
public:
    __fastcall TOnOffButton(TComponent* Owner);
    __fastcall ~TOnOffButton();
    void __fastcall Changed(TObject *Sender);
    bool __fastcall PointIn(TPoint P,TRect R);
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
    __property TPen *Pen={read=FPen,write=SetPen};
    __property TBrush *Brush={read=FBrush,write=SetBrush};
    __property TFont *Font={read=FFont,write=SetFont};
    __property TFont *UpFont={read=FUpFont,write=SetUpFont};
    __property TFont *DownFont={read=FDownFont,write=SetDownFont};
    __property int InnerRadius={read=FInnerRadius,write=SetInnerRadius};
    __property int OuterRadius={read=FOuterRadius,write=SetOuterRadius};
    __property AnsiString UpText={read=FUpText,write=SetUpText};
    __property AnsiString DownText={read=FDownText,write=SetDownText};
    __property bool IsUp={read=FIsUp,write=SetIsUp};
    __property float Angle={read=FAngle,write=SetAngle};
    __property TColor HandleColor={read=FHandleColor,write=SetHandleColor};
    __property bool Active={read=FActive,write=SetActive};
    __property AnsiString UpHint={read=FUpHint,write=SetUpHint};
    __property AnsiString DownHint={read=FDownHint,write=SetDownHint};
    __property bool Orientation={read=FOrientation,write=SetOrientation};
};
//---------------------------------------------------------------------------
#endif
 