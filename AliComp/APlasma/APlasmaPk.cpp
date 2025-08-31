//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("APlasmaPk.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("APlasma.cpp");
USERES("APlasma.dcr");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------
