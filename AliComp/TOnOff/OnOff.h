//---------------------------------------------------------------------------

#ifndef OnOffH
#define OnOffH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TOnOff : public TGraphicControl
{
private:
    TPen *FPen;
    TBrush *FBrush;
    TFont *FFont;
    TFont *FActiveFont;
    int FInnerRadius;
    int FOuterRadius;
    float FAngle;
    bool FIsUp;
    TColor FHandleColor;
    bool FActive;
    bool FOrientation;
    TLabel *FUpLabel;
    TLabel *FDownLabel;

    TPoint P;
    void __fastcall SetPen(TPen *Value);
    void __fastcall SetBrush(TBrush *Value);
    void __fastcall SetFont(TFont *Value);
    void __fastcall SetActiveFont(TFont *Value);
    void __fastcall SetInnerRadius(int Value);
    void __fastcall SetOuterRadius(int Value);
    void __fastcall SetIsUp(bool Value);
    void __fastcall SetAngle(float Value);
    void __fastcall SetHandleColor(TColor Value);
    void __fastcall SetActive(bool Value);
    void __fastcall SetOrientation(bool Value);
    void __fastcall SetUpLabel(TLabel *Value);
    void __fastcall SetDownLabel(TLabel *Value);
protected:
    void __fastcall Paint();
    void __fastcall WndProc(TMessage &Message);
    void __fastcall FOnUpClick(TObject *Sender);
    void __fastcall FOnDownClick(TObject *Sender);
public:
    __fastcall TOnOff(TComponent* Owner);
    __fastcall ~TOnOff();
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
    __property TFont *ActiveFont={read=FActiveFont,write=SetActiveFont};
    __property int InnerRadius={read=FInnerRadius,write=SetInnerRadius};
    __property int OuterRadius={read=FOuterRadius,write=SetOuterRadius};
    __property bool IsUp={read=FIsUp,write=SetIsUp};
    __property float Angle={read=FAngle,write=SetAngle};
    __property TColor HandleColor={read=FHandleColor,write=SetHandleColor};
    __property bool Active={read=FActive,write=SetActive};
    __property bool Orientation={read=FOrientation,write=SetOrientation};
    __property TLabel *UpLabel={read=FUpLabel,write=SetUpLabel};
    __property TLabel *DownLabel={read=FDownLabel,write=SetDownLabel};
};
//---------------------------------------------------------------------------
#endif
 