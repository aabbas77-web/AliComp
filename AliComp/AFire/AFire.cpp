//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AFire.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAFire *)
{
        new TAFire(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAFire::TAFire(TComponent* Owner)
        : TGraphicControl(Owner)
{
}
//---------------------------------------------------------------------------
namespace Afire
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TAFire)};
                 RegisterComponents("AliSoft", classes, 0);
        }
}
//---------------------------------------------------------------------------
 