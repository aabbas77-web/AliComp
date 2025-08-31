//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BSelecter.h"
#include "AHMTLeds.hpp"
#include "rbutton.hpp"
#include "Rbutton.hpp"
#include "mshape.hpp"
#include "MShape.hpp"
#include "OnOffButton.h"
#include <AppEvnts.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TBSelecter *BSelecter1;
    TBSelecter *BSelecter2;
    TBSelecter *BSelecter3;
    TBSelecter *BSelecter4;
    TBSelecter *BSelecter6;
    TBSelecter *BSelecter7;
    TAHMLed *AHMLed1;
    TRoundButton *RoundButton1;
    TMultiShape *MultiShape1;
    TBSelecter *BSelecter8;
    TOnOffButton *OnOffButton1;
    TOnOffButton *OnOffButton2;
    TStatusBar *StatusBar1;
    TApplicationEvents *ApplicationEvents1;
    TEdit *Edit1;
    TButton *Button1;
    TBSelecter *BSelecter5;
    TBSelecter *BSelecter9;
    void __fastcall ApplicationEvents1Hint(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 