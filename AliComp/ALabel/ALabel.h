//---------------------------------------------------------------------------
#ifndef ALabelH
#define ALabelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
#include <Dsgnintf.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAboutALabelProperty : public TPropertyEditor
{
public:                                                     
    __fastcall TAboutALabelProperty(const _di_IFormDesigner ADesigner, int APropCount)
                                    :TPropertyEditor(ADesigner,APropCount){};
    __fastcall ~TAboutALabelProperty(){};
    virtual bool __fastcall AllEqual();
    virtual TPropertyAttributes __fastcall GetAttributes();
    virtual void __fastcall Edit();
    virtual AnsiString __fastcall GetValue();
};
//---------------------------------------------------------------------------
class  PACKAGE TAboutALabel : public TPersistent
{
private:
    AnsiString FProgrammer;
    void __fastcall SetProgrammer(AnsiString Value)
     {
      if(FProgrammer!=Value)
       {
        FProgrammer=Value;
       }
     }
public:
	__fastcall TAboutALabel(void){};
	__fastcall virtual ~TAboutALabel(void){};
__published:
    __property AnsiString Programmer={read=FProgrammer,write=SetProgrammer};
};
//---------------------------------------------------------------------------
class PACKAGE TALabel : public TLabel
{
private:
    TAboutALabel *FAbout;
    bool FMouseFriendy;
    TNotifyEvent FOnMouseEnterEvent,FOnMouseExitEvent;
    TColor FOnMouseColor;
    TColor FOnMouseExitColor;
    TFont *FFontOnMouse;
    TFont *FFontOnMouseExit;
    void __fastcall SetFont(TFont *Value);
protected:
    void __fastcall CMMouseEnter(TMessage &Message);
    void __fastcall CMMouseExit(TMessage &Message);
    BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(CM_MOUSEENTER,TMessage,CMMouseEnter)
     MESSAGE_HANDLER(CM_MOUSELEAVE,TMessage,CMMouseExit)
    END_MESSAGE_MAP(TLabel)
public:
    __fastcall TALabel(TComponent* Owner);
    __fastcall ~TALabel();
    void __fastcall MouseEnterEvent();
    void __fastcall MouseExitEvent();
__published:
    __property TAboutALabel *About={read=FAbout,write=FAbout};
    __property bool EnabledMouseFriendy={read=FMouseFriendy,write=FMouseFriendy};
    __property TColor ColorOnMouse={read=FOnMouseColor,write=FOnMouseColor};
    __property TNotifyEvent OnMouseEnter={read=FOnMouseEnterEvent,write=FOnMouseEnterEvent};
    __property TNotifyEvent OnMouseExit={read=FOnMouseExitEvent,write=FOnMouseExitEvent};
    __property TFont *FontOnMouse={read=FFontOnMouse,write=SetFont};
};
//---------------------------------------------------------------------------
#endif
