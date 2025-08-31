//---------------------------------------------------------------------------

#ifndef APanelH
#define APanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAPanel : public TPanel
{
private:
    TColor FOnColor;
    TColor FOffColor;
    bool FIsOn;
    void __fastcall SetOnColor(TColor Value);
    void __fastcall SetOffColor(TColor Value);
    void __fastcall SetIsOn(bool Value);
protected:
public:
    __fastcall TAPanel(TComponent* Owner);
__published:
    __property TColor OnColor={read=FOnColor,write=SetOnColor};
    __property TColor OffColor={read=FOffColor,write=SetOffColor};
    __property bool IsOn={read=FIsOn,write=SetIsOn};
};
//---------------------------------------------------------------------------
#endif
