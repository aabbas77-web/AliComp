//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AWav.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAWav *)
{
    new TAWav(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAWav::TAWav(TComponent* Owner)
    : TComponent(Owner)
{
 Wav=new WaveFile();
}
//---------------------------------------------------------------------------
__fastcall TAWav::~TAWav()
{
 if(Wav)
  delete Wav;
}
//---------------------------------------------------------------------------
namespace Awav
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TAWav)};
         RegisterComponents("AliSoft", classes, 0);
    }
}
//---------------------------------------------------------------------------
 